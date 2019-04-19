#include "blackdos.h"

int strlen(char* s);
int strcmp(char* s1, char* s2);
int isUpper(char c);
void getArg(char* userInput, char* arg, int argNum);
void copyFile(char* src, char* tar);
void listDirectory();

int main()
{
    char buffer[13312];
    char config[512];
    char userInput[512];
    char command[512];
    char arg1[512];
    char arg2[512];
    int* size;
    char tweet[141];

    // loads config
    interrupt(33, 2, config, 258, 0);

    // clears screen
    interrupt(33, 12, config[0] + 1, config[1] + 1, 0);

    PRINTS("Welcome to blackdos!\r\n\r\n\0");

    while(1)
    {
        PRINTS("^(~(oo)~)^ \0");
        SCANS(userInput);

        getArg(userInput, command, 0);

        PRINTS("\r\n\0");

        if (strcmp(command, "boot", 4))
        {
            interrupt(33,11,0,0,0);
        }
        else if (strcmp(command, "clrs", 4))
        {
            interrupt(33, 12, config[0] + 1, config[1] + 1, 0);
        }
        else if (strcmp(command, "copy", 4))
        {
            getArg(userInput, arg1, 1);
            getArg(userInput, arg2, 2);
            if (isUpper(arg1[0]) || isUpper(arg2[0]))
            {
                interrupt(33, 15, 1, 0, 0);
            }
            copyFile(arg1, arg2);
        }
        else if (strcmp(command, "ddir", 4))
        {
            listDirectory();
        }
        else if (strcmp(command, "echo", 4))
        {
            getArg(userInput, arg1, 1);
            PRINTS(arg1);
        }
        else if (strcmp(command, "exec", 4))
        {
            getArg(userInput, arg1, 1);
            interrupt(33, 4, arg1, 4, 0);
        }
        else if (strcmp(command, "remv", 4))
        {
            getArg(userInput, arg1, 1);
            if (isUpper(arg1[0]))
            {
                interrupt(33, 15, 1, 0, 0);
            }
            interrupt(33, 7, arg1, 0, 0);
        }
        else if (strcmp(command, "senv", 4))
        {
            interrupt(33, 4, "Stenv", 4, 0);
        }
        else if (strcmp(command, "show", 4))
        {
            getArg(userInput, arg1, 1);
            interrupt(33, 3, arg1, buffer, size);
            PRINTS(buffer);
        }
        else if (strcmp(command, "print", 4))
        {
            getArg(userInput, arg1, 1);
            interrupt(33, 3, arg1, buffer, size);
            LPRINTS(buffer);
        }
        else if (strcmp(command, "help", 4))
        {
            PRINTS("prnt - to print to printer.out\r\n\0");
            PRINTS("show - show the contents of a file on the screen\r\n\0");
            PRINTS("exec - execute a program with given filename as argument\r\n\0");
            PRINTS("remv - remove a file with given filename as argument\r\n\0");
            PRINTS("copy - copy file1 file2 - copies file1 to file2\r\n\0");
            PRINTS("ddir - list contents of disk\r\n\0");
            PRINTS("boot - reboot system\r\n\0");
            PRINTS("clrs - clear screen\r\n\0");
            PRINTS("echo - echos the text following the command\r\n\0");
            PRINTS("help - help menu\r\n\0");
            PRINTS("senv - set enviornment variables\r\n\0");
            PRINTS("twet - create a text file\0");
            PRINTS("\r\n\0");
        }
        else if (strcmp(command, "twet", 4))
        {
            getArg(userInput, arg1, 1);
            if (isUpper(arg1[0]))
            {
                interrupt(33, 15, 1, 0, 0);
            }
            else
            {
                PRINTS("Enter tweet shorter than 140 chars: ");
                SCANS(tweet);
                tweet[140] = '\0';
                interrupt(33, 8, arg1, tweet, 1);
                PRINTS("\r\n\0");
                buffer[0] = '\0';
            }
        }
    }
    return 0;
}

void getArg(char* userInput, char* arg, int argNum)
{
    int i, j, currArg;
    i = 0, j = 0, currArg = 0;


    while (userInput[i] != '\0' && currArg < argNum)
    {
        if (userInput[i] == ' ')
        {
            currArg++;
        }
        i++;

    }
    while (userInput[i] != ' ' && userInput[i] != '\0')
    {
        arg[j] = userInput[i];
        i++;
        j++;
    }
    arg[j] = '\0';
}


int strlen(char* s)
{
    int i;
    i = 0;
    while (s[i] != '\0')
    {
        i++;
    }
    return i;
}

int strcmp(char* s1, char* s2, int len)
{
    int i;

    for (i = 0; i < len; i++)
    {
        if (s1[i] != s2[i])
        {
            return 0;
        }
    }

    return 1;
}

int isUpper(char c)
{
    if (c >= 'A' && c <= 'Z')
    {
        return 1;
    }
    return 0;
}

void copyFile(char* src, char* tar)
{
    char buffer[13312];
    char directory[512];
    char currentFile[9];
    int i, j, k, sectors;

    sectors = 0;

    interrupt(33, 2, directory, 2, 0);

    // loop through all 16 files in directory
    for (i = 0; i < 512; i += 32)
    {
        for (j = 0; j < 6; j++)
        {
            currentFile[j] = directory[i + j];
        }
        if (strcmp(currentFile, src))
        {
            for (k = i + 6; directory[k] != 0; k++)
            {
                sectors++;
            }
        }
    }

    interrupt(33, 3, src, buffer);
    interrupt(33, 8, tar, buffer, sectors);
}

void listDirectory()
{
    char directory[512];
    char currentFile[7];
    int i, j;

    interrupt(33, 2, directory, 257, 0);

    // loop through all 16 files in directory
    for (i = 0; i < 512; i += 32)
    {
        if (directory[i] != 0)
        {
            for (j = 0; j < 6; j++)
            {
                if (directory[i + j] == ' ')
                {
                    break;
                }
                currentFile[j] = directory[i + j];
            }
            currentFile[j + 1] = '\0';

            PRINTS(currentFile);
            PRINTS("\r\n\0");       
        }
    }
}
