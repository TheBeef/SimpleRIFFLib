# GroupBasic
This is an example that uses groups.  Groups are where you group serveral data blocks together.

For example you might want to place all the META data under a group.

# Compile
You can use the build.sh script to compile the example or manualy compile with:

```
g++ main.cpp ../../src/RIFF.cpp -I ../../src -o main
```

## Writing
The `StartGroup()` function starts a new group.  When you call it you provide a group ID (a 4 letter, AscII name the same as all other ID's).

You then add data the normal way (`StartDataBlock()`, `Write()`, `EndDataBlock()`).

When you are done you call `EndGroup()` to finalize the group.

You can also start groups inside of groups to make a hierarchy.


## Reading
When you are reading groups back out you need to call `StartReadingGroup()` to let the library know you are going to read the group instead
of skipping it.

You do this by using strcmp() to recognize the group ID, then calling `StartReadingGroup()` and entering a new `ReadNextDataBlock()` block.

So for example:
```C++
    RIFF.Open(Filename,e_RIFFOpen_Read,"GRUP");
    while(RIFF.ReadNextDataBlock(ChunkID,&ChunkLen))
    {
        if(strcmp(ChunkID,"DATA")==0)
        {
            RIFF.StartReadingGroup();
            while(RIFF.ReadNextDataBlock(ChunkID,&ChunkLen))
            {
                /* ... normal strcmp() code here */
            }
            RIFF.DoneReadingGroup();
        }
    }
    RIFF.Close();
```

When you are done with the group you call `DoneReadingGroup()` to skip any unread data.

## Extra
The `ReadNextDataBlock()` function takes an extra arg named `DataType`:
```
bool ReadNextDataBlock(char *ChunkID,uint32_t *ChunkSize,e_RIFFDataType &DataType);
```

You can use this to tell if the ChunkID that was just read is normal data ID or a group ID.

This will be set to one of the following:

| Value                | Description |
| -------------------- | ----------- |
| e_RIFFData_DataBlock | Data block that was made with StartDataBlock() |
| e_RIFFData_Group     | Group that was made with StartGroup() |

This is only needed if you have groups and data block that use the same ID's
