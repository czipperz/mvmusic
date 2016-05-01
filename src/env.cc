#include "env.hh"

using namespace boost::filesystem;
path get_home_dir() {
    const char* env = getenv("HOME");
    if (!env) {
        env = ".";
    }
    return env;
}
