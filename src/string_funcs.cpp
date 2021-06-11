#include "string_funcs.h"
#include <algorithm>
#include <string>
#include <cctype>  // isspace

namespace pabo {

void replaceCharWithSpace(std::string& s, char ch)
{
    auto pos = s.find(ch);
    if (pos != std::string::npos) {
        s[pos] = ' ';
    }
}

void trimWhitespace(std::string& s)
{
    leftTrim(s);
    rightTrim(s);
}

// Courtesy of:
// https://stackoverflow.com/a/217605

void leftTrim(std::string& s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](char c) {
                return !std::isspace(c);
            }));
}

void rightTrim(std::string& s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
                return !std::isspace(ch);
            }).base(),
            s.end());
}

}  // namespace pabo
