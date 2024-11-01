
#pragma once
#ifndef COVBR_2_HTML_H
#define COVBR_2_HTML_H

#include <BaseTypes.h>
#include <string>
#include <regex>

//  covbr to html converter
class Covbr2Html
{
public:
    //  the conversion
    //  covbrTxt: source file name
    //  wb: write back cleaned sources
    static bool convert(const std::string& covbrTxt, bool wb = false);

private:
    static const CONST_C_STRING cTtl;
    static const CONST_C_STRING cHead;
    static const CONST_C_STRING cTail;

    //  read file into string
    static bool read(std::string& trg, const std::string& txtFile);

    //  Python regex style replacement
    template <typename T>
    inline static auto repl(const std::regex& re, CONST_C_STRING rep, const T& src)
    {
        return std::regex_replace(src, re, rep);
    }

    Covbr2Html() = delete;
    Covbr2Html(const Covbr2Html&) = delete;
    Covbr2Html(Covbr2Html&&) = delete;
    Covbr2Html& operator = (const Covbr2Html&) = delete;
};


#endif // _H
