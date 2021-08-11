# Hello World
This demo's a very basic usage of the library.

It makes a HelloWorld.RIFF file and then loads it back in.

It only reads/writes one data block and is about as basic as you can get.

# Compile
You can use the build.sh script to compile the example or manualy compile with:

```
g++ main.cpp ../../src/RIFF.cpp -I ../../src -o main
```

# Write
```
/* Write the file */
RIFF.Open("HelloWorld.RIFF",e_RIFFOpen_Write,"HELO");
RIFF.StartDataBlock("WORL");
RIFF.Write(&WriteHello,sizeof(WriteHello));
RIFF.EndDataBlock();
RIFF.Close();
```
1. `RIFF.Open("HelloWorld.RIFF",e_RIFFOpen_Write,"HELO");`
   - This will open the "HelloWorld.RIFF" file and give it a filetype of "HELO".
2. `RIFF.StartDataBlock("WORL");`
   - Starts a new block of data named "WORL"
   - After you open a new block you call Write() to add data to the block.  All Write()'s will be appended to the block until EndDataBlock() is called
3. `RIFF.Write(&WriteHello,sizeof(WriteHello));`
   - Write out the `struct HelloWorld` structure
4. `RIFF.EndDataBlock();`
   - Close the "WORL" data block.
   - The number of bytes in this block will be recorded
5. At this point you could start another data block with `StartDataBlock()` if you wanted
6. `RIFF.Close();`
   - Close the file and flush any data

# Read
```
RIFF.Open("HelloWorld.RIFF",e_RIFFOpen_Read,"HELO");
while(RIFF.ReadNextDataBlock(ChunkID,&ChunkLen))
{
    if(strcmp(ChunkID,"WORL")==0)
    {
        printf("Reading WORL\n");
        if(ChunkLen!=sizeof(ReadHello))
            throw("Data error");
        RIFF.Read(&ReadHello,sizeof(ReadHello));
        ReadHello.Data[12]=0;   // Make sure it's a string
        printf("    \"%s\"\n",ReadHello.Data);
    }
}
RIFF.Close();
```
1. `RIFF.Open("HelloWorld.RIFF",e_RIFFOpen_Read,"HELO");`
   - Open the "HelloWorld.RIFF" file for reading.  The lib will check if the file type is "HELO" and throw an error if not
2. `while(RIFF.ReadNextDataBlock(ChunkID,&ChunkLen))`
   - This readys the next data block in the file for loading.
   - You need to check if you understand the `ChunkID` or not and if so load the data
3. `if(strcmp(ChunkID,"WORL")==0)`
   - This check to see if this chunk ID is for the "WORL" block
   - If it is then we load the data block, other wize we just skip this block (by getting to the end of the while() loop and calling ReadNextDataBlock() again
4. `if(ChunkLen!=sizeof(ReadHello))`
   - We check to make sure we don't have a bad file and to make sure we aren't going to crash if so
5. `RIFF.Read(&ReadHello,sizeof(ReadHello));`
   - Read the data into a `struct HelloWorld` structure
6. At this point we are at the end of the while loop.  We will do another ReadNextDataBlock() until it returns false.
7. `RIFF.Close();`
   - Close the file
