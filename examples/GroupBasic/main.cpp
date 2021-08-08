/*******************************************************************************
 * FILENAME: main.cpp
 *
 * PROJECT:
 *    RIFF
 *
 * FILE DESCRIPTION:
 *    This file is an example of using groups.
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
 *    Paul Hutchinson (18 Jul 2021)
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
struct Data
{
    int a;
    int b;
    int c;
};

struct SubData1
{
    int d;
    int e;
};

struct SubData2
{
    int f;
    int g;
};

struct Data2
{
    int h;
    int i;
    int j;
};

/*** FUNCTION PROTOTYPES      ***/
static void ReadFile(const char *Filename);
static void WriteFile(const char *Filename);

/*** VARIABLE DEFINITIONS     ***/

int main(void)
{
    try
    {
        /* Write 2 versions of the file */
        WriteFile("Data.RIFF");

        /* Read them back in */
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
    class RIFF RIFF(true);
    struct Data Data;
    struct SubData1 SubData1;
    struct SubData2 SubData2;
    struct Data2 Data2;

    Data.a=1;
    Data.b=2;
    Data.c=3;

    SubData1.d=4;
    SubData1.e=5;

    SubData2.f=6;
    SubData2.g=7;

    Data2.h=8;
    Data2.i=9;
    Data2.j=10;

    /* Write the file */
    RIFF.Open(Filename,e_RIFFOpen_Write,"GRUP");

    /* DATA Group */
    RIFF.StartGroup("DATA");
        /* SUB1 structure */
        RIFF.StartDataBlock("SUB1");
        RIFF.Write(&SubData1,sizeof(SubData1));
        RIFF.EndDataBlock();
        /* SUB2 structure */
        RIFF.StartDataBlock("SUB2");
        RIFF.Write(&SubData2,sizeof(SubData2));
        RIFF.EndDataBlock();
    RIFF.EndGroup();

    RIFF.StartDataBlock("DAT2");
    RIFF.Write(&Data2,sizeof(Data2));
    RIFF.EndDataBlock();

    RIFF.Close();
}

void ReadFile(const char *Filename)
{
    class RIFF RIFF(true);
    char ChunkID[5];
    uint32_t ChunkLen;
    struct Data Data;
    struct SubData1 SubData1;
    struct SubData2 SubData2;
    struct Data2 Data2;

    /* Read the file back in */
    RIFF.Open(Filename,e_RIFFOpen_Read,"GRUP");
    while(RIFF.ReadNextChunk(ChunkID,&ChunkLen))
    {
        if(strcmp(ChunkID,"DATA")==0)
        {
            printf("Reading DATA Group\n");

            /* Read the sub data */
            RIFF.StartReadingGroup();
            while(RIFF.ReadNextChunk(ChunkID,&ChunkLen))
            {
                if(strcmp(ChunkID,"SUB1")==0)
                {
                    printf("    Reading SUB1\n");
                    if(ChunkLen!=sizeof(SubData1))
                        throw("Data error");
                    RIFF.Read(&SubData1,sizeof(SubData1));
                }
                else if(strcmp(ChunkID,"SUB2")==0)
                {
                    printf("    Reading SUB2\n");
                    if(ChunkLen!=sizeof(SubData2))
                        throw("Data error");
                    RIFF.Read(&SubData2,sizeof(SubData2));
                }
            }
            RIFF.DoneReadingGroup();
        }
        else if(strcmp(ChunkID,"DAT2")==0)
        {
            printf("Reading DAT2\n");
            if(ChunkLen!=sizeof(Data2))
                throw("Data error");
            RIFF.Read(&Data2,sizeof(Data2));
        }
    }
    RIFF.Close();
}
