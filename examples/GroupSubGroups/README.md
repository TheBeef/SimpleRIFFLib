# Group sub groups
This example shows using groups inside of other groups (nesting).

# Compile
You can use the build.sh script to compile the example or manualy compile with:

```
g++ main.cpp ../../src/RIFF.cpp -I ../../src -o main
```

## Writing
```
RIFF.StartGroup("META");
    RIFF.StartDataBlock("AUTH");
    RIFF.Write("Row",4);
    RIFF.EndDataBlock();
    RIFF.StartGroup("DATE");
        RIFF.StartDataBlock("TIME");
        RIFF.Write("4:00pm",7);
        RIFF.EndDataBlock();
    RIFF.EndGroup();
RIFF.EndGroup();
```

This starts a group named "META".  In side that group it writes a data block named "AUTH".

It then starts a new group inside of "META" named "DATE".  It writes a data block named "TIME" inside the sub group.

## Reading
```
if(strcmp(ChunkID,"META")==0)
{
    RIFF.StartReadingGroup();
    while(RIFF.ReadNextDataBlock(ChunkID,&ChunkLen))
    {
        else if(strcmp(ChunkID,"DATE")==0)
        {
            RIFF.StartReadingGroup();
            while(RIFF.ReadNextDataBlock(ChunkID,&ChunkLen))
            {
                if(strcmp(ChunkID,"TIME")==0)
                {
                    /* Read data */
                }
            }
        }
    }
}
```
When reading the groups back in you call `StartReadingGroup()` when you start reading a group.  Then you start reading the data blocks
in the group.  If you find another group then then you call `StartReadingGroup()` again and processing any data blocks you find.

