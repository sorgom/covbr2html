# covbr2html

## covbr
Bullseye code coverage provides a great tool: **covbr**.

```shell
$>covbr -h
BullseyeCoverage Detail Report 9.11.3 Windows License 123456
Copyright (c) Bullseye Testing Technology
Usage: covbr [options] [region ...]
Show low-level coverage measurements with annotated source code
...
-cN    --context N    Show N source lines before or after a probe [N=3]
...
-u     --uncover      Suppress fully covered probes (with -c)
...
--html                Output in HTML format
```

## -u option
With the **-u** option we get a listing of all uncovered parts of all sources concerned.
- see [small sample](sample_outputs/covbr_sample_tiny.txt)

## desired enhancement
Currently the tool needs improvements:
-   It reports all files - regardless if containing uncovered bits.
    - see [big sample](sample_outputs/covbr_sample_big.txt)
    - output of fully covered files should be optional
-   Compared to the nicely highlighted coverage browser or covhtml output
    the generated texts are not really comfortable to read.
-   The **--html** option delivers just the same plain text wrapped in a pre.

## meanwhile: covbr2html
**covbr2html** improves on this:
-   removes fully covered files from listing
-   generates real highlighted stand-alone html

## Python / C++
-   use covbr2html.py in the [sompy folder](submodules/sompy/covbr2html)
-   build stand-alone binaries from C++ code
    - see [make folder](make)
