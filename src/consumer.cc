/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <mpd/client.h>
#include "consumer.hh"
#include "globals.hh"

using namespace boost::filesystem;
using namespace std;

#include <fstream>
Consumer::Consumer(string&& throwaway)
    : _playlist_file_(std::move(throwaway))
    , _write_to_file_(true) {

    std::ifstream file(_playlist_file_);

    if (file) {
        string temp;
        while (getline(file, temp)) {
            _playlist_lines_.insert(temp);
        }
    }
}

void Consumer::_sorted_insert_song_(const std::string& str) {
    bool inserted = _playlist_lines_.insert(str).second;
    if (NONO && inserted) {
        printf("Insert `%s` into playlist `%s`.\n", str.c_str(),
               _playlist_file_.c_str());
    }
}

Consumer::~Consumer() {
    if (_write_to_file_) {
        if (!NONO) {
            if (auto file = std::ofstream(_playlist_file_)) {
                for (const auto& s : _playlist_lines_) {
                    file << s << '\n';
                }
                if (USE_MPD) {
                    _run_mpd_();
                }
            } else {
                fprintf(stderr,
                        "Problem opening file %s!\nAborting!\n",
                        _playlist_file_.c_str());
                exit(EXIT_FAILURE);
            }
        } else if (USE_MPD) {
            printf("Will clear the mpd playlist, update the mpd "
                   "database, and load %s.\n",
                   path(_playlist_file_)
                       .filename()
                       .replace_extension()
                       .c_str());
        }
    }
}

#include <taglib/fileref.h>
void Consumer::_apply_tags_(const path& path, const string& artist,
                            const string& title) {
    using namespace TagLib;
    bool changed = false;

    FileRef file(path.c_str());
    Tag* tags = file.tag();

    if (!tags) {
        fprintf(stderr, "Music file `%s` has no tags!\n", path.c_str());
        return;
    }

    if (tags->artist() != artist) {
        if (NONO) {
            printf("  Artist: `%s` -> `%s`\n",
                   tags->artist().toCString(), artist.c_str());
        } else {
            changed = true;
            tags->setArtist(artist);
        }
    }

    if (tags->title() != title) {
        if (NONO) {
            printf("  Title: `%s` -> `%s`\n",
                   tags->title().toCString(), title.c_str());
        } else {
            changed = true;
            tags->setTitle(title);
        }
    }

    if (changed) {
        file.save();
    }
}
