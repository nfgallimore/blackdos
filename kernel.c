/* ACADEMIC INTEGRITY PLEDGE                                              */
/*                                                                        */
/* - I have not used source code obtained from another student nor        */
/*   any other unauthorized source, either modified or unmodified.        */
/*                                                                        */
/* - All source code and documentation used in my program is either       */
/*   my original work or was derived by me from the source code           */
/*   published in the textbook for this course or presented in            */
/*   class.                                                               */
/*                                                                        */
/* - I have not discussed coding details about this project with          */
/*   anyone other than my instructor. I understand that I may discuss     */
/*   the concepts of this program with other students and that another    */
/*   student may help me debug my program so long as neither of us        */
/*   writes anything during the discussion or modifies any computer       */
/*   file during the discussion.                                          */
/*                                                                        */
/* - I have violated neither the spirit nor letter of these restrictions. */
/*                                                                        */
/*                                                                        */
/*                                                                        */
/* Signed: Nicholas Gallimore Date: January 31, 2019       */
/*                                                                        */
/*                                                                        */
/* 3460:4/526 BlackDOS2020 kernel, Version 1.01, Spring 2019.             */
#include <stdlib.h>
#include <math.h>

void handleInterrupt21(int, int, int, int);
void printLogo();
void readString(char*);
void printString(char*, int);
void readInt(int*);
void writeInt(int);
void error(int);
void readFile(char* fname, char* buffer, int* size);
void deleteFile(char* filename);
void writeFile(char* src, char* tar, int* numSectors);
void runProgram(char* programName, int segments);
void stop();
int getNthDigit(int, int);
int getNumDigits(int);

void main()
{
    char buffer[512];
    makeInterrupt21();
    interrupt(33,2,buffer,258,0);
    interrupt(33,12,buffer[0]+1,buffer[1]+1,0);
    printLogo();
    interrupt(33,4,"Shell\0",2,0);
    interrupt(33,0,"Bad or missing command interpreter.\r\n\0",0,0);
    while (1);
}

void printLogo()
{
    interrupt(33, 0, "       ___   `._   ____  _            _    _____   ____   _____ \r\n\0", 0, 0);
    interrupt(33, 0, "      /   \\__/__> |  _ \\| |          | |  |  __ \\ / __ \\ / ____|\r\n\0", 0, 0);
    interrupt(33, 0, "     /_  \\  _/    | |_) | | __ _  ___| | _| |  | | |  | | (___ \r\n\0", 0, 0);
    interrupt(33, 0, "    // \\ /./      |  _ <| |/ _` |/ __| |/ / |  | | |  | |\\___ \\ \r\n\0", 0, 0);
    interrupt(33, 0, "   //   \\\\        | |_) | | (_| | (__|   <| |__| | |__| |____) |\r\n\0", 0, 0);
    interrupt(33, 0, "._/'     `\\.      |____/|_|\\__,_|\\___|_|\\_\\_____/ \\____/|_____/\r\n\0", 0, 0);
    interrupt(33, 0, " BlackDOS2020 v. 1.01, c. 2018. Based on a project by M. Black. \r\n\0", 0, 0);
    interrupt(33, 0, " Author: Nick Gallimore\r\n\r\n\0", 0, 0);
}

void readString(char* c)
{
    int count = 0;
    char* letter;

    while(1)
    {
        letter = interrupt(22, 0, 0, 0, 0);
        
        if (letter == 13)
        {
        	break;
        }
        else if (letter == 8 && count > 0) 
        {
            --count;
            interrupt(16, 14 * 256 + 8, 0, 0, 0);
        }
        else
        {
            c[count++] = letter;
            interrupt(16, 14 * 256 + letter, 0, 0, 0);
        }
    }

    c[count] = '\0';
}

void readInt(int* n)
{
    char input[6];
    int i = 0;
    int tmp = 0;

    readString(input);

    for (i = 0; input[i] != '\0'; ++i)
    {

        tmp = tmp * 10 + (input[i] - '0');
    }

    *n = tmp;
}

/* Prints a string to the screen when `d` is equal to 0 */
/* Prints a string to the printer when `d` is equal to 1 */
void printString(char* c, int d)
{
    if (d == 1)
    {
        while (*c != '\0')
        {
            interrupt(23, *c, 0, 0, 0);
            ++c;
        }
    }
    else
    {
        while (*c != '\0')
        {
            interrupt(16, 14 * 256 + *c, 0, 0, 0);
            ++c;
        }
    }

}

void writeInt(int x, int opt)
{    
    char str[6];
    int numDigits;
    int i;

    if (x < 0)
    {
    	return;
    }

    if (x == 0)
    {
        printString("0", opt);
        return;
    }

    numDigits = getNumDigits(x);

    for (i = 0; i < numDigits; i++)
    {
        str[i] = getNthDigit(x, numDigits - i - 1) + '0';
    }

    while (numDigits < 5)
    {
        str[numDigits] = '\0';
        numDigits++;
    }

    str[5] = '\0';

    printString(str, opt);
}

