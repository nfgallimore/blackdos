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

void handleInterrupt21(int, int, int, int);
void printLogo();
void readString(char*);
void printString(char*, int);

void main()
{
    makeInterrupt21();
    printLogo();
    interrupt(33, 0, "Hello world from Nick Gallimore.\r\n\0", 1, 0);
    interrupt(33, 1, "Random string that doesnt get used for anything", 0, 0);
    while(1);
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
    return;
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
    interrupt(33, 0, " Author(s): Nick Gallimore\r\n\r\n\0", 0, 0);
}

void readString(char* c)
{
    int count = 0;
    char* letter;

    do 
    {
        letter = interrupt(22, 0, 0, 0, 0);

        if (letter == 8 && count > 0) 
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
    while (letter != 13);

    c[--count] = '\0';
    printString("\r\n\0");
    printString(c, 0);

    return;
}

/* MAKE FUTURE UPDATES HERE */
/* VVVVVVVVVVVVVVVVVVVVVVVV */



/* ^^^^^^^^^^^^^^^^^^^^^^^^ */
/* MAKE FUTURE UPDATES HERE */

void handleInterrupt21(int ax, int bx, int cx, int dx)
{
    switch(ax) {  
        case 0: 
            printString(bx, cx); 
            break;
        case 1:
            readString(bx);
            break; 
        /*case 2: 
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
        case 14: 
        case 15:*/
        default: 
            printString("General BlackDOS error.\r\n\0", 0);
    }
    return;
}
