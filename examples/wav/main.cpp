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
 *    Paul Hutchinson (09 Aug 2021)
 *
 ******************************************************************************/

/*** HEADER FILES TO INCLUDE  ***/
#include "RIFF.h"
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/*** DEFINES                  ***/
//#define SCREEN_WIDTH    80
#define SCREEN_WIDTH    80
#define SCREEN_HEIGHT   20

/*** MACROS                   ***/

/*** TYPE DEFINITIONS         ***/
struct fmt
{
    uint16_t AudioFormat;
    uint16_t NumChannels;
    uint32_t SampleRate;
    uint32_t ByteRate;
    uint16_t BlockAlign;
    uint16_t BitsPerSample;
};

/*** FUNCTION PROTOTYPES      ***/
static void ReadFile(const char *Filename);
static void WriteFile(const char *Filename);
static void ShowHelp(void);

/*** VARIABLE DEFINITIONS     ***/
char Screen[SCREEN_HEIGHT][SCREEN_WIDTH+1];
char *Artist=NULL;
char *Genre=NULL;
char *Comment=NULL;
char *CDate=NULL;
char *Title=NULL;

int main(int argc,char **argv)
{
    try
    {
        if(argc<3)
        {
            ShowHelp();
            return 1;
        }
        if(strcmp(argv[1],"w")==0)
        {
            WriteFile(argv[2]);
        }
        else if(strcmp(argv[1],"r")==0)
        {
            ReadFile(argv[2]);
        }
        else
        {
            ShowHelp();
            return 1;
        }
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

void ShowHelp(void)
{
    printf("USAGE:\n");
    printf("    main r/w filename\n");
    printf("WHERE:\n");
    printf("    r/w -- A 'r' for read\n");
    printf("        -- A 'w' for write a new file\n");
    printf("    filename -- The filename to load / save\n");
}

void WriteFile(const char *Filename)
{
    class RIFF RIFF;
    struct fmt fmtblock;
    int16_t Sample;
    double Rad;
    int r;

    printf("Writing %s\n",Filename);

    /* Write the file */
    RIFF.Open(Filename,e_RIFFOpen_Write,"WAVE");

    fmtblock.AudioFormat=1;
    fmtblock.NumChannels=1;
    fmtblock.SampleRate=10000;
    fmtblock.ByteRate=20000;
    fmtblock.BlockAlign=2;
    fmtblock.BitsPerSample=16;
    RIFF.StartDataBlock("fmt ");
    RIFF.Write(&fmtblock,sizeof(fmtblock));
    RIFF.EndDataBlock();

    RIFF.StartGroup("INFO");
        RIFF.StartDataBlock("INAM");
        RIFF.Write("Sine wave",10);
        RIFF.EndDataBlock();
        RIFF.StartDataBlock("ICMT");
        RIFF.Write("Example file from Simple RIFF Lib",34);
        RIFF.EndDataBlock();
        RIFF.StartDataBlock("IGNR");
        RIFF.Write("Sound Clip",11);
        RIFF.EndDataBlock();
    RIFF.EndGroup();

    RIFF.StartDataBlock("data");
    Rad=0.0;
    for(r=0;r<10000;r++)
    {
        Rad+=.1;
        if(Rad>=M_PI*2)
            Rad=0;
        Sample=sin(Rad)*32767;
        RIFF.Write(&Sample,sizeof(Sample));
    }
    RIFF.EndDataBlock();

    RIFF.Close();

    printf("Done\n");
}

void ReadFile(const char *Filename)
{
    class RIFF RIFF;
    char ChunkID[5];
    uint32_t ChunkLen;
    struct fmt fmtblock;
    int16_t *Data;
    uint32_t DataSize;
    uint32_t Samples;
    uint32_t r;
    uint32_t c;
    const int16_t *Plot;
    int16_t Sample;
    int ScreenY;

    Data=NULL;
    DataSize=0;

    /* Read the file back in */
    RIFF.Open(Filename,e_RIFFOpen_Read,"WAVE");
    while(RIFF.ReadNextDataBlock(ChunkID,&ChunkLen))
    {
        if(strcmp(ChunkID,"fmt ")==0)
        {
            if(ChunkLen>sizeof(fmtblock))
                throw("Data error");
            RIFF.Read(&fmtblock,sizeof(fmtblock));
        }
        else if(strcmp(ChunkID,"data")==0)
        {
            if(fmtblock.BitsPerSample==16 && fmtblock.NumChannels<3)
            {
                DataSize=ChunkLen;
                Data=(int16_t *)malloc(DataSize);
                if(Data==NULL)
                    throw("Out of memory");
                RIFF.Read(Data,DataSize);
            }
        }
        else if(strcmp(ChunkID,"INFO")==0)
        {
            /* Read the sub data */
            RIFF.StartReadingGroup();
            while(RIFF.ReadNextDataBlock(ChunkID,&ChunkLen))
            {
                /* See https://www.recordingblogs.com/wiki/list-chunk-of-a-wave-file
                   for chunk types */
                if(strcmp(ChunkID,"IART")==0)
                {
                    Artist=(char *)malloc(ChunkLen+1);
                    if(Artist==NULL)
                        throw("Out of memory");
                    RIFF.Read(Artist,ChunkLen);
                    Artist[ChunkLen]=0;
                }
                else if(strcmp(ChunkID,"ICMT")==0)
                {
                    Comment=(char *)malloc(ChunkLen+1);
                    if(Comment==NULL)
                        throw("Out of memory");
                    RIFF.Read(Comment,ChunkLen);
                    Comment[ChunkLen]=0;
                }
                else if(strcmp(ChunkID,"ICRD")==0)
                {
                    CDate=(char *)malloc(ChunkLen+1);
                    if(CDate==NULL)
                        throw("Out of memory");
                    RIFF.Read(CDate,ChunkLen);
                    CDate[ChunkLen]=0;
                }
                else if(strcmp(ChunkID,"IGNR")==0)
                {
                    Genre=(char *)malloc(ChunkLen+1);
                    if(Genre==NULL)
                        throw("Out of memory");
                    RIFF.Read(Genre,ChunkLen);
                    Genre[ChunkLen]=0;
                }
                else if(strcmp(ChunkID,"INAM")==0)
                {
                    Title=(char *)malloc(ChunkLen+1);
                    if(Title==NULL)
                        throw("Out of memory");
                    RIFF.Read(Title,ChunkLen);
                    Title[ChunkLen]=0;
                }
            }
            RIFF.DoneReadingGroup();
        }
    }
    RIFF.Close();

    /* Display sum stuff about the file */
    printf("FORMAT:\n");
    printf("    AudioFormat:%d (",fmtblock.AudioFormat);
    if(fmtblock.AudioFormat==0x0001)
        printf("PCM");
    if(fmtblock.AudioFormat==0x0003)
        printf("IEE float");
    if(fmtblock.AudioFormat==0x0006)
        printf("8-bit A-law");
    if(fmtblock.AudioFormat==0x0007)
        printf("8-bit u-law");
    printf(")\n");
    printf("    NumChannels:%d\n",fmtblock.NumChannels);
    printf("    SampleRate:%d\n",fmtblock.SampleRate);
    printf("    ByteRate:%d\n",fmtblock.ByteRate);
    printf("    BlockAlign:%d\n",fmtblock.BlockAlign);
    printf("    BitsPerSample:%d\n",fmtblock.BitsPerSample);
    printf("INFO:\n");
    if(Title!=NULL)
        printf("    Title:%s\n",Title);
    if(Artist!=NULL)
        printf("    Artist:%s\n",Artist);
    if(CDate!=NULL)
        printf("    Creation date:%s\n",CDate);
    if(Genre!=NULL)
        printf("    Genre:%s\n",Genre);
    if(Comment!=NULL)
        printf("    Comment:%s\n",Comment);

    if(Data!=NULL)
    {
        printf("GRAPH:\n");
        Samples=DataSize/(fmtblock.BitsPerSample/8)/fmtblock.NumChannels;
        Plot=Data;

        for(ScreenY=0;ScreenY<SCREEN_HEIGHT;ScreenY++)
        {
            for(r=0;r<SCREEN_WIDTH;r++)
                Screen[ScreenY][r]=' ';
            Screen[ScreenY][r]=0;
        }
        for(r=0;r<SCREEN_WIDTH;r++)
        {
            Sample=Data[r*(Samples/SCREEN_WIDTH)];
            ScreenY=(-(((int32_t)Sample*(SCREEN_HEIGHT/2)/32768))+(SCREEN_HEIGHT/2));
            Screen[ScreenY][r]='#';
        }
        for(ScreenY=0;ScreenY<SCREEN_HEIGHT;ScreenY++)
            printf("%s\n",Screen[ScreenY]);
    }

    free(Data);
    free(Artist);
    free(Comment);
    free(CDate);
    free(Genre);
    free(Title);
}
