#include <CovbrGlobber.h>
#include <Covbr2Html.h>
#include <SOM/TraceMacros.h>
#include <iostream>
#include <filesystem>

void CovbrGlobber::process(const CONST_C_STRING item)
{
    if (mOk) mThreads.push_back(std::thread(&CovbrGlobber::threadFunc, this, std::string(item)));
}

bool CovbrGlobber::setOdir(const CONST_C_STRING odir)
{
    mOk = true;
    mOdir.clear();
    try
    {
        std::filesystem::create_directories(odir);
        mOdir = odir;
    }
    catch(const std::filesystem::filesystem_error&)
    {
        mOk = false;
        mRet = 1;
        std::cerr << "cannot create directory " << odir << '\n';
    }
    return mOk;
}

INT32 CovbrGlobber::ret()
{
    join();
    if (mRet != 0) std::cerr << mRet << " errors\n";
    return mRet;
}

void CovbrGlobber::threadFunc(const std::string&& file)
{
    TRACE_FUNC_TIME()
    if (not Covbr2Html::convert(file, mOdir, mWb, mHc)) ++mRet;
}

void CovbrGlobber::join()
{
    TRACE_FUNC_TIME()
    for (auto& th : mThreads) th.join();
    mThreads.clear();
}
