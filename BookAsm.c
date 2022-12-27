#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

int binaryToDecimal(int binaryNumber)
{
    // declaration of variables
    int decimalRep = 0, temp, remainder, base = 1;
    temp = binaryNumber;

    // applying the conversion algorithm
    while(temp > 0)
    {
        // dividing the binary number and storing the rightmost digit in the remainder variable
        remainder = temp % 10;
        decimalRep = decimalRep + (remainder * base);
        temp /= 10;
        base *= 2;
    }
    return decimalRep;
}

char *inputString(FILE* fp, size_t size)
{
    //The size is extended by the input with the value of the provisional
    char *str;
    int ch;
    size_t len = 0;
    str = realloc(NULL, sizeof(*str)*size);//size is start size
    if(!str)return str;
    while(EOF!=(ch=fgetc(fp)) && ch != '\n'){
        str[len++]=ch;
        if(len==size){
            str = realloc(str, sizeof(*str)*(size+=16));
            if(!str)return str;
        }
    }
    str[len++]='\0';

    return realloc(str, sizeof(*str)*len);
}

bool startsWith(const char *pre, const char *str)
{
    size_t lenpre = strlen(pre),
           lenstr = strlen(str);
    return lenstr < lenpre ? false : memcmp(pre, str, lenpre) == 0;
}

void displayHelp(void)
{
    // printf("Book Asm is based on the idea of an imaginary tape, like a turing machine.\n");
    // printf("You start at index 0, or at the left most of the tape. Using < you can move left, and using > you can move right.\n");
    // printf("To flip the current bit on the tape, use !.\n");
    // printf("To get the user input and set it to the current bit, use @.\n");
    // printf("To display the current bit, use #.\n");
    // printf("To reset to the start of the tape, use r.\n");
    // printf("There is a more special character, c. The c command will take the current and next 7 bits (8 bits in total) and convert them into a number. It will then print that number's Ascii character\n");
    // printf("For example, the letter c in ascii is 99, in binary 99 is 01100011. To print c, you could use this program: >!>!>>>>!>!rc\n");
    printf("Check Book Asm's Github Repo for help.\n");
}

void runToks(char *input, int inputLen, char *cur, bool *roll)
{
    //printf("input: %s  len: %d\n", input, inputLen);
    for(int i = 0; i < inputLen; i++)
    {
        switch (input[i])
        {
            case '<':
                if(*cur == 0)
                {
                    *cur = 64;
                }
                else
                {
                    *cur -= 1;
                }
                break;
            case '>':
                if(*cur == 63)
                {
                    *cur = 0;
                }
                else
                {
                    *cur += 1;
                }
                break;
            case '!':
                // flip bit
                roll[*cur] = !(roll[*cur]);
                break;
            case '@':
                // get user input
                printf("Enter 0 or 1: ");
                int temp;
                scanf("%d", &temp);
                roll[*cur] = temp;
                break;
            case '#':
                // print current bit
                printf("%d", roll[*cur]);
                break;

            case 'r':
                // reset cur
                *cur = 0;
                break;

            case 'c': ;
                // print current byte as an ascii character
                int bin = 0;

                // turn an array like {0, 1, 1, 0, 0, 0, 1, 1} to -> (int) 01100011
                int toPow = pow(10, 7);
                for(int i = 0; i < 8; i++)
                {
                    bin += roll[*cur + i] * toPow;
                    toPow = toPow / 10;
                }

                printf("%c", binaryToDecimal(bin));
                break;

            case 'i':
                // skip over the i
                i++;
                if(input[i] != '{')
                {
                    printf("Expected opening curly brace, found: %c", input[i]);
                    break;
                }

                char *closing;
                int openingBraceCount = 0;
                for(int j = i + 1; j < inputLen; j++)
                {
                    if(input[j] == "{")
                    {
                        openingBraceCount++;
                    }
                    else if(input[j] == "}")
                    {
                        if(openingBraceCount == 0)
                        {
                            closing = &input[j];
                            break;
                        }
                        else
                        {
                            openingBraceCount--;
                        }
                    }
                }

                if(!closing)
                {
                    printf("Could not find closing curly brace.");
                    break;
                }
                
                if(roll[*cur])
                {
                    runToks(&input[i + 1], (int) (closing - &input[i + 1]), cur, roll);
                }


                i = closing - input;
                break;

            case 's':
                // print a smile
                printf("%c", 1);
                break;

            case ':':
                // comment
                for(int j = 1; true; i++)
                {
                    if(input[i + j] == ';')
                    {
                        i = i + j;
                        break;
                    }
                }
                break;

            default:
                printf("Unknown token: %c\n", input[i]);
                break;
        }
    }
}

int main(void)
{
    printf("Type ~h for help. Type ~q to quit.");

    do
    {
        printf("\n$ ");
        char *input = inputString(stdin, 10);

        if(startsWith("~h", input))
        {
            displayHelp();
            free(input);
            continue;;
        }

        if(startsWith("~q", input))
        {
            free(input);
            break;
        }

        bool roll[64] = {false};

        //printf("Output:\n");

        // loop over input
        char cur = 0;

        runToks(input, strlen(input), &cur, roll);

        free(input);
    }
    while(true);

    return 0;
}
