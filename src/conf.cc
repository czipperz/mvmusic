/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <boost/filesystem.hpp>
#include "env.hh"

boost::filesystem::path get_downloads_dir() {
    return get_home_dir() / "wnlds";
}
boost::filesystem::path get_default_playlist() {
    return get_home_dir() / "music" / "p" / "Electronic.m3u";
}
boost::filesystem::path get_songs_dir() {
    return get_home_dir() / "music" / "s";
}
