
#pragma once
#ifndef COVBR_2_HTML_H
#define COVBR_2_HTML_H

#include <SOM/coding.h>

#include <sstream>
#include <string>
#include <vector>

class Covbr2Html
{
public:
    // set output directory other than source directory
    inline void setOdir(const std::string& odir) { mOdir = odir; }
    // highlight covered items
    inline void setHc(const bool hc = true) { mHc = hc; }
    // keep fully covered sources listed
    inline void setFc(const bool fc = true) { mFc = fc; }

    // convert covbr text file to html
    // with given options
    // return false on read / write error
    bool convert(const std::string& fpath);

    inline Covbr2Html() = default;
private:
    static const std::string cTtl;
    static const std::string cHead;
    static const std::string cTail;

    std::ostringstream mOs;
    bool mHc = false;
    bool mFc = false;
    std::string mOdir;
    std::vector<std::string> mFiles;
    void files2os();
    void fc2os();

    NOCOPY(Covbr2Html)
};

#endif // _H
