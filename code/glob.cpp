#include <glob.h>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>

using fp = std::filesystem::path;
using std::regex, std::regex_replace, std::regex_match;
using std::string;

void glob(const CONST_C_STRING item, globFunc func)
{
    if (std::filesystem::is_regular_file(item))
    {
        func(item);
    }
    else if (string(item).find('*') != string::npos)
    {
        static const regex reAst("[*]");
        static const regex reDot("[.]");
        const string rep = regex_replace(regex_replace(fp(item).filename().string(), reDot, "\\."), reAst, ".*");
        const regex re(rep);
        for (const auto& entry : std::filesystem::directory_iterator(fp(item).parent_path()))
        {
            if (
                entry.is_regular_file() and
                regex_match(entry.path().filename().string(), re)
            )
            {
                func(entry.path().string().c_str());
            }
        }
    }
}
