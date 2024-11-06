#include <CovbrGlobber.h>
#include <Covbr2Html.h>
#include <SOM/TraceMacros.h>
#include <iostream>

void CovbrGlobber::process(const CONST_C_STRING item)
{
    mThreads.push_back(std::thread(&CovbrGlobber::threadFunc, this, std::string(item)));
}

INT32 CovbrGlobber::ret()
{
    TRACE_FUNC_TIME()
    for (auto& th : mThreads) th.join();
    mThreads.clear();
    if (_ret != 0) std::cerr << _ret << " errors\n";
    return _ret;
}

void CovbrGlobber::threadFunc(const std::string&& file)
{
    TRACE_FUNC_TIME()
    if (not Covbr2Html::convert(file, mWb, mHc)) ++_ret;
}

