#include <CovbrGlobber.h>
#include <Covbr2Html.h>
#include <trace.h>
#include <iostream>

void CovbrGlobber::process(const CONST_C_STRING item)
{
    TRACE_FUNC()
    mThreads.push_back(std::thread(&CovbrGlobber::treadFunc, this, std::string(item)));
}

INT32 CovbrGlobber::ret() 
{   
    TRACE_FUNC()
    for (auto& th : mThreads)
    {
        th.join();
    }
    mThreads.clear();
    if (_ret != 0)
    {
        std::cerr << _ret << " errors converting files\n";
    }
    return _ret; 
}

void CovbrGlobber::treadFunc(const std::string&& file)
{
    TRACE_FUNC()
    TRACE_VAR(file)
    if (not Covbr2Html::convert(file, mWb))
    {
        ++_ret;
    }
}
