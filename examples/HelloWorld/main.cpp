/*******************************************************************************
 * FILENAME: main.cpp
 *
 * PROJECT:
 *    RIFF
 *
 * FILE DESCRIPTION:
 *    A very minimal basic example of using RIFF.
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
 *    Paul Hutchinson (17 Jul 2021)
 *
 ******************************************************************************/

/*** HEADER FILES TO INCLUDE  ***/
#include "RIFF.h"
#include <string.h>

/*** DEFINES                  ***/

/*** MACROS                   ***/

/*** TYPE DEFINITIONS         ***/
struct HelloWorld
{
    char Data[13];
};

/*** FUNCTION PROTOTYPES      ***/

/*** VARIABLE DEFINITIONS     ***/

int main(void)
{
    class RIFF RIFF;
    char ChunkID[5];
    uint32_t ChunkLen;
    struct HelloWorld WriteHello;
    struct HelloWorld ReadHello;

    try
    {
        strcpy(WriteHello.Data,"Hello World!");

        /* Write the file */
        RIFF.Open("HelloWorld.RIFF",e_RIFFOpen_Write,"HELO");
        RIFF.StartDataBlock("WORL");
        RIFF.Write(&WriteHello,sizeof(WriteHello));
        RIFF.EndDataBlock();
        RIFF.Close();

        /* Read the file back in */
        RIFF.Open("HelloWorld.RIFF",e_RIFFOpen_Read,"HELO");
        while(RIFF.ReadNextChunk(ChunkID,&ChunkLen))
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
