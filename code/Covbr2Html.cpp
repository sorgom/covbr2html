#include <Covbr2Html.h>

#include <filesystem>
#include <iostream>
#include <fstream>
#include <streambuf>

using std::cout, std::endl;
using std::regex, std::regex_replace, std::regex_constants::extended;
using std::string;
using fpath = std::filesystem::path;

bool Covbr2Html::convert(const CONST_C_STRING covbrFile)
{
    // removal of multiple files
    static const string rBeg    = "(?:^|(\\n))";
    static const string rFile   = "(?:\\w+:/?)?\\w+(?:/\\w+)*\\.(?:cpp|h):";
    static const string rEclip  = "(?: +\\.\\.\\.\\n)?";
    static const string rDouble = rBeg + rEclip + "(?:" + rFile + "\\n)*(" + rFile + ")";
    static const string rSpc    = string("\\s+(\\n" + rFile + ")");

    static const regex reDouble(rDouble);
    static const regex reSpc(rSpc);
    // static const regex reTail(rTail);
    static const regex reFile(rBeg + '(' + rFile + ')');
    static const regex reLead("^\\s+");
    static const regex reTail(rFile + "\\s+$");

    // html conversion
    static const regex reAmp("&");
    static const regex reLt("<");
    static const regex reGt(">");

    // OK cases
    static const regex re_tf(rBeg + "( *)(TF|tf)(?: (.*))?");
    static const regex re_X(rBeg + "( *)X  (.*)");

    // NOK cases
    static const string rNok = rBeg + "( *)--&gt;";
    static const regex re_x(rNok + " (.*)");
    static const regex re_t(rNok + "t (.*)");
    static const regex re_f(rNok + "f (.*)");
    static const regex re_T(rNok + "T (.*)");
    static const regex re_F(rNok + "F (.*)");

    //  replacements
    static const string bOk = "$1<span>$2";
    static const string bNok = "$1<span class=x>$2";
    static const string es = " $3</span>";

    static const string rep_tf = bOk + "<u>$3</u> $4</span>";
    static const string rep_X  = bOk + "<u>&gt;&gt;</u>" + es;

    static const string rep_x = bNok + "<s>X</s>  " + es;
    static const string rep_t = bNok + "<u>t</u><s>f</s>  " + es;
    static const string rep_f = bNok + "<s>t</s><u>f</u>  " + es;
    static const string rep_T = bNok + "<u>T</u><s>F</s>  " + es;
    static const string rep_F = bNok + "<s>T</s><u>F</u>  " + es;

    static const regex reExt("\\.\\w+$");

    string buff;
    const bool ok = read(buff, covbrFile);
    if (ok)
    {
        string rep = repl(reTail, "", repl(reLead, "", repl(reSpc, "\n$1", repl(reDouble, "\n$1$2", buff))));

        //  TODO:  write file if changed
        if (rep != buff)
        {
            std::ofstream os(covbrFile);
            if (os.good())
            {
                cout << "-> " << covbrFile << endl;
                os << rep;
            }
            os.close();
        }

        rep =   repl(re_tf, rep_tf, 
                repl(re_X,  rep_X,
                repl(re_x,  rep_x,
                repl(re_t,  rep_t,
                repl(re_f,  rep_f,
                repl(re_T,  rep_T,
                repl(re_F,  rep_F, 
                repl(reFile, "$1<em>$2</em>", 
                repl(reGt, "&gt;", repl(reLt, "&lt;", repl(reAmp, "&amp;", rep)
            ))))))))));

        const string ttl = repl(reExt, "", fpath(covbrFile).filename().string());
        const string out = repl(reExt, ".html", covbrFile);

        {
            std::ofstream os(out);
            if (os.good())
            {
                cout << "-> " << out << endl;
                os << cTtl << ttl << cHead << rep << cTail;
            }
            os.close();
        }
    }
    else
    {
        cout << "Error reading file " << covbrFile << endl;
    }
    return ok;
}

bool Covbr2Html::read(string& trg, const CONST_C_STRING txtFile)
{
    //  read text file into string
    std::ifstream is(txtFile);
    const bool ok = is.good();
    if (ok)
    {
        trg.assign((std::istreambuf_iterator<char>(is)),
            std::istreambuf_iterator<char>());
    }
    is.close();
    return ok;
}
const CONST_C_STRING Covbr2Html::cTtl =
    "<!DOCTYPE html>\n"
    "<html lang=en>\n"
    "<head>\n"
    "<title>";

const CONST_C_STRING Covbr2Html::cHead =
    "</title>\n"
    "<meta charset=\"UTF-8\">\n"
    "<style>\n"
    "* { font-style: normal; text-decoration: none;\n"
    "    font-family:Consolas,Consolas,Menlo,monospace;\n"
    "    margin: 0;\n"
    "    white-space:pre;\n"
    "}\n"
    "p { font-size:10pt; margin-left: 1em; margin-bottom: 2em; }\n"
    "span { background-color: hsl(120,100%,93%); }\n"
    "span > u { color: blue; font-weight: bold; }\n"
    "span.x { background-color: hsl(355,100%,91%); }\n"
    "span > s { color: red; font-weight: bold; }\n"
    "em { font-weight: bold; font-size:12pt; }\n"
    "span > s { color: red; font-weight: bold; }\n"
    "</style>\n"
    "</head>\n"
    "<body>\n"
    "<p>";

const CONST_C_STRING Covbr2Html::cTail = "</p></body></html>";
