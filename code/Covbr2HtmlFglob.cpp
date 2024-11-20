#include <Covbr2HtmlFglob.h>
#include <SOM/TraceMacros.h>
#include <iostream>
#include <filesystem>

void Covbr2HtmlFglob::process(const CONST_C_STRING item)
{
    if (ok()) mThreads.push_back(std::thread(&Covbr2HtmlFglob::threadFunc, this, std::string(item)));
}

INT32 Covbr2HtmlFglob::ret()
{
    join();
    if (not ok()) mRet += 1;
    if (mRet != 0) std::cerr << mRet << " errors\n";
    return mRet;
}

void Covbr2HtmlFglob::threadFunc(const std::string&& file)
{
    TRACE_FUNC_TIME()
    if (not convert(file)) ++mRet;
}

void Covbr2HtmlFglob::join()
{
    TRACE_FUNC_TIME()
    for (auto& th : mThreads) th.join();
    mThreads.clear();
}
