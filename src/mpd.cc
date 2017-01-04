#include <mpd/client.h>
#include <memory>
#include "consumer.hh"

namespace {
class connection : public std::unique_ptr<mpd_connection,
                                          void (*)(mpd_connection*)> {
    template <class Fun, class Con, class... Args>
    static auto wrap_error(Fun f, Con con, Args&&... args)
        -> decltype(f(con, std::forward<Args>(args)...)) {
        auto x = f(con, std::forward<Args>(args)...);
        if (!x) {
            throw std::runtime_error(
                mpd_connection_get_error_message(con));
        }
        return x;
    }

    using super = std::unique_ptr<mpd_connection,
                                  void (*)(mpd_connection*)>;

public:
    connection(const char* name, unsigned port, unsigned delay)
        : super(mpd_connection_new(name, port, delay),
                mpd_connection_free) {}

    void run_clear() {
        wrap_error(mpd_run_clear, get());
    }

    void run_update(const char* path = nullptr) {
        wrap_error(mpd_run_update, get(), path);
    }

    bool run_add(const char* fname) {
        return mpd_run_add(get(), fname);
    }
};
}

void Consumer::_run_mpd_() {
    connection con("localhost", 6600, 1000);
    con.run_clear();
    con.run_update();
    for (const auto& s : _playlist_lines_) {
        if (!con.run_add(s.c_str())) {
            std::fprintf(stderr, "ERROR adding file: %s\n", s.c_str());
        }
    }
}
