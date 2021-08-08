# SimpleRIFFLib
SimpleRIFFLib is a library for reading and writing RIFF/IFF files with a simple API.

# What is a RIFF/IFF file
IFF is a container file format for storing binary data.  It lets you structure binary data so you can add new blocks of data and
still have older version read them.

It is a standard introduced by Electronic Arts and Commodore Amiga back in 1985.  The concepts have been widely copied to
many other formats (PNG for example), that use the basic idea but not the actual spec.

Microsoft decided to use the IFF spec but with the block sizes in little-endian instead of big-endian to make it easyer work with
on the x86 processor.

# Why would I want to use it for my data?

You can think of RIFF/IFF as like XML but for binary data.  It lets you ignore things you do not understand, add human readable
tags (when viewed with a hex editor), and store data in a hierarchy.

This library tries to make it easy to store / read your data without knowning much about the underlying data structure.

# Quick start
To compile SimpleRIFFLib all you need to do is compile "src/RIFF.cpp" + your code.  There are no external dependencies (beyond standard C++)

## Helloworld
This is an example of compiling the HelloWorld example in the examples directory using gcc.

```
cd SimpleRIFFLib/examples/HelloWorld
g++ -g -I ../../src ../../src/RIFF.cpp main.cpp -o main
./main
```

### Save example
This is am example of saving out some basic data that's in a structure.

This example will write the `MyData` structure to a file named "Example.RIFF".

```C++
#include "RIFF.h"
#include <stdio.h>

struct MyData
{
    int First;
    int Second;
    double Third;
};

int main(void)
{
    class RIFF RIFF;
    struct MyData Data2Save;

    try
    {
        Data2Save.First=1;
        Data2Save.Second=2;
        Data2Save.Third=3.3;

        RIFF.Open("Example.RIFF",e_RIFFOpen_Write,"EXAM");
        RIFF.StartDataBlock("DATA");
        RIFF.Write(&Data2Save,sizeof(Data2Save));
        RIFF.EndDataBlock();
        RIFF.Close();
    }
    catch(e_RIFFErrorType err)
    {
        printf("Error:%d, %s\n",err,RIFF.Error2Str(err));
    }

    return 0;
}

```

1. The first thing we do is make an instance of RIFF named `RIFF`
   - The class takes 1 arg, a bool that lets you set if you want an RIFF or IFF file
   - By default it will make a RIFF file.
2. We open a new file for writing with the `RIFF.Open("Example.RIFF",e_RIFFOpen_Write,"EXAM");` line.
   - The first arg is the filename, next we have the mode (write), the last arg is the file type
   - File type is used so we know what type of file this is.  When we load it back it we check for this name.
   - The name must be 4 letters long and made of AscII chars (0-9, a-z, A-Z, _ or space).
   - You can pad a name with spaces.  For example "DOG " would be valid where as "DOG" would not.
3. We start a data block with `RIFF.StartDataBlock("DATA");`.
   - This will write the chunk ID (block name) to the file and reserve space for the block size
   - The name of the block follows the same rules as the file type.  4 letters long, AscII only.
4. Now we write our data.
   - We only do one write here but we can call write many times and they will all be merge into one block.
5. Next we end the block with `RIFF.EndDataBlock();`
   - This will back up in the file and write the number of bytes in this block to the reserved space for this block.
6. Finally we close the file
   - This will flush any unwritten data and record the total file size
