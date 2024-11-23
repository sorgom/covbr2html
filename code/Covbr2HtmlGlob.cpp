#include <Covbr2HtmlGlob.h>
#include <SOM/TraceMacros.h>
#include <iostream>
#include <filesystem>

void Covbr2HtmlGlob::process(const std::string& item)
{
    if (ok()) mThreads.push_back(std::thread(&Covbr2HtmlGlob::threadFunc, this, std::string(item)));
}

INT32 Covbr2HtmlGlob::ret()
{
    join();
    if (not ok()) mRet += 1;
    if (mRet != 0) std::cerr << mRet << " errors\n";
    return mRet;
}

void Covbr2HtmlGlob::threadFunc(const std::string&& file)
{
    TRACE_FUNC_TIME()
    if (not convert(file)) ++mRet;
}

void Covbr2HtmlGlob::join()
{
    TRACE_FUNC_TIME()
    for (auto& th : mThreads) th.join();
    mThreads.clear();
}