void readSector(char* buffer, int sector)
{
    int relSecNo = mod(sector, 18) + 1;
    int headNo = mod(div(sector, 18), 2);
    int trackNo = div(sector, 36);

    interrupt(19, 513, buffer, trackNo * 256 + relSecNo, headNo * 256);
}

void writeSector(char* buffer, int sector)
{
    int relSecNo = mod(sector, 18) + 1;
    int headNo = mod(div(sector, 18), 2);
    int trackNo = div(sector, 36);

    interrupt(19, 769, buffer, trackNo * 256 + relSecNo, headNo * 256);
}

void clearScreen(int bg, int fg)
{
    int i;

    for (i = 0; i < 24; ++i)
    {
        printString("\r\n", 0);
    }

    interrupt(16, 512, 0, 0, 0);

    if (bg > 0 && fg > 0)
    {
        if (bg > 8 || fg > 16)
        {
            return;
        }
        interrupt(16, 1536, 4096 * (bg - 1) + 256 * (fg - 1), 0, 6223);
    }
}

void readFile(char* fname, char* buffer, int* size)
{
    char directory[512];
    int i, j, fileFound;

    readSector(directory, 257);

    fileFound = 0;
    *size = 0;

    // loop through all 16 files in directory
    for (i = 0; i < 512; i += 32)
    {
        // check if first 8 bytes of this file match the fname
        for (j = 0; j < 8; ++j)
        {
            if (directory[i + j] == fname[j])
            {
                if (j == 7 || fname[j] == '\0')
                {
                    fileFound = 1;
                }
            }
            else
            {
                break;
            }
        }
        if (fileFound == 1)
        {
            break;
        }
    }
    
    if (fileFound == 0)
    {
        interrupt(33, 15, 0, 0, 0);
    }


    // load remaining 24 bytes into buffer
    for (j = i + 8; j < 32; ++j)
    {
        if (directory[j + i] != 0) 
        {
            readSector(buffer, directory[j + i]);       

            buffer += 512;
            *size = *size + 1;
        }
        else
        {
            break; // if it is 0 then we can assume the rest are 0
        }
    }   
}

void deleteFile(char* fname)
{

    char directory[512], map[512];
    int i, j, fileFound, directoryIndex;

    readSector(directory, 257);
    readSector(map, 256);

    fileFound = 0;

    // find filename
    for (i = 0; i < 512; i += 32)
    {
        // check if first 8 bytes of this file match the fname
        for (j = 0; j < 8; ++j)
        {
            if (directory[i + j] == fname[j])
            {
                if (j == 7 || fname[j] == '\0')
                {
                    fileFound = 1;
                }
            }
            else
            {
                break;
            }
        }
        if (fileFound == 1)
        {
            directoryIndex = i;
            break;
        }
    }

    if (fileFound == 0)
    {
        interrupt(33, 15, 0, 0, 0);
    }

    directory[directoryIndex] = 0;

    for(i = 8; i < 32; ++i)
    {
        map[directory[i]] = 0;
    }

    writeSector(directory, 257);
    writeSector(map, 256);
}

void writeFile(char* name, char* buffer, int* numSectors)
{
    char directory[512], map[512], nextPartOfBuffer[512];
    int i, j, k, l, directoryFreeSpaceIndex, freeSpaceFound;

    readSector(directory, 257);
    readSector(map, 256); 

    freeSpaceFound = 0;

    // loop through directory to search if name is used
    for (i = 0; i < 512; i += 32)
    {
        // if is free space, record index
        if (directory[i] == 0)
        {
            directoryFreeSpaceIndex = i;
            freeSpaceFound = 1;
            continue;
        }

        // check if first 8 bytes of this file match the name
        for (j = 0; j < 8; ++j)
        {
            // if directory entry has same name as proposed new name
            if (directory[i + j] == name[j])
            {
                if (j == 7 || name[j] == '\0')
                {
                    interrupt(33, 15, 1, 0, 0);
                    return;
                }
            }
            else
            {
                break;
            }
        }
    }

    // if no directory entries left
    if (freeSpaceFound == 0)
    {
        interrupt(33, 15, 2, 0, 0);
        return;
    }

    // copy name to free directory entry
    for (i = 0; name[i] != '\0'; ++i)
    {
        directory[directoryFreeSpaceIndex + i] = name[i];
    }
    while (i < 8)
    {
        directory[directoryFreeSpaceIndex + i] = 0;
        ++i;
    }
    
    // update map for each sector of the file
    for (i = 0; i < numSectors; ++i)
    {
        // search through map to find a free sector
        for (j = 0; j < 512; ++j)
        {
            // if its free, use it
            if (map[j] == 0)
            {
                map[j] = 255;
                directory[directoryFreeSpaceIndex + 8 + i] = j;

                // write the data to the sector
                for (k = 0; k < 512; ++k)
                {
                    nextPartOfBuffer[k] = buffer[k + 512 * i];
                }
                writeSector(nextPartOfBuffer, j);

                break;
            }
        }

        // disk is full
        if (j == 512)
        {
            interrupt(33, 15, 2, 0, 0);
            return;
        }
    }

    // fill remaining directory sector entries with 0
    for (i = numSectors + 8; i < 32; ++i)
    {
        directory[directoryFreeSpaceIndex + i] = 0;
    }

    writeSector(directory, 257);
    writeSector(map, 256);
}

