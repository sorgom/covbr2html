#pragma once
#ifndef COVBR_2_HTML_GLOB_H
#define COVBR_2_HTML_GLOB_H

#include <Covbr2Html.h>
#include <SOM/coding.h>
#include <SOM/Glob.h>

#include <atomic>
#include <string>
#include <thread>
#include <vector>

//  runs conversion of files in separate threads
class Covbr2HtmlGlob :
    public Covbr2Html,
    public I_GlobProcessor
{
public:
    //  I_FglobProcessor
    void process(const std::string& item) override;

    //  wait for all threads to finish
    //  return: number of failures
    //  -> end of main return
    int ret();

    Covbr2HtmlGlob() = default;

    inline ~Covbr2HtmlGlob() { join(); }

private:
    std::vector<std::thread> mThreads;
    std::atomic<int> mRet = 0;
    void threadFunc(const std::string&& file);
    void join();
    NOCOPY(Covbr2HtmlGlob)
};

#endif // _H
