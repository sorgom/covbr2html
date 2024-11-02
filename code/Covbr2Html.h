
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

    Covbr2Html() = delete;
    Covbr2Html(const Covbr2Html&) = delete;
    Covbr2Html(Covbr2Html&&) = delete;
    Covbr2Html& operator = (const Covbr2Html&) = delete;
};


#endif // _H
