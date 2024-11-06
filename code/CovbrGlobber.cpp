#include <CovbrGlobber.h>
#include <Covbr2Html.h>
#include <SOM/TraceMacros.h>
#include <iostream>
#include <iso646.h>

void CovbrGlobber::process(const CONST_C_STRING item)
{
    TRACE_FUNC_TIME()
    mThreads.push_back(std::thread(&CovbrGlobber::treadFunc, this, std::string(item)));
}

INT32 CovbrGlobber::ret()
{
    TRACE_FUNC_TIME()
    for (auto& th : mThreads)
    {
        th.join();
    }
    mThreads.clear();
    if (_ret != 0)
    {
        std::cerr << _ret << " errors\n";
    }
    return _ret;
}

void CovbrGlobber::treadFunc(const std::string&& file)
{
    TRACE_FUNC_TIME()
    TRACE_VAR(file)
    if (not Covbr2Html::convert(file, mWb, mHc))
    {
        ++_ret;
    }
}

