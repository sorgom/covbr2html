#pragma once
#ifndef REGEXBUFFER_H
#define REGEXBUFFER_H

#include <BaseTypes.h>

#include <fstream>
#include <regex>
#include <streambuf>

class RegexBuffer : 
    public std::streambuf,
    public std::ostream 
{
public:
    inline RegexBuffer(const size_t factor = 2) : 
        std::ostream(this), 
        mFactor(factor < 1 ? 1 : factor)
    {}

    ~RegexBuffer();

    int overflow(int c) override;


    inline CONST_C_STRING str() const { return rBuf; }
private:    
    
    inline CONST_C_STRING begin() const { return rBuf; }
    inline const char* end() const { return rBuf + rPos; }
    bool resize(size_t sz);
    bool read(const CONST_C_STRING filename)
    {
        std::ifstream is(filename);
        mOk = is.good();
        if (mOk)
        {
            is.seekg(0, is.end);
            const auto end = is.tellg();
            is.seekg(0, is.beg);
            const auto fsize = end - is.tellg();
            if (resize(fsize * mFactor))
            {
                is.read(rBuf, fsize);
                rPos = fsize;
                rBuf[rPos] = '\0';
            }
        }
        is.close();
        return mOk;
    }
    void swap()
    {
        rPos = wPos;
        wPos = 0;
        const C_STRING tmp = rBuf;
        rBuf = wBuf;
        wBuf = tmp;
    }
    bool repl(const std::regex re, const CONST_C_STRING fmt)
    {
        if (mOk)
        {
            std::regex_replace(std::ostreambuf_iterator<char>(*this),
                      begin(), end(), re, fmt);
            
            if (mOk) swap();
        }
        return mOk;
    }

private:
    const size_t mFactor;
    size_t mSize = 0;
    size_t rPos = 0;    
    size_t wPos = 0;
    C_STRING buf1 = nullptr;
    C_STRING buf2 = nullptr;
    C_STRING rBuf = nullptr;
    C_STRING wBuf = nullptr;
    bool mOk = false;

    void del(C_STRING& ptr)
    {
        if (ptr != nullptr) delete[] ptr;
        ptr = nullptr;
    }
    bool resize(C_STRING& ptr, const size_t sz)
    {
        del(ptr);
        ptr = new CHAR[sz + 1];
        return ptr != nullptr;
    }

    RegexBuffer(const RegexBuffer&) = delete;
    RegexBuffer(RegexBuffer&&) = delete; 
    RegexBuffer& operator = (const RegexBuffer&) = delete;
};

#endif // _H