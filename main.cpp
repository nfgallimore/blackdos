#include <iostream>


void readInt(int* n)
{
    char* input;
    int i = 0;
    int digit = 0;

    input = "123";

    for (i = 0; input[i] != '\0'; ++i)
    {
        digit = input[i] - '0';
        *n = *n * 10 + digit;
    }

    return;
}

int mod(int a, int b)
{
    int x = a;
    while (x >=b)
    {
        x = x - b;
    }
    return x;
}

int divide(int a, int b)
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
int getNthDigit(int x, int n)
{
    return divide(mod(x, pow(10, n + 1)), pow(10, n));
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

void writePositiveInt(int x)
{
    char str[6];
    int numDigits;
    int i = 0;

    if (x < 1)
    {
        return;
    }

    numDigits = getNumDigits(x);

    for(; i < numDigits; i++)
    {
        str[i] = getNthDigit(x, numDigits-i-1) + '0';
    }

    while (numDigits < 5)
    {
        str[numDigits] = '\0';
        numDigits++;
    }

    str[5] = '\0';

    std::cout << str;
}

int main(int argc, char** argv)
{
    int x = 0;
    readInt(&x);
    writePositiveInt(x);
}