#include <CovbrGlobber.h>
#include <SOM/docOpts.h>
#define TRACE_ME
#include <SOM/TraceMacros.h>

#include <filesystem>
#include <iostream>
#include <regex>

const CONST_C_STRING cOpts =
    "options:\n"
    "-c  highlight covered items\n"
    "-w  write back cleaned covbr text files\n"
    "-h  this help\n"
;

void help(const CONST_C_STRING arg)
{
    std::cout
        << "\nconvert covbr text files to html\n\n"
        << "usage: " << std::filesystem::path(arg).filename().string() << " [options] covbr files\n"
        << cOpts;
    ;
}

INT32 main(const INT32 argc, const CONST_C_STRING* const argv)
{
    TRACE_FUNC_TIME()
    INT32 ret = 1;
    DocOpts opts;
    if (opts.process(cOpts, argc, argv))
    {
        ret = 0;
        if (opts.isSet('h'))
        {
            help(argv[0]);
        }
        else
        {
            CovbrGlobber globber;
            globber.setWb(opts.isSet('w'));
            globber.setHc(opts.isSet('c'));
            for (INT32 i = 0; i < opts.argc(); ++i)
            {
                fglob(opts.args()[i], globber);
            }
            ret = globber.ret();
        }
    }
    return ret;
}

