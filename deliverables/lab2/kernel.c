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
    char food[25], adjective[25], color[25], animal[25];
    int temp;
    makeInterrupt21();
    printLogo();
    interrupt(33,0,"\r\nWelcome to the Mad Libs kernel.\r\n\0",0,0);
    interrupt(33,0,"Enter a food: \0",0,0);
    interrupt(33,1,food,0,0);
    temp = 0;
    while ((temp < 100) || (temp > 120)) {
        interrupt(33,0,"Enter a number between 100 and 120: \0",0,0);
        interrupt(33,14,&temp,0,0);
    }
    interrupt(33,0,"Enter an adjective: \0",0,0);
    interrupt(33,1,adjective,0,0);
    interrupt(33,0,"Enter a color: \0",0,0);
    interrupt(33,1,color,0,0);
    interrupt(33,0,"Enter an animal: \0",0,0);
    interrupt(33,1,animal,0,0);
    interrupt(33,0,"Your note is on the printer, go get it.\r\n\0",0,0);
    interrupt(33,0,"Dear Professor O\'Neil,\r\n\0",1,0);
    interrupt(33,0,"\r\nI am so sorry that I am unable to turn in my program at this time.\r\n\0",1,0);
    interrupt(33,0,"First, I ate a rotten \0",1,0);
    interrupt(33,0,food,1,0);
    interrupt(33,0,", which made me turn \0",1,0);
    interrupt(33,0,color,1,0);
    interrupt(33,0," and extremely ill.\r\n\0",1,0);
    interrupt(33,0,"I came down with a fever of \0",1,0);
    interrupt(33,13,temp,1,0);
    interrupt(33,0,". Next my \0",1,0);
    interrupt(33,0,adjective,1,0);
    interrupt(33,0," pet \0",1,0);
    interrupt(33,0,animal,1,0);
    interrupt(33,0," must have\r\nsmelled the remains of the \0",1,0);
    interrupt(33,0,food,1,0);
    interrupt(33,0," on my computer, because he ate it. I am\r\n\0",1,0);
    interrupt(33,0,"currently rewriting the program and hope you will accept it late.\r\n\0",1,0);
    interrupt(33,0,"\r\nSincerely,\r\n\0",1,0);
    interrupt(33,0,"Nick Gallimore\r\n\0",1,0);
    while(1);
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
    switch(ax) {  
        case 0: 
            printString(bx, cx); 
            break;
        case 1:
            readString(bx);
            break; 
        case 2: 
        case 3: 
        case 4: 
        case 5:
        case 6: 
        case 7: 
        case 8: 
        case 9: 
        case 10:
        case 11: 
        case 12: 
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
    return;
}
