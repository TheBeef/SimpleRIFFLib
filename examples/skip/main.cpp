/*******************************************************************************
 * FILENAME: main.cpp
 *
 * PROJECT:
 *    RIFF
 *
 * FILE DESCRIPTION:
 *    This file is an example of skiping unknown groups and data block.
 *
 * COPYRIGHT:
 *    Copyright 2021 Paul Hutchinson
 *
 *    Permission is hereby granted, free of charge, to any person obtaining a
 *    copy of this software and associated documentation files (the "Software"),
 *    to deal in the Software without restriction, including without limitation
 *    the rights to use, copy, modify, merge, publish, distribute, sublicense,
 *    and/or sell copies of the Software, and to permit persons to whom the
 *    Software is furnished to do so, subject to the following conditions:
 *
 *    The above copyright notice and this permission notice shall be included
 *    in all copies or substantial portions of the Software.
 *
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *    IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 *    CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 *    TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 *    SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * CREATED BY:
 *    Paul Hutchinson (11 Aug 2021)
 *
 ******************************************************************************/

/*** HEADER FILES TO INCLUDE  ***/
#include "RIFF.h"
#include <string.h>
#include <stdint.h>

/*** DEFINES                  ***/

/*** MACROS                   ***/
#define SAFTY_STRINGIZE(x) (x)[sizeof(x)-1]=0

/*** TYPE DEFINITIONS         ***/

/*** FUNCTION PROTOTYPES      ***/
static void ReadFile(const char *Filename);
static void WriteFile(const char *Filename);

/*** VARIABLE DEFINITIONS     ***/

int main(void)
{
    try
    {
        WriteFile("Data.RIFF");
        ReadFile("Data.RIFF");
    }
    catch(e_RIFFErrorType err)
    {
        printf("Error:%d, %s\n",err,RIFF::Error2Str(err));
    }
    catch(const char *Err)
    {
        printf("ERR:%s\n",Err);
    }

    return 0;
}

void WriteFile(const char *Filename)
{
    class RIFF RIFF;

    /* Write the file */
    RIFF.Open(Filename,e_RIFFOpen_Write,"EXAM");

    /* DATA Group */
    RIFF.StartGroup("DATA");
        /* SUB1 structure */
        RIFF.StartDataBlock("SUB1");
        RIFF.Write("Test Block1",11);
        RIFF.EndDataBlock();
        /* SUB2 structure */
        RIFF.StartDataBlock("SUB2");
        RIFF.Write("Test Block2",11);
        RIFF.EndDataBlock();
    RIFF.EndGroup();

    RIFF.StartDataBlock("MORE");
    RIFF.Write("More data here",14);
    RIFF.EndDataBlock();

    RIFF.StartDataBlock("JUNK");
    RIFF.Write("This is junk",12);
    RIFF.EndDataBlock();

    RIFF.StartDataBlock("SKIP");
    RIFF.Write("The skip me block",17);
    RIFF.EndDataBlock();

    RIFF.Close();
}

void ReadFile(const char *Filename)
{
    class RIFF RIFF;
    char ChunkID[5];
    uint32_t ChunkLen;
    char buff[100];

    /* Read the file back in */
    RIFF.Open(Filename,e_RIFFOpen_Read,"EXAM");
    while(RIFF.ReadNextDataBlock(ChunkID,&ChunkLen))
    {
        if(strcmp(ChunkID,"MORE")==0)
        {
            printf("Loading MORE block\n");
            if(ChunkLen>sizeof(buff)-1)
                throw("Data error");
            RIFF.Read(buff,ChunkLen);
        }
        else
        {
            printf("Skipping \"%s\"\n",ChunkID);
        }
    }
    RIFF.Close();
}
