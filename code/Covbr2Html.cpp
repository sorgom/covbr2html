#include <Covbr2Html.h>

#include <iostream>
using std::cout, std::endl;
#include <fstream>
#include <streambuf>
using std::regex, std::regex_replace;

using std::string;

bool Covbr2Html::convert(const CONST_C_STRING covbrFile)
{
    // removal of multiple files
    static const string rBeg  = "(?:^|(\\n))";
    static const string rFile = "(?:\\w+:/?)?\\w+(?:/\\w+)*\\.(?:cpp|h):";
    static const string rEclip = "(?: +\\.\\.\\.\\n)?";
    static const string rDouble = rBeg + rEclip + "(?:" + rFile + "\\n)*(" + rFile + ")";
    static const string rSpc = string("\\s+(\\n" + rFile + ")");
    static const string rTail = rEclip + "(?:" + rFile + ")?\\s*$";
    static const regex  reDouble(rDouble);
    static const regex  reSpc(rSpc);
    static const regex  reTail(rTail);

    // html conversion
    static const regex reAmp("&");
    static const regex reLt("<");
    static const regex reGt(">");

    // OK cases
    static const regex re_tf(rBeg + "( *)(TF|tf) ");

    string buff;
    const bool ok = read(buff, covbrFile);
    if (ok)
    {

        string rep = repl(reTail, "", repl(reSpc, "\n$1", repl(reDouble, "\n$1$2", buff)));


        //  TODO:  write file if changed

        // rep = regex_replace(rep, reGt, "&gt;");
        rep = repl(reGt, "&gt;", repl(reLt, "&lt;", repl(reAmp, "&amp;", rep)));
        // regex_replace(regex_replace(regex_replace(rep, reAmp, "&Amp;"), reLt, "&Lt;"), reGt, "&gt;");

        rep = repl(re_tf, "$1$2<u>$3</u>", rep);

        cout << rep << endl;
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

