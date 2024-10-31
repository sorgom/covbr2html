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

    //  not thread-safe
    static RegexBuffer& instance();

    ~RegexBuffer();

    int overflow(int c) override;

    bool read(CONST_C_STRING filename);
    bool repl(const std::regex re, CONST_C_STRING fmt);
    inline CONST_C_STRING str() const { return rBuf; }

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
    
    inline CONST_C_STRING begin() const { return rBuf; }
    inline const char* end() const { return rBuf + rPos; }
    bool resize(size_t sz);
    bool resize(C_STRING& ptr, size_t sz);
    void swap();
    void del(C_STRING& ptr);

    RegexBuffer(const RegexBuffer&) = delete;
    RegexBuffer(RegexBuffer&&) = delete; 
    RegexBuffer& operator = (const RegexBuffer&) = delete;
};

#endif // _H