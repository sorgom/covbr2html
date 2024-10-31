#include <RegexBuffer.h>
#include <trace.h>

RegexBuffer::~RegexBuffer()
{
    TRACE("RegexBuffer::~RegexBuffer()")
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

bool RegexBuffer::read(const CONST_C_STRING filename)
{
    TRACE("read: " << filename)
    std::ifstream is(filename);
    mOk = is.good();
    if (mOk)
    {
        is.seekg(0, is.end);
        const auto end = is.tellg();
        is.seekg(0, is.beg);
        const auto fsize = end - is.tellg();
        if (resize(fsize))
        {
            is.read(rBuf, fsize);
            rPos = fsize;
            rBuf[rPos] = '\0';
        }
    }
    is.close();
    return mOk;
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

CONST_C_STRING RegexBuffer::str() const
{ 
    static const CHAR dummy = '\0';
    return mOk? rBuf : &dummy;
}

bool RegexBuffer::write(CONST_C_STRING filename) const
{
    bool ok = mOk;
    if (ok)
    {
        TRACE("write: " << rPos)
        std::ofstream os(filename);
        ok = os.good();
        if (ok)
        {
            os << rBuf;
        }
        os.close();
    }
    return ok;
}

bool RegexBuffer::resize(const size_t sz)
{
    if (sz > mHyst)
    {
        const size_t nSize = sz * mFactor;
        TRACE("incomming: " << sz)
        TRACE("resize to: " << nSize)
        mOk =
            resize(buf1, nSize) and
            resize(buf2, nSize);
        rPos = 0;
        wPos = 0;
        rBuf = buf1;
        wBuf = buf2;
        mSize = mOk ? nSize : 0;
        mHyst = mOk ? (sz + nSize + 1) / 2 : 0;
        TRACE_VAR(mOk)
    }
    return mOk;
}

bool RegexBuffer::resize(C_STRING& ptr, const size_t sz)
{
    del(ptr);
    ptr = new CHAR[sz + 1];
    return ptr != nullptr;
}

void RegexBuffer::swap()
{
    TRACE("swap")
    TRACE_VAR(wPos)
    rPos = wPos;
    wPos = 0;
    const C_STRING tmp = rBuf;
    rBuf = wBuf;
    wBuf = tmp;
    rBuf[rPos] = '\0';
}

void RegexBuffer::del(C_STRING& ptr)
{
    if (ptr != nullptr) delete[] ptr;
    ptr = nullptr;
}


