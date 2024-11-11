
#pragma once
#ifndef COVBR_2_HTML_H
#define COVBR_2_HTML_H

#include <SOM/BaseTypes.h>
#include <SOM/coding.h>

#include <string>

//  covbr to html converter
class Covbr2Html
{
public:
    //  the conversion
    //  covbrTxt: source file name
    //  wb: write back cleaned sources
    //  hc: highlight covered parts
    //  fc: keep fully covered sources listed
    static bool convert(
        const std::string& covbrTxt,
        const std::string& odir,
        bool wb = false, bool hc = false, bool fc = false);

private:
    static const CONST_C_STRING cTtl;
    static const CONST_C_STRING cHead;
    static const CONST_C_STRING cTail;

    NODEF(Covbr2Html)
    NOCOPY(Covbr2Html)
};

#endif // _H
