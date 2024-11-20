#pragma once
#ifndef COVBR_2_HTML_FGLOB_H
#define COVBR_2_HTML_FGLOB_H

#include <Covbr2Html.h>
#include <SOM/coding.h>
#include <SOM/fglob.h>

#include <atomic>
#include <string>
#include <thread>
#include <vector>

//  runs conversion of files in separate threads
class Covbr2HtmlFglob :
    public Covbr2Html,
    public I_GlobProcessor
{
public:
    //  I_GlobProcessor
    void process(CONST_C_STRING item) override;

    //  wait for all threads to finish
    //  return: number of failures
    //  -> end of main return
    int ret();

    Covbr2HtmlFglob() = default;

    inline ~Covbr2HtmlFglob() { join(); }

private:
    std::vector<std::thread> mThreads;
    std::atomic<int> mRet = 0;
    void threadFunc(const std::string&& file);
    void join();
    NOCOPY(Covbr2HtmlFglob)
};

#endif // _H
