#include <fglob.h>
#include <pyrepl.h>

#define TRACE_ME
#include <trace.h>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>

using py::repl;
using fp = std::filesystem::path;
using std::regex, std::regex_replace, std::regex_match;
using std::string;


void fglob(const CONST_C_STRING item, I_GlobProcessor& proc)
{
    //  any posix system has expanded globbing
    if (std::filesystem::is_regular_file(item))
    {
        proc.process(item);
    }
#ifdef _WIN32
    //  windows crap shell has no globbing
    //  therefore at least apply simple file globbing by regex
    else if (string(item).find('*') != string::npos)
    {
        // simple globbing:
        // abc*[0-9]?.txt -> abc.*[0-9].\.txt
        // . -> \.
        // * -> .*
        // ? -> .
        TRACE_FLOW(fglob win)
        static const regex reAst("[*]");
        static const regex reDot("[.]");
        static const regex reQue("[?]");

        const string restr = repl(reQue, ".", repl(reAst, ".*", repl(reDot, "\\.", fp(item).filename().string())));
        TRACE_VAR(restr)
        const regex re(restr);
        for (const auto& entry : std::filesystem::directory_iterator(fp(item).parent_path()))
        {
            if (
                entry.is_regular_file() and
                regex_match(entry.path().filename().string(), re)
            )
            {
                proc.process(entry.path().string().c_str());
            }
        }
    }
#endif
}
