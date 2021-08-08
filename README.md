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
To compile SimpleRIFFLib all you need to do is compile "src/RIFF.cpp" + your code.

There are no external dependencies (beyond standard C++)

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
   - This will write the chunk ID (block name) to the file and reserve space for the block size.
   - The name of the block follows the same rules as the file type.  4 letters long, AscII only.
4. Now we write our data.
   - We only do one write here but we can call write many times and they will all be merge into one block.
5. Next we end the block with `RIFF.EndDataBlock();`
   - This will back up in the file and write the number of bytes in this block to the reserved space for this block.
6. Finally we close the file
   - This will flush any unwritten data and record the total file size

### Load example
This is am example of loading your data back in.

This example will read the `MyData` structure from the save example from file named "Example.RIFF".

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
    char ChunkID[5];
    uint32_t ChunkLen;
    struct MyData Data2Load;

    try
    {
        /* Default the data before we try to load */
        Data2Load.First=0;
        Data2Load.Second=0;
        Data2Load.Third=0;

        RIFF.Open("Example.RIFF",e_RIFFOpen_Read,"EXAM");
        while(RIFF.ReadNextDataBlock(ChunkID,&ChunkLen))
        {
            if(strcmp(ChunkID,"DATA")==0)
            {
                if(ChunkLen!=sizeof(Data2Load))
                    throw("Data error");
                RIFF.Read(&Data2Load,sizeof(Data2Load));
            }
        }
        RIFF.Close();
    }
    catch(e_RIFFErrorType err)
    {
        printf("Error:%d, %s\n",err,RIFF.Error2Str(err));
    }
    catch(const char *Err)
    {
        printf("ERR:%s\n",Err);
    }

    return 0;
}

```
1. Again we make an instance of RIFF named `RIFF`
2. We then open the file for reading with the `RIFF.Open("Example.RIFF",e_RIFFOpen_Read,"EXAM");` line
   - We pass in "EXAM" again as the file type.  This time the library will check that the file type for the file we are loading matches this
3. Now we read the next chunk (block) from the file with `while(RIFF.ReadNextDataBlock(ChunkID,&ChunkLen))`
   - ReadNextDataBlock() return true if it finds a data block, or false if there are no more data blocks to read
   - It returns the data block ID in the `ChunkID` buffer.  This will use the same rules as the file type.  It will also be converted into a string so you can do strcmp on it.
   - `ChunkLen` in the number of bytes in this block.  You can read all the bytes or just some of them.  If you don't read all the bytes the next call to ReadNextDataBlock() will skip whatever you didn't read.
4. Next we check if we want to load this data block.  We do a strcmp() on it to see if we recognize it.
   - If we don't recognize it (or don't want to load it) we just ignore it and ReadNextDataBlock() will skip it on the next call
   - If we have more than 1 data block, we use `else if()` to check for more data block ID's
5. If we found our data block then we check that `struct MyData` is big enough to load the block by checking against `ChunkLen`
   - We do this so if someone hacks the file we won't trash memory (or it's a corrupt file)
   - This can also be used to only load part of a data structure if a newer version that is bigger is found
6. We now read the data using `RIFF.Read(&Data2Load,sizeof(Data2Load));`
   - This will just read the data into the `Data2Load` structure.
   - You should default the `Data2Load` structure before starting the load, incase this block is not found in the file.
7. We now loop back to try to load the next data block.  If there isn't any data blocks available we will exit the loop.
8. Finally we close the file
