#include "catch.hpp"
#include "../src/consume_path.hh"

using namespace std;
TEST_CASE("find_feat") {
    int find_feat(string::iterator bstring, string::iterator estring);

    string s = "feat";
    REQUIRE(find_feat(s.begin(), s.end()) == 0);
    s += '.';
    REQUIRE(find_feat(s.begin(), s.end()) == 5);
    s[0] = 'F';
    REQUIRE(find_feat(s.begin(), s.end()) == 5);
    s = "ft.";
    REQUIRE(find_feat(s.begin(), s.end()) == 3);
    s[0] = 'F';
    REQUIRE(find_feat(s.begin(), s.end()) == 3);
}

TEST_CASE("fix_feat_delim should do nothing") {
    string fix_feat_delim(const string::iterator bstring,
                          const string::iterator estring,
                          const char odelim, const char cdelim);

    string s = "A - S [ft F]";
    REQUIRE(fix_feat_delim(s.begin(), s.end(), '[', ']') ==
            "A - S [ft F]");
}

TEST_CASE("fix_feat_delim should transform []") {
    string fix_feat_delim(const string::iterator bstring,
                          const string::iterator estring,
                          const char odelim, const char cdelim);

    string f;
    SECTION("ft.") {
        f = "ft.";
    }
    SECTION("Ft.") {
        f = "Ft.";
    }
    SECTION("feat.") {
        f = "feat.";
    }
    SECTION("Feat.") {
        f = "Feat.";
    }
    string s = "A - S [" + f + " F]";
    REQUIRE(fix_feat_delim(s.begin(), s.end(), '[', ']') ==
            "A - S (feat. F)");
}

TEST_CASE("fix_feat_nodelim should transform") {
    string fix_feat_nodelim(const string::iterator bstring,
                            const string::iterator estring);

    string f;
    SECTION("ft.") {
        f = "ft.";
    }
    SECTION("Ft.") {
        f = "Ft.";
    }
    SECTION("feat.") {
        f = "feat.";
    }
    SECTION("Feat.") {
        f = "Feat.";
    }
    string s = "A - S " + f + " F";
    REQUIRE(fix_feat_nodelim(s.begin(), s.end()) ==
            "A - S (feat. F)");
}

TEST_CASE("fix_feat_nodelim should do nothing") {
    string fix_feat_nodelim(const string::iterator bstring,
                            const string::iterator estring);

    string f;
    SECTION("ft") {
        f = "ft";
    }
    SECTION("feat") {
        f = "feat";
    }
    SECTION("Ft") {
        f = "Ft";
    }
    SECTION("Feat") {
        f = "Feat";
    }
    SECTION("Rand") {
        f = "Rand";
    }
    SECTION("rand") {
        f = "rand";
    }
    string s = "A - S " + f + " F";
    REQUIRE(fix_feat_nodelim(s.begin(), s.end()) == s);
}

TEST_CASE("remove_genre should transform") {
    void remove_genre(string::iterator& istring,
                      const string::iterator& estring);

    string g;
    SECTION("[DFSJKL:]") {
        g = "[DFSJKL:]";
    }
    SECTION("[SDFJK] -") {
        g = "[SDFJK] -";
    }
    string s = g + " random stuff here";
    string::iterator i = s.begin();
    remove_genre(i, s.end());
    REQUIRE(i - s.begin() == g.size() + 1);
}
