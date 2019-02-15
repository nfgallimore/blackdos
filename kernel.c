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

#define TRUE = 1
#define FALSE = 0

void handleInterrupt21(int, int, int, int);
void printLogo();
void readString(char*);
void printString(char*, int);
void readInt(int*);
void writeInt(int);
void writePositiveInt(int);
void writeNegativeInt(int);
int getNthDigit(int, int);
int getNumDigits(int);

/* Mad Lib kernel.c - c. 2018 O'Neil */
void main()
{
    char buffer[512]; int i;
    makeInterrupt21();
    for (i = 0; i < 512; i++) buffer[i] = 0;
    buffer[0] = 4;
    buffer[1] = 15;
    interrupt(33,6,buffer,258,0);
    interrupt(33,12,buffer[0]+1,buffer[1]+1,0);
    printLogo();
    interrupt(33,2,buffer,30,0);
    interrupt(33,0,buffer,0,0);
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
            printString(&letter, 0);
        }
    }

    c[count] = '\0';
    printString("\r\n\0");
    printString(c, 0);
    printString("\r\n\0");
}

void readInt(int* n)
{
    char input[6];
    int i = 0;

    readString(input);

    for (i = 0; input[i] != '\0'; ++i)
    {
        *n = *n * 10 + (input[i] - '0');
    }
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

    for(i = 0; i < 24; ++i)
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

        case 4: 

        case 5:

        case 6:
            writeSector(bx, cx);
            break;

        case 7: 

        case 8: 

        case 9: 

        case 10:

        case 11: 

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

        default: 
            printString("General BlackDOS error.\r\n\0", 0);
    }
}
