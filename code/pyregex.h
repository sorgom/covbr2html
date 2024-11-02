//  Python regex style replacement
#pragma once
#ifndef PYREPL_H
#define PYREPL_H

#include <regex>

namespace py
{
    template <typename T>
    inline auto repl(const std::regex& re, const char* const fmt, const T& src)
    {
        return std::regex_replace(src, re, fmt);
    }

    template <typename T>
    inline auto repl(const std::regex& re, const char* const fmt, const T&& src)
    {
        return std::regex_replace(src, re, fmt);
    }

    class regex : public std::regex
    {
    public :
        regex(const char* const re) : std::regex(re) {}

        template <typename T>
        inline auto sub(const char* const fmt, const T& src) const
        {
            return repl(*this, fmt, src);
        }

        template <typename T>
        inline auto sub(const char* const fmt, const T&& src) const
        {
            return repl(*this, fmt, src);
        }
    };
}

#endif // _H