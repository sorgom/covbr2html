# covbr2html

## covbr
Bullseye code coverage provides a great tool: **covbr**.

```shell
$>covbr -h
BullseyeCoverage Detail Report 9.11.3 Windows License 123456
Copyright (c) Bullseye Testing Technology
Usage: covbr [options] [region ...]
Show low-level coverage measurements with annotated source code
-a     --all          Show all source lines
-cN    --context N    Show N source lines before or after a probe [N=3]
-dDir  --srcdir Dir   Source file directory is Dir
-eN    --tab N        Expand tabs to every N column [N=8]
-fPath --file Path    Coverage file is Path
-h     --help         Display this usage summary
-i     --no-time      Ignore source file date/time
-oPath --output Path  Write output to Path instead of stdout
-q     --quiet        Quiet
-u     --uncover      Suppress fully covered probes (with -c)
-v     --verbose      Verbose
-wN    --width N      Report width is N columns
--csv                 Output comma separated values
--html                Output in HTML format
--no-banner           Suppress startup banner
```

## -u option
With the **-u** option we get a listing of all uncovered parts of all sources concerned.
- see [sample output](sample_outputs/covbr_sample_big.txt)

## desired enhancement
Currently the tool needs improvements:
-   It reports all files - regardless if containing uncovered bits.
    - this should be optional
-   Compared to the nicely highlighted coverage browser or covhtml output
    the generated texts are not really comfortable to read.
-   The **--html** option delivers the same plain text wrapped in a pre.

## meanwhile: covbr2html
**covbr2html** improves on this:
-   removes fully covered files from listing
-   generates real highlighted stand-alone html

## Python / C++ / binary
-   use covbr2html.py in the [sompy folder](submodules/sompy/covbr2html)
-   build linux stand-alone binary from C++ code
    - see [make folder with Makefile](make)
-   find linux binary in the [sombin repo](https://github.com/sorgom/sombin)
