
#pragma once
#ifndef COVBR2HTML_H
#define COVBR2HTML_H

#include <BaseTypes.h>
#include <RegexBuffer.h>

#include <string>
#include <regex>

class Covbr2Html
{
public:
    inline Covbr2Html() = default;
    bool convert(CONST_C_STRING covbrTxt);

private:
    static const CONST_C_STRING cTtl;
    static const CONST_C_STRING cHead;
    static const CONST_C_STRING cTail;

    RegexBuffer mRb;

    //  read file into string
    static bool read(std::string& trg, CONST_C_STRING txtFile);

    //  Python regex style replacement
    template <typename T>
    inline static auto repl(const std::regex& re, CONST_C_STRING rep, const T& src)
    {
        return std::regex_replace(src, re, rep);
    }

    Covbr2Html(const Covbr2Html&) = delete;
    Covbr2Html(Covbr2Html&&) = delete;
    Covbr2Html& operator = (const Covbr2Html&) = delete;
};


#endif // _H
