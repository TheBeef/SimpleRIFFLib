# Skip
This is a example of how the library skips unknown chunks including groups.

# Compile
You can use the build.sh script to compile the example or manualy compile with:

```
g++ main.cpp ../../src/RIFF.cpp -I ../../src -o main
```

# Unknown chunks
```
while(RIFF.ReadNextDataBlock(ChunkID,&ChunkLen))
{
    if(strcmp(ChunkID,"TEST")==0)
    {
        /* Read data */
    }
}
```
When ReadNextDataBlock() returns a chunkID you normally do a strcmp() on it to load your data.  If you don't know how to
load the chunk you just call ReadNextDataBlock() and it will skip to the next block.

This way you can add new data types to your file and still have older versions of your program be able to load the newer
file.
