/**
    @file include/string_funcs.h
    @author Patrik Bogren (pabo1800)
    @date June 2019
    @version: 0.1
    @brief Useful string operations
*/
#ifndef INCLUDE_STRING_FUNCS_H
#define INCLUDE_STRING_FUNCS_H

#include <string>

namespace pabo {

void replaceCharWithSpace(std::string&, char);
void trimWhitespace(std::string& s);
void rightTrim(std::string& s);
void leftTrim(std::string& s);

}  // namespace pabo

#endif
