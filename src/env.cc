/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "env.hh"

using namespace boost::filesystem;
path get_home_dir() {
    const char* env = getenv("HOME");
    if (!env) {
        env = ".";
    }
    return env;
}
