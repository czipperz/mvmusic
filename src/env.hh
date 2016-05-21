/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HEADER_GUARD_ENV_H
#define HEADER_GUARD_ENV_H

/**
 * @file env.hh
 * @brief Defines @c get_home_dir()
 */

#include <boost/filesystem.hpp>

/**
 * @brief Returns the path of the home directory on this platform.
 */
boost::filesystem::path get_home_dir();

#endif
