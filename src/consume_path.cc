/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <algorithm>
#include <stdio.h>
#include <ctype.h>

#include "conf.hh"
#include "globals.hh"
#include "consumer.hh"
#include "env.hh"

using namespace std;
using namespace boost::filesystem;

int
find_feat(string::iterator bstring, string::iterator estring) {
    // bounds check for ft./Ft.
    if (bstring + 2 >= estring) {
        return 0;
    }
    if (bstring[0] == 'f' || bstring[0] == 'F') {
        if (bstring[1] == 't' && bstring[2] == '.') {
            return 3;
        }

        // bounds check for feat./Feat.
        if (bstring + 4 >= estring) {
            return 0;
        }
        if (bstring[1] == 'e' && bstring[2] == 'a' &&
            bstring[3] == 't' && bstring[4] == '.') {
            return 5;
        }
    }
    return 0;
}

string fix_feat_delim(const string::iterator bstring,
                      const string::iterator estring,
                      const char odelim, const char cdelim) {
    auto istring = find(bstring, estring, odelim);
    if (istring == estring || /* !!! */ ++istring == estring) {
        return {bstring, estring};
    }

    // istring is now after the [
    if (auto feat_offset = find_feat(istring, estring)) {
        auto ebracket = find(istring, estring, cdelim);
        if (ebracket == estring) {
            return {};
        }
        string output{bstring, istring - 1};
        output += '(';
        output.append("feat.");
        output.append(istring + feat_offset, ebracket);
        output += ')';
        if (ebracket + 1 != estring) {
            output.append(ebracket + 1, estring);
        }
        return output;
    }
    return {bstring, estring};
}

string
fix_feat_nodelim(string::iterator bstring, string::iterator estring) {
    auto istring = bstring;
    while (istring != estring) {
        istring = find_if(istring, estring, [](char ch) {
            return ch == 'f' || ch == 'F';
        });
        if (istring - 1 != bstring && istring[-1] == '(') {
            ++istring;
            continue;
        }
        if (istring == estring) {
            break;
        }
        if (auto feat_offset = find_feat(istring, estring)) {
            string output{bstring, istring};
            output += "(feat.";

            // if has (for example) a `(SXX Remix)`
            auto efeat = find(istring + feat_offset, estring, '(');
            while (isspace(*--efeat)) {
            }
            ++efeat;

            output.append(istring + feat_offset, efeat);
            output += ')';
            if (efeat != estring) {
                output.append(efeat, estring);
            }
            return output;
        }
        ++istring;
    }
    return {bstring, estring};
}

/// Change featuring info to the way I want:
/// Words:
/// `ft.`, `Ft.`, `Feat.` --> feat.
/// Delimiter:
/// None, `[` then `]` --> `(` then `)`
static string
fix_feat(string::iterator istring, string::iterator estring) {
    auto fbrackets = fix_feat_delim(istring, estring, '[', ']');
    auto fparens =
        fix_feat_delim(fbrackets.begin(), fbrackets.end(), '(', ')');
    auto fnone = fix_feat_nodelim(fparens.begin(), fparens.end());
    return fnone;
}

/// Remove `\[.*?\] ` and an optional `- ` from the beginning.
void remove_genre(string::iterator& istring,
                  const string::iterator& estring) {
    if (*istring == '[') {
        do {
            istring = find(istring, estring, ']');
            if (istring == estring) {
                fprintf(stderr,
                        "Unmatched `[`.  Skipping removing genre.\n");
                break;
            }
            if (istring[1] == ' ') {
                istring += 2;
                if (istring[0] == '-' && istring[1] == ' ') {
                    istring += 2;
                }
            }
        } while (0);
    }
}

/// Remove `\s*\[.*?\]` from end if it exists.
static void remove_trailing_tags(const string& no_slashes,
                                 string::iterator& istring,
                                 string::iterator& estring) {
    do {
        auto rbstring = no_slashes.rbegin();
        auto ristring = rbstring;
        auto restring = rbstring + (estring - istring);

        if (ristring != restring && *ristring == ']') {
            ristring = find(ristring, restring, '[');
            if (ristring == restring) {
                fprintf(stderr, "Unmatched `]`.  Skipping remove "
                                "trailing tags.\n");
                break;
            }

            // check for feat, skip if is
            auto iter = ristring;
            --iter;
            if (*iter == 'f' || *iter == 'F') {
                --iter;
                if (*iter == 't') {
                    --iter;
                    if (*iter == '.') {
                        break;
                    }
                } else if (*iter == 'e') {
                    --iter;
                    if (*iter == 'a') {
                        --iter;
                        if (*iter == 't') {
                            --iter;
                            if (*iter == '.') {
                                break;
                            }
                        }
                    }
                }
            }

            ++ristring;
            if (ristring == restring) {
                fprintf(stderr, "Trailing tags are entire filename.  "
                                "Skipping remove trailing tags.\n");
                break;
            }

            // skip whitespace
            ristring =
                find_if(ristring, restring,
                        [](char ch) -> bool { return !isspace(ch); });
            if (ristring == restring) {
                fprintf(stderr, "Trailing tags are entire filename.  "
                                "Skipping remove trailing tags.\n");
                break;
            }

            // commit ristring to estring
            estring = estring - (ristring - rbstring);
        }
    } while (0);
}

/// Split `fname` into `artist\s+-\s*name`
static void
parse_file_name(const string& fname, string& artist, string& name) {
    const auto bfname = fname.begin();
    const auto efname = fname.end();

    auto dash_loc = bfname;
restart:
    dash_loc = find(dash_loc, efname, '-');

    /* artist */ {
        auto eartist = dash_loc;
        bool went_through_space = false;
        // backtrack through whitespace
        while (isspace(*--eartist)) {
            went_through_space = true;
        }
        ++eartist;
        // handle the case where name has a -
        if (!went_through_space) {
            ++dash_loc;
            goto restart;
        }
        artist.assign(bfname, eartist);
    }

    /* name */ {
        auto bname = dash_loc;
        // go forward through whitespace
        while (isspace(*++bname)) {
        }
        name.assign(bname, efname);
    }
}

void Consumer::consume(const path& p) {
    // dropped .mp3 here
    auto no_slashes = p.filename().replace_extension().string();

    auto bstring = no_slashes.begin();
    auto estring = no_slashes.end();
    auto istring = bstring;

    remove_genre(istring, estring);
    remove_trailing_tags(no_slashes, istring, estring);
    auto new_fname = fix_feat(istring, estring);

    auto new_path = get_songs_dir() / (new_fname + ".mp3");

    _sorted_insert_song_(new_path.filename().string());
    if (p != new_path) {
        if (NONO) {
            printf("mv \"%s\" \"%s\"\n", p.c_str(), new_path.c_str());
        } else {
            rename(p, new_path);
        }
    }

    string artist, name;
    parse_file_name(new_fname, artist, name);

    if (NONO) {
        _apply_tags_(p, artist, name);
    } else {
        _apply_tags_(new_path, artist, name);
    }
}
