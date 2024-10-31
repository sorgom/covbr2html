#include <RegexBuffer.h>

RegexBuffer::~RegexBuffer()
{
    del(buf1);
    del(buf2);
}

int RegexBuffer::overflow(const int c)
{
    mOk = mOk and wPos < mSize;
    if (mOk)
    {
        wBuf[wPos++] = static_cast<CHAR>(c);
        wBuf[wPos] = '\0';
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
