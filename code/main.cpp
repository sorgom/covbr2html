#include <Covbr2Html.h>
#include <glob.h>

#include <atomic>
#include <filesystem>
#include <iostream>
#include <thread>
#include <vector>

#define TRACE_ME
#include <trace.h>

class CovbrGlobber : public I_GlobProcessor
{
public:
    CovbrGlobber() = default;

    void process(const CONST_C_STRING item) override
    {
        TRACE_FUNC()
        mThreads.push_back(std::thread(&CovbrGlobber::treadFunc, this, std::string(item)));
    }
    inline INT32 ret() 
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
    void setWb(bool wb = true)
    {
        mWb = wb;
    }
private:
    std::vector<std::thread> mThreads;
    std::atomic<INT32> _ret = 0;
    bool mWb = false;
    void treadFunc(const std::string&& file)
    {
        TRACE_FUNC()
        TRACE_VAR(file)
        if (not Covbr2Html::convert(file, mWb))
        {
            ++_ret;
        }
    }
};

INT32 main(const INT32 argc, const CONST_C_STRING* const argv)
{
    TRACE_FUNC()
    CovbrGlobber globber;

    const std::string cWb  = "-w";
    const std::string cHlp = "-h";

    for (INT32 i = 1; i < argc; ++i)
    {
        if (cWb == argv[i])
        {
            globber.setWb();
        }
        else if (cHlp == argv[i])
        {
            std::cout 
                << "Usage: " << std::filesystem::path(argv[0]).filename() << " [options] covbr files\n"
                << "options:\n"
                << "-h  this help\n"
                << "-w  write back cleaned covbr files\n"
            ;
            return 0;
        }
    }
    for (INT32 i = 1; i < argc; ++i)
    {
        TRACE_VAR(argv[i])
        if (cWb != argv[i] and cHlp != argv[i])
        {
            glob(argv[i], globber);
        }
    }
    return globber.ret();
}
