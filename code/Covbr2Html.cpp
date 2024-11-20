#include "Covbr2Html.h"
#include <SOM/fio.h>
#include <SOM/pyregex.h>
#include <SOM/TraceMacros.h>

#include <filesystem>
using fspath = std::filesystem::path;

using py::repl;
using std::regex, std::regex_match, std::regex_search;

#include <iostream>
using std::string, std::cout, std::cerr, std::ifstream;
#include <sstream>
using std::istringstream, std::ostringstream;
#include <vector>
using std::vector;
#include <map>
#include <ciso646>

bool Covbr2Html::setOdir(const std::string& odir)
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
        std::cerr << "cannot create directory " << odir << '\n';
    }
    return mOk;
}

bool Covbr2Html::convert(const std::string& fpath)
{
    if (not mOk) return false;

    //  html escape
    static const regex reAmp("&");
    static const regex reLt("<");
    static const regex reGt(">");

    //  check for missing coverage indicator
    static const regex reCheck("\\n *-->[TFtf]?( .*)?\\r?\\n");

    //  line wise
    //  single file
    static const regex reFile("(?:\\w+:)?/?\\w+(?:/\\w+)*\\.\\w+:");
    //  missing
    static const regex reNok("( *)--&gt;([TFtf])?( .*)?");
    //  covered
    static const regex reCov("( *)(TF|tf|X)( .*)?");
    //  extension
    static const regex reExt("\\.\\w+$");

    static const std::map<const string, const string> repMap = {
        {"",  "<s>X</s> "},
        {"t", "<u>t</u><s>f</s>  "},
        {"f", "<s>t</s><u>f</u>  "},
        {"T", "<u>T</u><s>F</s>  "},
        {"F", "<s>T</s><u>F</u>  "},
        {"X", " "},
        {"TF", "<u>TF</u>"},
        {"tf", "<u>tf</u>"}
    };

    const CONST_C_STRING cOkBeg = mHc ? "<i>" : "";
    const CONST_C_STRING cOkEnd = mHc ? "</i>" : "";

    mFiles.clear();

    string buff;
    TRACE_FUNC_TIME()

    //  read error?
    if (not read(buff, fpath)) return false;
    //  nothing to do?
    if (not (mFc or regex_search(buff, reCheck))) return true;

    //  write error?
    {
        const auto opath = mOdir.empty() ? fspath(fpath).parent_path() : fspath(mOdir);
        const auto fname = fspath(fpath).filename().string();
        const string ttl = repl(reExt, "", fname);

        if (not open(mOs, opath / (ttl + ".html"))) return false;

        mOs << cTtl << ttl << cHead;
    }
    {
        TRACE_FLOW_TIME(convert to html)
        //  html escape complete text
        buff = repl(reGt, "&gt;", repl(reLt, "&lt;", repl(reAmp, "&amp;", buff)));

        //  now line wise
        istringstream is(buff);
        string line;
        std::smatch sm;
        while (getline(is, line))
        {
            if (regex_match(line, reFile))
            {
                mFiles.push_back(line);
            }
            else
            {
                files2os();
                if (regex_match(line, sm, reNok))
                {
                    mOs << "<b>" << sm[1] << repMap.find(sm[2].str())->second << sm[3] << "</b>\n";
                }
                else if (regex_match(line, sm, reCov))
                {
                    mOs << cOkBeg << sm[1] << repMap.find(sm[2].str())->second << sm[3] << cOkEnd << '\n';
                }
                else
                {
                    mOs << line << '\n';
                }
            }
        }
        if (mFc) fc2os();
    }
    mOs << cTail;
    mOs.close();
    return true;
}

void Covbr2Html::files2os()
{
    if (mFiles.size() > 0)
    {
        auto last = mFiles.back();
        if (mFc)
        {
            mFiles.pop_back();
            fc2os();
        }
        mOs << "<em>" << last << "</em>\n";
        mFiles.clear();
    }
}

void Covbr2Html::fc2os()
{
    if (mFiles.size() > 0)
    {
        mOs << (mHc ? "<i>" : "");
        for (size_t i = 0; i < mFiles.size() - 1; ++i)
        {
            mOs << mFiles[i] << '\n';
        }
        mOs << mFiles.back() << (mHc ? "</i>" : "") << '\n';
        mFiles.clear();
    }
}

const string Covbr2Html::cTtl(
    "<!DOCTYPE html>\n"
    "<html lang=en>\n"
    "<head>\n"
    "<title>"
);

const string Covbr2Html::cHead(
    "</title>\n"
    "<meta charset=\"UTF-8\">\n"
    "<style>\n"
    "* { font-style: normal; text-decoration: none; font-weight: normal;\n"
    "    font-family:Consolas,Consolas,Menlo,monospace;\n"
    "    margin: 0;\n"
    "    white-space:pre;\n"
    "}\n"
    "p { font-size:10pt; margin-left: 1em; margin-bottom: 2em; }\n"
    "u { color: blue; font-weight: bold; }\n"
    "s { color: red; font-weight: bold; }\n"
    "b { background-color: hsl(355,100%,91%); }\n"
    "i { background-color: hsl(120,100%,93%); }\n"
    "em { font-weight: bold; }\n"
    "</style>\n"
    "</head>\n"
    "<body>\n"
    "<p>");

const string Covbr2Html::cTail("</p></body></html>\n");
