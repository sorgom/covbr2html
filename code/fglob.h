//  ============================================================
//  a very simple file name globbing utility for windows
//  intended to be used for main arguments
//  - supports * [0-9] ? in file names
//  - does not support any directory globbing
//  ============================================================
//  created by Manfred Sorgo

#pragma once
#ifndef FGLOB_H
#define FGLOB_H

#include <BaseTypes.h>
#include <regex>

class I_GlobProcessor
{
public:
    virtual void process(const CONST_C_STRING item) = 0;
};

//!  glob files
//!  @param item file, or directory with file glob pattern
//!  @param proc I_GlobProcessor
void fglob(const CONST_C_STRING item, I_GlobProcessor& proc);

#endif // _H