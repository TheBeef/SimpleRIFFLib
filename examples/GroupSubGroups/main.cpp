/*******************************************************************************
 * FILENAME: main.cpp
 *
 * PROJECT:
 *    Simple RIFF
 *
 * FILE DESCRIPTION:
 *    This file is an example of using groups within groups.
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
#include <stdlib.h>

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
        WriteFile("AscII.RIFF");

        /* Read them back in */
        ReadFile("AscII.RIFF");
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

    /* Write the file */
    RIFF.Open(Filename,e_RIFFOpen_Write,"ART ");

    RIFF.StartGroup("META");
        RIFF.StartDataBlock("AUTH");
        RIFF.Write("Row",4);
        RIFF.EndDataBlock();
        RIFF.StartDataBlock("DESC");
        RIFF.Write("AscII art downloaded from https://www.asciiart.eu",50);
        RIFF.EndDataBlock();
        RIFF.StartGroup("DATE");
            RIFF.StartDataBlock("TIME");
            RIFF.Write("4:00pm",7);
            RIFF.EndDataBlock();
            RIFF.StartDataBlock("DATE");
            RIFF.Write("08 Aug 2021",12);
            RIFF.EndDataBlock();
            RIFF.StartDataBlock("TZON");
            RIFF.Write("-5",3);
            RIFF.EndDataBlock();
        RIFF.EndGroup();
    RIFF.EndGroup();
    RIFF.StartDataBlock("BODY");
    RIFF.Write(" ____________________________________________________\n",54);
    RIFF.Write("T ================================================= |T\n",55);
    RIFF.Write("| ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|[L\n",56);
    RIFF.Write("| __________________________________________________[|\n",55);
    RIFF.Write("|I __==___________  ___________     .  ,. _ .   __  T|\n",55);
    RIFF.Write("||[_j  L_I_I_I_I_j  L_I_I_I_I_j    /|/V||(g/|   ==  l|\n",55);
    RIFF.Write("lI _______________________________  _____  _________I]\n",55);
    RIFF.Write(" |[__I_I_I_I_I_I_I_I_I_I_I_I_I_I_] [__I__] [_I_I_I_]|\n",54);
    RIFF.Write(" |[___I_I_I_I_I_I_I_I_I_I_I_I_L  I   ___   [_I_I_I_]|\n",54);
    RIFF.Write(" |[__I_I_I_I_I_I_I_I_I_I_I_I_I_L_I __I_]_  [_I_I_T ||\n",54);
    RIFF.Write(" |[___I_I_I_I_I_I_I_I_I_I_I_I____] [_I_I_] [___I_I_j|\n",54);
    RIFF.Write(" | [__I__I_________________I__L_]                   |\n",54);
    RIFF.Write(" |                                                  |  -Row\n",60);
    RIFF.Write(" l__________________________________________________j\n",54);
    RIFF.EndDataBlock();

    RIFF.Close();
}

void ReadFile(const char *Filename)
{
    class RIFF RIFF(true);
    char ChunkID[5];
    uint32_t ChunkLen;
    char *buffer;
    char buff[100];

    /* Read the file back in */
    RIFF.Open(Filename,e_RIFFOpen_Read,"ART ");
    while(RIFF.ReadNextDataBlock(ChunkID,&ChunkLen))
    {
        if(strcmp(ChunkID,"BODY")==0)
        {
            printf("Reading BODY\n");
            buffer=(char *)malloc(ChunkLen);
            if(buffer==NULL)
                throw("Failed to get needed memory");

            RIFF.Read(buffer,ChunkLen);
            printf("%s\n",buffer);

            free(buffer);
        }
        if(strcmp(ChunkID,"META")==0)
        {
            printf("Reading META data\n");

            /* Read the sub data */
            RIFF.StartReadingGroup();
            while(RIFF.ReadNextDataBlock(ChunkID,&ChunkLen))
            {
                if(strcmp(ChunkID,"AUTH")==0)
                {
                    if(ChunkLen>=sizeof(buff)-1)
                        throw("Read buffer too small");
                    RIFF.Read(buff,ChunkLen);
                    buff[ChunkLen+1]=0;
                    printf("    Author:%s\n",buff);
                }
                else if(strcmp(ChunkID,"DESC")==0)
                {
                    if(ChunkLen>=sizeof(buff)-1)
                        throw("Read buffer too small");
                    RIFF.Read(buff,ChunkLen);
                    buff[ChunkLen+1]=0;
                    printf("    Description:%s\n",buff);
                }
                else if(strcmp(ChunkID,"DATE")==0)
                {
                    printf("    Date info:\n");
                    RIFF.StartReadingGroup();
                    while(RIFF.ReadNextDataBlock(ChunkID,&ChunkLen))
                    {
                        if(strcmp(ChunkID,"TIME")==0)
                        {
                            if(ChunkLen>=sizeof(buff)-1)
                                throw("Read buffer too small");
                            RIFF.Read(buff,ChunkLen);
                            buff[ChunkLen+1]=0;
                            printf("        Time:%s\n",buff);
                        }
                        else if(strcmp(ChunkID,"DATE")==0)
                        {
                            if(ChunkLen>=sizeof(buff)-1)
                                throw("Read buffer too small");
                            RIFF.Read(buff,ChunkLen);
                            buff[ChunkLen+1]=0;
                            printf("        Date:%s\n",buff);
                        }
                        else if(strcmp(ChunkID,"TZON")==0)
                        {
                            if(ChunkLen>=sizeof(buff)-1)
                                throw("Read buffer too small");
                            RIFF.Read(buff,ChunkLen);
                            buff[ChunkLen+1]=0;
                            printf("        Timezone:%s\n",buff);
                        }
                    }
                    RIFF.DoneReadingGroup();
                }
            }
            RIFF.DoneReadingGroup();
        }
    }
    RIFF.Close();
}
