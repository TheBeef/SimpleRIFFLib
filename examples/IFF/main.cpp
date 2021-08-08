/*******************************************************************************
 * FILENAME: main.cpp
 *
 * PROJECT:
 *    RIFF
 *
 * FILE DESCRIPTION:
 *    This is an example of reading/writing IFF and RIFF files.
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
struct UserData
{
    char Name[50];
    char Password[50];
    uint8_t Age;
};

struct BankData
{
    uint32_t BankAccountNumber;
    int32_t Balance;
    char CreditRating;
};

/*** FUNCTION PROTOTYPES      ***/
static void ReadFile(const char *Filename,bool IFFMode);
static void WriteFile(const char *Filename,bool IFFMode);

/*** VARIABLE DEFINITIONS     ***/

int main(void)
{
    try
    {
        /* Write 2 versions of the file */
        WriteFile("Data.IFF",true);
        WriteFile("Data.RIFF",false);

        /* Read them back in */
        printf("*** IFF Version:\n");
        ReadFile("Data.IFF",true);

        printf("*** RIFF Version:\n");
        ReadFile("Data.RIFF",false);
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

void WriteFile(const char *Filename,bool IFFMode)
{
    class RIFF RIFF;
    struct UserData User;
    struct BankData Bank;

    strncpy(User.Name,"User1",sizeof(User.Name));
    strncpy(User.Password,"ridiculousmodel",sizeof(User.Password));
    User.Age=21;

    Bank.BankAccountNumber=123456;
    Bank.Balance=500;
    Bank.CreditRating='C';

    RIFF.SetIFFMode(IFFMode);

    /* Write the file */
    RIFF.Open(Filename,e_RIFFOpen_Write,"TEST");

    /* User chunk */
    RIFF.StartDataBlock("USER");
    RIFF.Write(&User,sizeof(User));
    RIFF.EndDataBlock();

    /* Bank chunk */
    RIFF.StartDataBlock("BANK");
    RIFF.Write(&Bank,sizeof(Bank));
    RIFF.EndDataBlock();

    RIFF.Close();
}

void ReadFile(const char *Filename,bool IFFMode)
{
    class RIFF RIFF;
    char ChunkID[5];
    uint32_t ChunkLen;
    struct UserData User;
    struct BankData Bank;

    RIFF.SetIFFMode(IFFMode);

    /* Read the file back in */
    RIFF.Open(Filename,e_RIFFOpen_Read,"TEST");
    while(RIFF.ReadNextChunk(ChunkID,&ChunkLen))
    {
        if(strcmp(ChunkID,"USER")==0)
        {
            printf("Reading USER\n");
            if(ChunkLen!=sizeof(User))
                throw("Data error");
            RIFF.Read(&User,sizeof(User));

            /* Print the data */
            SAFTY_STRINGIZE(User.Name);
            SAFTY_STRINGIZE(User.Password);

            printf("    Name:%s\n",User.Name);
            printf("    Password:%s\n",User.Password);
            printf("    Age:%d\n",User.Age);
        }

        if(strcmp(ChunkID,"BANK")==0)
        {
            printf("Reading BANK\n");
            if(ChunkLen!=sizeof(Bank))
                throw("Data error");
            RIFF.Read(&Bank,sizeof(Bank));

            /* Print the data */
            printf("    Account #%d\n",Bank.BankAccountNumber);
            printf("    Balance $%d\n",Bank.Balance);
            printf("    CreditRating:%c\n",Bank.CreditRating);
        }
    }
    RIFF.Close();
}
