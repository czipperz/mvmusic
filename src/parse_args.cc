/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <boost/filesystem.hpp>
#include <string.h>
#include <stdio.h>

#include "conf.hh"
#include "env.hh"
#include "parse_args.hh"
#include "globals.hh"
#include "usage.hh"

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
        } else if (strcmp(str, "-h") == 0 ||
                   strcmp(str, "--help") == 0) {
            usage(argv[-1]);
            exit(EXIT_SUCCESS);
        } else if (strcmp(str, "-n") == 0 ||
                   strcmp(str, "--nono") == 0) {
            NONO = true;
        } else if (strcmp(str, "-m") == 0 ||
                   strcmp(str, "--mpc") == 0) {
            USE_MPC = true;
        } else if (strcmp(str, "-p") == 0 ||
                   strcmp(str, "--playlist") == 0) {
            if (argi + 1 == argc) {
                fprintf(stderr, "Playlist flag `%s` detected without "
                                "appropriate playlist.\n\n",
                        str);
                exit(EXIT_FAILURE);
            } else if (set_playlist) {
                fprintf(stderr, "Already set playlist to `%s`, can't "
                                "set it again to `%s`.\n\n",
                        playlist.c_str(), str);
                exit(EXIT_FAILURE);
            } else {
                ++argi;
                playlist = argv[argi];
                set_playlist = true;

                // if not .m3u file
                if (playlist.size() < 4) {
                not_m3u_file:
                    fprintf(stderr,
                            "Warning: Playlist specified `%s` is not "
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
        } else if (str[0] == '-') {
            fprintf(stderr, "Unrecognized option `%s`.\n", str);
            fprintf(stderr, "Use `--` as a unique argument before "
                            "this argument to make it parse as\n"
                            "  a file/directory.\n\n");
            usage(argv[-1]);
            exit(EXIT_FAILURE);
        } else {
            read(files, str);
            read_manual = true;
        }
    }

    if (!set_playlist) {
        playlist = get_default_playlist().string();
    }
    if (argc == 0 || !read_manual) {
        read_directory(files, get_home_dir() / "wnlds");
    }
}
