#include <boost/filesystem.hpp>
#include <string.h>
#include <stdio.h>

#include "env.hh"
#include "parse_args.hh"
#include "globals.hh"

using namespace std;
using namespace boost::filesystem;

static void read_file(vector<path>& files, path file) {
    if (file.extension() == ".mp3") {
        files.push_back(file);
    }
}

static void read_directory(vector<path>& files, path dir) {
    for (directory_iterator it(dir), end; it != end; ++it) {
        read_file(files, it->path());
    }
}

static void read(vector<path>& files, path path) {
    if (is_directory(path)) {
        read_directory(files, path);
    } else if (is_regular_file(path)) {
        read_file(files, path);
    }
}

void parse_args(int argc, char** argv, vector<path>& files,
                string& playlist) {
    bool read_manual = false;
    bool set_playlist = false;

    for (int argi = 0; argi != argc; ++argi) {
        char* str = argv[argi];
        if (strcmp(str, "--") == 0) {
            for (; argi != argc; ++argi) {
                read(files, str);
                read_manual = true;
            }
            break;
        } else if (strcmp(str, "-n") == 0 ||
                   strcmp(str, "--nono") == 0) {
            NONO = true;
        } else if (strcmp(str, "-p") == 0 ||
                   strcmp(str, "--playlist") == 0) {
            if (argi + 1 == argc) {
                fprintf(stderr, "Playlist flag `%s` detected without "
                                "appropriate playlist.\n",
                        str);
                exit(EXIT_FAILURE);
            } else if (set_playlist) {
                fprintf(stderr, "Already set playlist to `%s`, can't "
                                "set it again to `%s`\n",
                        playlist.c_str(), str);
                exit(EXIT_FAILURE);
            } else {
                playlist = argv[argi + 1];
                set_playlist = true;

                // if not .m3u file
                if (playlist.size() < 4) {
                not_m3u_file:
                    fprintf(stderr, "Playlist specified `%s` is not "
                                    "a .m3u file.\n",
                            playlist.c_str());
                } else {
                    auto rb = playlist.rbegin();
                    if (rb[3] != '.' || rb[2] != 'm' ||
                        rb[1] != '3' || rb[0] != 'u') {
                        goto not_m3u_file;
                    }
                }
            }
        } else {
            read(files, str);
            read_manual = true;
        }
    }

    if (!set_playlist) {
        playlist = (get_home_dir() / "music" / "p" / "Electronic.m3u")
                       .string();
    }
    if (argc == 0 || !read_manual) {
        read_directory(files, get_home_dir() / "wnlds");
    }
}
