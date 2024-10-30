#include <iostream>
using std::cout, std::endl;
#include <iterator>
#include <regex>
#include <string>

#include <BaseTypes.h>

#include <iostream>
#include <iomanip>

#include <streambuf>

//  NullBuffer and NullStream are used to prevent from output
//  thanks to ChatGPT for the basics of this
class NullBuffer : 
    public std::streambuf,
    public std::ostream 
{
public:
    inline NullBuffer() : std::ostream(this)
    {
        resize(1024);
    }
    inline int overflow(int c) override
    {
        if (mPos < mSize)
        {
            mBuffer[mPos] = static_cast<CHAR>(c);
            mBuffer[++mPos] = '\0';
        }
        return c; // Do nothing with the character
    }
    inline CONST_C_STRING str() const { return mBuffer; }
    inline CONST_C_STRING begin() const { return mBuffer; }
    inline const char* end() const { return mBuffer + mPos; }
    inline void clear() { mPos = 0; }
    inline void resize(const size_t sz)
    {
        if (mBuffer != nullptr) delete[] mBuffer;
        mBuffer = new CHAR[sz];
        mSize = sz;
        mPos = 0;
    }
private:
    size_t mSize = 0;
    size_t mPos = 0;    
    CHAR* mBuffer = nullptr;
};

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
//     NullBuffer mBuffer;
// };

int main()
{
   std::string text = "Quick brown fox - QUICK BROWN FOX";
   std::regex re1("a|e|i|o|u");
   std::regex re2("A|E|I|O|U");
 
   NullBuffer ns; 

   // write the results to an output iterator
   std::regex_replace(std::ostreambuf_iterator<char>(ns),
                      text.begin(), text.end(), re1, "*");
 
    cout << ns.str() << '\n';
    // write the results to an output iterator
    std::regex_replace(std::ostreambuf_iterator<char>(std::cout),
                      ns.begin(), ns.end(), re2, "*");

   // construct a string holding the results
   std::cout << '\n' << std::regex_replace(text, re1, "[$&]") << '\n';
}