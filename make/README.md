# how to build and run
## linux make / gcc
````shell
covbr2html/make$> make clean
covbr2html/make$> make -j
````
- find binary in build folder
````shell
covbr2html/make$> cd ../build/linux
covbr2html/build$> ls
````

## msbuild / visual studio
- requires [premake5](https://premake.github.io/)

### create solution for your VS version
````shell
covbr2html\make> premake5 --help
covbr2html\make> premake5 vsNNNN
````
### build
````shell
covbr2html\make> cd ../vs
covbr2html\vs> msbuild -m covbr2html.sln
````
- find binary in build folder
````shell
covbr2html\vs> cd ../build/windows
covbr2html\build\windows> dir /B *.exe
````
