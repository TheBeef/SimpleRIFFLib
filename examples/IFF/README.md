# IFF
This examples shows that the library can also read/write IFF files as well as RIFF.

When you construct the RIFF class, if you pass true as the arg then it will read/write IFF files.

You can also `SetIFFMode()` function to set the mode after the instance has been created.

# Compile
You can use the build.sh script to compile the example or manualy compile with:

```
g++ main.cpp ../../src/RIFF.cpp -I ../../src -o main
```
