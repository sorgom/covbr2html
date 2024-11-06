#pragma once
#ifndef COVBR_GLOBBBER_H
#define COVBR_GLOBBBER_H

#include <SOM/coding.h>
#include <SOM/fglob.h>

#include <atomic>
#include <string>
#include <thread>
#include <vector>

//  runs conversion of files in separate threads
class CovbrGlobber : public I_GlobProcessor
{
public:
    CovbrGlobber() = default;

    //  I_GlobProcessor
    void process(CONST_C_STRING item) override;

    //  set write back input files
    void setWb(bool wb = true)
    {
        mWb = wb;
    }
    void setHc(bool hc = true)
    {
        mHc = hc;
    }

    //  wait for all threads to finish
    //  return: number of failed conversions
    //  -> end of main return
    INT32 ret();
private:
    std::vector<std::thread> mThreads;
    std::atomic<INT32> _ret = 0;
    bool mWb = false;
    bool mHc = false;
    void threadFunc(const std::string&& file);

    NOCOPY(CovbrGlobber)
};

#endif // _H

