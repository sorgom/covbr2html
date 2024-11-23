#include <Covbr2Html.h>
#include <SOM/fio.h>
#include <SOM/pyregex.h>
#include <SOM/TraceMacros.h>

#include <filesystem>
#include <iostream>
#include <map>
#include <sstream>

using fspath = std::filesystem::path;
using py::repl;
using std::regex, std::regex_match, std::regex_search;
using std::string;
using std::vector;

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
    TRACE_FUNC_TIME()

    static const CONST_C_STRING cTtl
    {
        "<!DOCTYPE html>\n"
        "<html lang=en>\n"
        "<head>\n"
        "<title>"
    };

    static const CONST_C_STRING cHead
    {
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
        "<p>"
    };

    static const CONST_C_STRING cTail { "</p></body></html>\n" };

    //  line endings
    static const regex reNl("\\r\\n|\\r");
    static const regex reEol("(\\r?\\n|\\r)");
    //  tailing spaces
    static const regex reSp("\\s+$");

    //  check for missing coverage indicator
    static const regex reCheck("\\n *-->[TFtf]?( .*)?\\n");

    //  html escape
    static const regex reAmp("&");
    static const regex reLt("<");
    static const regex reGt(">");

    //  line wise
    //  single file
    static const regex reFile("^(?:\\w+:)?/?\\w+(?:/\\w+)*\\.\\w+:$");
    //  missing
    static const regex reNok("( *)--&gt;([TFtf])?( .*)?");
    //  covered
    static const regex reCov("( *)(TF|tf|X)( .*)?");

    static const std::map<const string, const CONST_C_STRING> repMap
    {
        //  missing coverage
        {"",  "<s>X</s> "},
        {"t", "<u>t</u><s>f</s>  "},
        {"f", "<s>t</s><u>f</u>  "},
        {"T", "<u>T</u><s>F</s>  "},
        {"F", "<s>T</s><u>F</u>  "},
        //  covered
        {"X", " "},
        {"TF", "<u>TF</u>"},
        {"tf", "<u>tf</u>"}
    };

    string buff;
    if (not read(buff, fpath)) return false;

    //  nothing to do?
    buff = repl(reNl, "\n", repl(reSp, "", buff));
    if (not (mFc or regex_search(buff, reCheck))) return true;

    std::ofstream os;
    {
        const auto opath =
            mOdir.empty() ?
                fspath(fpath).replace_extension("html") :
                fspath(mOdir) / fspath(fpath).filename().replace_extension("html");

        if (not open(os, opath)) return false;

        const auto ttl = fspath(fpath).filename().replace_extension("");
        os << cTtl << ttl.string() << cHead;
    }
    {
        TRACE_FLOW_TIME(convert to html)
        //  html escape complete text
        buff = repl(reGt, "&gt;", repl(reLt, "&lt;", repl(reAmp, "&amp;", buff)));

        //  now line wise
        const CONST_C_STRING cOkBeg = mHc ? "<i>" : "";
        const CONST_C_STRING cOkEnd = mHc ? "</i>" : "";

        std::istringstream is(buff);
        vector<string> files;
        string line;
        std::smatch sm;
        while (getline(is, line))
        {
            if (regex_match(line, reFile))
            {
                files.push_back(line);
            }
            else
            {
                if (not files.empty())
                {
                    const auto last = files.back();
                    if (mFc)
                    {
                        files.pop_back();
                        fc2os(os, files);
                    }
                    os << "<em>" << last << "</em>\n";
                    files.clear();
                }

                if (regex_match(line, sm, reNok))
                {
                    os << "<b>" << sm[1] << repMap.find(sm[2].str())->second << sm[3] << "</b>\n";
                }
                else if (regex_match(line, sm, reCov))
                {
                    os << cOkBeg << sm[1] << repMap.find(sm[2].str())->second << sm[3] << cOkEnd << '\n';
                }
                else
                {
                    os << line << '\n';
                }
            }
        }
        if (mFc) fc2os(os, files);
    }
    os << cTail;
    os.close();
    return true;
}

void Covbr2Html::fc2os(std::ofstream& os, const vector<string>& files)
{
    if (not files.empty())
    {
        os << (mHc ? "<i>" : "");
        for (size_t i = 0; i < files.size() - 1; ++i)
        {
            os << files[i] << '\n';
        }
        os << files.back() << (mHc ? "</i>" : "") << '\n';
    }
}
