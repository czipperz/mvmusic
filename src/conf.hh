/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HEADER_GUARD_CONF_H
#define HEADER_GUARD_CONF_H

#include <boost/filesystem.hpp>

boost::filesystem::path get_downloads_dir();
boost::filesystem::path get_default_playlist();
boost::filesystem::path get_songs_dir();

#endif
