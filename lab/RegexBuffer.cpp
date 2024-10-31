#include <RegexBuffer.h>

RegexBuffer::~RegexBuffer()
{
    del(buf1);
    del(buf2);
}

RegexBuffer& RegexBuffer::instance()
{
    static RegexBuffer instance;
    return instance;
}

int RegexBuffer::overflow(const int c)
{
    mOk = mOk and wPos < mSize;
    if (mOk)
    {
        wBuf[wPos++] = static_cast<CHAR>(c);
    }
    return c;
}

bool RegexBuffer::resize(const size_t sz)
{
    if (sz > mSize)
    {
        mOk =
            resize(buf1, sz) and
            resize(buf2, sz);
    }
    rPos = 0;
    wPos = 0;
    rBuf = buf1;
    wBuf = buf2;
    mSize = mOk ? sz : 0;
    return mOk;
}

bool RegexBuffer::read(const CONST_C_STRING filename)
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
void RegexBuffer::swap()
{
    rPos = wPos;
    wPos = 0;
    const C_STRING tmp = rBuf;
    rBuf = wBuf;
    wBuf = tmp;
    rBuf[rPos] = '\0';
}

bool RegexBuffer::repl(const std::regex re, const CONST_C_STRING fmt)
{
    if (mOk)
    {
        std::regex_replace(std::ostreambuf_iterator<char>(*this),
                    begin(), end(), re, fmt);
        
        if (mOk) swap();
    }
    return mOk;
}
void RegexBuffer::del(C_STRING& ptr)
{
    if (ptr != nullptr) delete[] ptr;
    ptr = nullptr;
}

bool RegexBuffer::resize(C_STRING& ptr, const size_t sz)
{
    del(ptr);
    ptr = new CHAR[sz + 1];
    return ptr != nullptr;
}

