# Wav
This is a real world example of using the library.  It loads / creates a .wav file.

.wav files are RIFF files from Microsoft.

This example can create a .wav with a sine wave in it.

You can then load this .wav file back into it and it will display info about the file.


# Compile
You can use the build.sh script to compile the example or manualy compile with:

```
g++ main.cpp ../../src/RIFF.cpp -I ../../src -o main
```
