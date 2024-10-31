#include <iostream>
using std::cout, std::endl;
#include <iterator>
#include <regex>
#include <string>

#include <BaseTypes.h>

#include <iostream>
#include <iomanip>

#include <streambuf>
#include <fstream>

#include <RegexBuffer.h>

// //  RegexBuffer and NullStream are used to prevent from output
// //  thanks to ChatGPT for the basics of this
// class RegexBuffer : 
//     public std::streambuf,
//     public std::ostream 
// {
// public:

//     inline RegexBuffer(const size_t factor = 2) : 
//         std::ostream(this), 
//         mFactor(factor < 1 ? 1 : factor)
//     {}

//     inline ~RegexBuffer()
//     {
//         del(buf1);
//         del(buf2);
//     }

//     inline int overflow(int c) override
//     {
//         mOk = mOk and wPos < mSize;
//         if (mOk)
//         {
//             wBuf[wPos++] = static_cast<CHAR>(c);
//             wBuf[wPos] = '\0';
//         }
//         return c; // Do nothing with the character
//     }
//     inline CONST_C_STRING str() const { return rBuf; }
//     inline CONST_C_STRING begin() const { return rBuf; }
//     inline const char* end() const { return rBuf + rPos; }
//     inline void clear() { 
//         wPos = 0; 
//         rPos = 0;
//         mOk = true;
//     }
//     bool resize(const size_t sz)
//     {
//         if (sz > mSize)
//         {
//             mOk =
//                 resize(buf1, sz) and
//                 resize(buf2, sz);
//         }
//         rPos = 0;
//         wPos = 0;
//         rBuf = buf1;
//         wBuf = buf2;
//         mSize = mOk ? sz : 0;
//         return mOk;
//     }
//     bool read(const CONST_C_STRING filename)
//     {
//         std::ifstream is(filename);
//         mOk = is.good();
//         if (mOk)
//         {
//             is.seekg(0, is.end);
//             const auto end = is.tellg();
//             is.seekg(0, is.beg);
//             const auto fsize = end - is.tellg();
//             if (resize(fsize * mFactor))
//             {
//                 is.read(rBuf, fsize);
//                 rPos = fsize;
//                 rBuf[rPos] = '\0';
//             }
//         }
//         is.close();
//         return mOk;
//     }
//     void swap()
//     {
//         rPos = wPos;
//         wPos = 0;
//         const C_STRING tmp = rBuf;
//         rBuf = wBuf;
//         wBuf = tmp;
//     }
//     bool repl(const std::regex re, const CONST_C_STRING fmt)
//     {
//         if (mOk)
//         {
//             std::regex_replace(std::ostreambuf_iterator<char>(*this),
//                       begin(), end(), re, fmt);
            
//             if (mOk) swap();
//         }
//         return mOk;
//     }

// private:
//     const size_t mFactor;
//     size_t mSize = 0;
//     size_t rPos = 0;    
//     size_t wPos = 0;
//     C_STRING buf1 = nullptr;
//     C_STRING buf2 = nullptr;
//     C_STRING rBuf = nullptr;
//     C_STRING wBuf = nullptr;
//     bool mOk = false;

//     void del(C_STRING& ptr)
//     {
//         if (ptr != nullptr) delete[] ptr;
//         ptr = nullptr;
//     }
//     bool resize(C_STRING& ptr, const size_t sz)
//     {
//         del(ptr);
//         ptr = new CHAR[sz + 1];
//         return ptr != nullptr;
//     }

//     RegexBuffer(const RegexBuffer&) = delete;
//     RegexBuffer(RegexBuffer&&) = delete; 
//     RegexBuffer& operator = (const RegexBuffer&) = delete;
// };

// class NullStream : public std::ostream
// {
// public:
//     inline NullStream(UINT32 = 0) : std::ostream(&mBuffer) {}
//     CONST_C_STRING str() const { return mBuffer.str(); }
//     void clear() { mBuffer.clear(); }
//     const char* c_str() const { return mBuffer.str(); }
//     const char* begin() const { return mBuffer.str(); }
//     const char* end() const { return mBuffer.mPtr; }
// private:
//     RegexBuffer mBuffer;
// };

int main(const INT32 argc, const CONST_C_STRING* const argv)
{
//    std::string text = "Quick brown fox - QUICK BROWN FOX";
//    std::regex re1("a|e|i|o|u");
//    std::regex re2("A|E|I|O|U");
 
//    // write the results to an output iterator
//    std::regex_replace(std::ostreambuf_iterator<char>(ns),
//                       text.begin(), text.end(), re1, "*");
 
//     cout << ns.str() << '\n';
//     // write the results to an output iterator
//     std::regex_replace(std::ostreambuf_iterator<char>(std::cout),
//                       ns.begin(), ns.end(), re2, "*");

//    // construct a string holding the results
//    std::cout << '\n' << std::regex_replace(text, re1, "[$&]") << '\n';
    RegexBuffer rb;
    if (argc > 1)
    {
        const std::regex re1("(\\n *)-->");
        const std::regex re2("(#include <)\\w+(\\.h>)");
        cout << "Reading " << argv[1] << endl;
        if (rb.read(argv[1]))
        {
            // std::regex re1("a|e|i|o|u");
            // std::regex re2("A|E|I|O|U");
            // rb.replace(re1, "*");
            // rb.replace(re2, "*");
            cout << "replacing" << endl;
            if (
                rb.repl(re1, "$1???") and
                rb.repl(re2, "$1Wumpel$2")   
            )
                cout << rb.str() << '\n';
        }
    } 


    return 0;
}