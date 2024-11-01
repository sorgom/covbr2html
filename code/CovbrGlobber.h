#pragma once
#ifndef COVBR_GLOBBBER_H
#define COVBR_GLOBBBER_H

#include <glob.h>

#include <atomic>
#include <thread>
#include <vector>

//  runs conversion of files in separate threads
class CovbrGlobber : public I_GlobProcessor
{
public:
    CovbrGlobber() = default;

    //  I_GlobProcessor
    void process(const CONST_C_STRING item) override;

    //  set write back input files
    void setWb(bool wb = true)
    {
        mWb = wb;
    }

    //  number of failed conversions
    INT32 ret();
private:
    std::vector<std::thread> mThreads;
    std::atomic<INT32> _ret = 0;
    bool mWb = false;
    void treadFunc(const std::string&& file);
};

#endif // _H
