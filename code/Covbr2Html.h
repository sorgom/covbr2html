
#pragma once
#ifndef COVBR_2_HTML_H
#define COVBR_2_HTML_H

#include <SOM/BaseTypes.h>
#include <SOM/coding.h>

#include <fstream>
#include <string>
#include <vector>

class Covbr2Html
{
public:
    // set output directory other than source directory
    bool setOdir(const std::string& odir);
    // highlight covered items
    inline void setHc(const bool hc = true) { mHc = hc; }
    // keep fully covered sources listed
    inline void setFc(const bool fc = true) { mFc = fc; }

    // convert covbr text file to html
    // with given options
    // return false on read / write error
    bool convert(const std::string& fpath);

    inline Covbr2Html() = default;

protected:
    inline bool ok() const { return mOk; }

private:
    bool mHc = false;
    bool mFc = false;
    std::string mOdir;
    bool mOk = true;
    void fc2os(
        std::ofstream& os,
        const std::vector<std::string>& files
    );

    NOCOPY(Covbr2Html)
};

#endif // _H
