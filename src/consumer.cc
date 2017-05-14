/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <mpd/client.h>
#include <taglib/fileref.h>
#include <stdexcept>

#include "consumer.hh"
#include "globals.hh"

struct ifstream {
    std::FILE* file;

    explicit ifstream(const char* path) :
        file(std::fopen(path, "r")) {}
    ~ifstream() noexcept {
        if (file) {
            std::fclose(file);
        }
    }
    ifstream(ifstream&& other) noexcept : file(nullptr) {
        *this = std::move(other);
    }
    ifstream& operator=(ifstream&& other) noexcept {
        std::swap(file, other.file);
        return *this;
    }

    bool read_line(std::string& other) {
        other.clear();
        for (int c = std::fgetc(file); c != EOF;
             c = std::fgetc(file)) {
            if (c == int('\n')) {
                return true;
            } else {
                other += char(c);
            }
        }
        return false;
    }

    operator bool() noexcept { return file; }
};

struct ofstream {
    std::FILE* file;

    explicit ofstream(const char* path)
        : file(std::fopen(path, "w")) {}
    ~ofstream() noexcept {
        if (file) {
            std::fclose(file);
        }
    }
    ofstream(ofstream&& other) noexcept : file(nullptr) {
        *this = std::move(other);
    }
    ofstream& operator=(ofstream&& other) noexcept {
        std::swap(file, other.file);
        return *this;
    }

    bool write(const char* str) noexcept {
        return std::fputs(str, file) != EOF;
    }

    bool write(char c) noexcept {
        return std::fputc(c, file) != EOF;
    }

    operator bool() noexcept { return file; }
};

Consumer::Consumer(std::string&& throwaway)
    : _playlist_file_(std::move(throwaway))
    , _write_to_file_(true) {

    if (auto file = ifstream(_playlist_file_.c_str())) {
        std::string temp;
        while (file.read_line(temp)) {
            _playlist_lines_.insert(temp);
        }
    }
}

void Consumer::_sorted_insert_song_(const std::string& str) {
    if (_playlist_lines_.insert(str).second && NONO) {
        std::printf("Insert `%s` into playlist `%s`.\n", str.c_str(),
                    _playlist_file_.c_str());
    }
}

Consumer::~Consumer() {
    if (_write_to_file_) {
        if (!NONO) {
            if (auto file = ofstream(_playlist_file_.c_str())) {
                for (const auto& s : _playlist_lines_) {
                    if (!(file.write(s.c_str()) &&
                          file.write('\n'))) {
                        throw std::runtime_error(
                            "In ofstream::write: Error writing to "
                            "playlist file.");
                    }
                }
                if (USE_MPD) {
                    _run_mpd_();
                }
            } else {
                std::fprintf(stderr,
                             "Problem opening file %s!\nAborting!\n",
                             _playlist_file_.c_str());
                std::exit(EXIT_FAILURE);
            }
        } else if (USE_MPD) {
            std::printf("Will clear the mpd playlist, update the mpd "
                        "database, and load %s.\n",
                        boost::filesystem::path(_playlist_file_)
                            .filename()
                            .replace_extension()
                            .c_str());
        }
    }
}

void Consumer::_apply_tags_(const boost::filesystem::path& path,
                            const std::string& artist,
                            const std::string& title) {
    bool changed = false;

    TagLib::FileRef file(path.c_str());
    TagLib::Tag* tags = file.tag();

    if (!tags) {
        std::fprintf(stderr, "Music file `%s` has no tags!\n",
                     path.c_str());
        return;
    }

    if (tags->artist() != artist) {
        if (NONO) {
            std::printf("  Artist: `%s` -> `%s`\n",
                        tags->artist().toCString(), artist.c_str());
        } else {
            changed = true;
            tags->setArtist(artist);
        }
    }

    if (tags->title() != title) {
        if (NONO) {
            std::printf("  Title: `%s` -> `%s`\n",
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
