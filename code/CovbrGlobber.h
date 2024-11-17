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

    inline ~CovbrGlobber()
    {
        join();
    }

    //  I_GlobProcessor
    void process(CONST_C_STRING item) override;

    //  set write back input files
    inline void setWb(bool wb = true)
    {
        mWb = wb;
    }
    //  set highlight covered items
    inline void setHc(bool hc = true)
    {
        mHc = hc;
    }
    //  set keep fully covered sources listed
    inline void setFc(bool fc = true)
    {
        mFc = fc;
    }
    //  set output directory
    bool setOdir(const CONST_C_STRING odir);

    //  wait for all threads to finish
    //  return: number of failures
    //  -> end of main return
    INT32 ret();
private:
    std::vector<std::thread> mThreads;
    std::atomic<INT32> mRet = 0;
    bool mWb = false;
    bool mHc = false;
    bool mFc = false;
    std::string mOdir;
    bool mOk = true;
    void threadFunc(const std::string&& file);
    void join();
    NOCOPY(CovbrGlobber)
};

#endif // _H