void runProgram(char* programName, int segment)
{
    char buffer[13312];
    int i, size;

    readFile(programName, buffer, &size);
    segment = segment << 12;
    size = size << 9;

    for (i = 0; i < size; i++)
    {
        putInMemory(segment, i, buffer[i]);
    }
    launchProgram(segment);
}

void stop()
{
    launchProgram(8192);
}

void error(int bx)
{
    char errMsg0[18], errMsg1[17], errMsg2[13], errMsg3[17];

    errMsg0[0] = 70; errMsg0[1] = 105; errMsg0[2] = 108; errMsg0[3] = 101;
    errMsg0[4] = 32; errMsg0[5] = 110; errMsg0[6] = 111; errMsg0[7] = 116;
    errMsg0[8] = 32; errMsg0[9] = 102; errMsg0[10] = 111; errMsg0[11] = 117;
    errMsg0[12] = 110; errMsg0[13] = 100; errMsg0[14] = 46; errMsg0[15] = 13;
    errMsg0[16] = 10; errMsg0[17] = 0;
    errMsg1[0] = 66; errMsg1[1] = 97; errMsg1[2] = 100; errMsg1[3] = 32;
    errMsg1[4] = 102; errMsg1[5] = 105; errMsg1[6] = 108; errMsg1[7] = 101;
    errMsg1[8] = 32; errMsg1[9] = 110; errMsg1[10] = 97; errMsg1[11] = 109;
    errMsg1[12] = 101; errMsg1[13] = 46; errMsg1[14] = 13; errMsg1[15] = 10;
    errMsg1[16] = 0;
    errMsg2[0] = 68; errMsg2[1] = 105; errMsg2[2] = 115; errMsg2[3] = 107;
    errMsg2[4] = 32; errMsg2[5] = 102; errMsg2[6] = 117; errMsg2[7] = 108;
    errMsg2[8] = 108; errMsg2[9] = 46; errMsg2[10] = 13; errMsg2[11] = 10;
    errMsg2[12] = 0;
    errMsg3[0] = 71; errMsg3[1] = 101; errMsg3[2] = 110; errMsg3[3] = 101;
    errMsg3[4] = 114; errMsg3[5] = 97; errMsg3[6] = 108; errMsg3[7] = 32;
    errMsg3[8] = 101; errMsg3[9] = 114; errMsg3[10] = 114; errMsg3[11] = 111;
    errMsg3[12] = 114; errMsg3[13] = 46; errMsg3[14] = 13; errMsg3[15] = 10;
    errMsg3[16] = 0;

    switch(bx) {
    case 0: printString(errMsg0); break;
    case 1: printString(errMsg1); break;
    case 2: printString(errMsg2); break;
    default: printString(errMsg3);
    }
}


void handleInterrupt21(int ax, int bx, int cx, int dx)
{
    switch(ax) 
    {  
        case 0: 
            printString(bx, cx); 
            break;

        case 1:
            readString(bx);
            break; 

        case 2:
            readSector(bx, cx);
            break;

        case 3:
            readFile(bx, cx, dx);
            break;

        case 4:
            runProgram(bx, cx);
            break;

        case 5:
            stop();
            break;

        case 6:
            writeSector(bx, cx);
            break;

        case 7: 
            deleteFile(bx);
            break;

        case 8:
            writeFile(bx, cx, dx);
            break;

        case 9: 

        case 10:

        case 11: 
            interrupt(25,0,0,0,0);
            break;
            
        case 12:
            clearScreen(bx, cx);
            break;

        case 13:
            writeInt(bx, cx);
            break;

        case 14:
            readInt(bx);
            break;

        case 15:
            error(bx);
            break;

        default: 
            printString("General BlackDOS error.\r\n\0", 0);
    }
}

/* Helper functions */

int mod(int a, int b)
{
    int x = a;
    while (x >= b)
    {
        x = x - b;
    }
    return x;
}

int div(int a, int b)
{
    int q = 0;
    while (q * b <= a)
    {
        q++;
    }
    return (q - 1);
}

// Can reduce to O(logn) operations
int pow(int base, int exponent)
{
    int result = 1;
    while (exponent > 0)
    {
        result = base * result;
        --exponent;
    }
    return result;
}

/* starting from right, if x is 12345, then when n = 0 return 5 */
/* The nth digit is (the remainder of dividing by 10^n) divided by 10^(n-1) */
int getNthDigit(int x, int n)
{
    return div(mod(x, pow(10, n + 1)), pow(10, n));
}

int getNumDigits(int x)
{
    if (x > 9999)
    {
        return 5;
    }
    else if (x > 999)
    {
        return 4;
    }
    else if (x > 99)
    {
        return 3;
    }
    else if (x > 9)
    {
        return 2;
    }
    return 1;
}
