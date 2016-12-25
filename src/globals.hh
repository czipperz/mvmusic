/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HEADER_GUARD_GLOBALS_H
#define HEADER_GUARD_GLOBALS_H

/**
 * @file globals.hh
 * @brief Defines @c NONO and @c USE_MPD.
 */

/**
 * @brief Controls whether the program will write to a file or print
 * out what it would do.
 */
extern bool NONO;

/**
 * @brief Controls whether the playlist should be reloaded in MPD.
 */
extern bool USE_MPD;

#endif
