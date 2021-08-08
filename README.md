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

