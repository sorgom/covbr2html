
#pragma once
#ifndef COVBR2HTML_H
#define COVBR2HTML_H

#include <BaseTypes.h>
#include <string>
#include <regex>

class Covbr2Html
{
public:
    static bool convert(CONST_C_STRING covbrFile);

    Covbr2Html() = delete;
    Covbr2Html(const Covbr2Html&) = delete;
    Covbr2Html(Covbr2Html&&) = delete;
    Covbr2Html& operator = (const Covbr2Html&) = delete;

private:
    static bool read(std::string& trg, CONST_C_STRING txtFile);

    inline static std::string repl(const std::regex& re, CONST_C_STRING rep, const std::string& src)
    {
        return std::regex_replace(src, re, rep);
    }
};


#endif // _H
