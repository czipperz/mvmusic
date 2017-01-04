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

static void read_file(std::vector<boost::filesystem::path>& files,
                      boost::filesystem::path file) {
    if (file.extension() == ".mp3") {
        files.push_back(file);
    }
}

static void
read_directory(std::vector<boost::filesystem::path>& files,
               boost::filesystem::path dir) {
    for (boost::filesystem::directory_iterator it(dir), end;
         it != end; ++it) {
        read_file(files, it->path());
    }
}

static void read(std::vector<boost::filesystem::path>& files,
                 boost::filesystem::path path) {
    if (is_directory(path)) {
        read_directory(files, path);
    } else if (is_regular_file(path)) {
        read_file(files, path);
    }
}

void parse_args(int argc, char** argv,
                std::vector<boost::filesystem::path>& files,
                std::string& playlist) {
    bool read_manual = false;
    bool set_playlist = false;

    for (int argi = 0; argi != argc; ++argi) {
        char* str = argv[argi];
        if (std::strcmp(str, "--") == 0) {
            for (; argi != argc; ++argi) {
                read(files, str);
                read_manual = true;
            }
            break;
        } else if (std::strcmp(str, "-h") == 0 ||
                   std::strcmp(str, "--help") == 0) {
            usage(argv[-1]);
            std::exit(EXIT_SUCCESS);
        } else if (std::strcmp(str, "-n") == 0 ||
                   std::strcmp(str, "--nono") == 0) {
            NONO = true;
        } else if (std::strcmp(str, "-m") == 0 ||
                   std::strcmp(str, "--mpd") == 0) {
            // default
            USE_MPD = true;
        } else if (std::strcmp(str, "-M") == 0 ||
                   std::strcmp(str, "--no-mpd") == 0) {
            USE_MPD = false;
        } else if (std::strcmp(str, "-p") == 0 ||
                   std::strcmp(str, "--playlist") == 0) {
            if (argi + 1 == argc) {
                std::fprintf(stderr, "Playlist flag `%s` detected without "
                                "appropriate playlist.\n\n",
                        str);
                std::exit(EXIT_FAILURE);
            } else if (set_playlist) {
                std::fprintf(stderr, "Already set playlist to `%s`, can't "
                                "set it again to `%s`.\n\n",
                        playlist.c_str(), str);
                std::exit(EXIT_FAILURE);
            } else {
                ++argi;
                playlist = argv[argi];
                set_playlist = true;

                // if not .m3u file
                if (playlist.size() < 4) {
                not_m3u_file:
                    std::fprintf(stderr,
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
            std::fprintf(stderr, "Unrecognized option `%s`.\n", str);
            std::fprintf(stderr,
                         "Use `--` as a unique argument before this "
                         "argument to make it parse as\n\
  a file/directory.\n\n");
            usage(argv[-1]);
            std::exit(EXIT_FAILURE);
        } else {
            read(files, str);
            read_manual = true;
        }
    }

    if (!set_playlist) {
        playlist = get_default_playlist().string();
    }
    if (argc == 0 || !read_manual) {
        read_directory(files, get_downloads_dir());
    }
}
