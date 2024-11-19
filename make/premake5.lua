--  ============================================================
--  premake5 build rules for covbr2html
--  ============================================================

buildoptions_vs = '/std:c++17 /MP /W4 /O2 /Ot /wd4100 /wd4103'
-- /wd4244 illegal conversion
buildoptions_gcc = '-std=c++17 -O3 -pedantic-errors -Wall'

somcpp = '../submodules/somcpp/'
somsrc = somcpp .. 'src/'

workspace 'covbr2html'

    configurations { 'ci', 'trace_on', 'trace_all' }
    language 'C++'
    targetdir '../build'
    objdir  '../build/%{_TARGET_OS}/%{cfg.name}'
    defines { 'NDEBUG' }
    optimize 'Speed'
    kind 'ConsoleApp'


    filter { 'action:vs*' }
        warnings 'high'
        buildoptions { buildoptions_vs }

    filter { 'action:gmake*' }
        buildoptions { buildoptions_gcc }
        linkoptions { '-pthread' }

    filter { 'configurations:trace_on' }
        defines { 'TRACE_ON' }

    filter { 'configurations:trace_all' }
        defines { 'TRACE_ALL' }

    project 'covbr2html'
        includedirs { '../code', somcpp .. '/include' }
        files {
            '../code/*.cpp',
            somsrc .. 'fglob.cpp',
            somsrc .. 'fio.cpp',
            somsrc .. 'docopts.cpp'
        }

    project 'lab'
        includedirs { '../lab', '../code', somcpp .. '/include' }
        files {
            '../lab/*.cpp',
            somsrc .. '*.cpp'
        }
