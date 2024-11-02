#include <CovbrGlobber.h>

#define TRACE_ME
#include <trace.h>

#include <filesystem>
#include <iostream>

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
                << "\nUsage: " << std::filesystem::path(argv[0]).filename().string() << " [options] covbr files\n"
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
            fglob(argv[i], globber);
        }
    }
    return globber.ret();
}
