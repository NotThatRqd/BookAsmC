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

int main(void)
{
    char *input;
    printf("Enter Book Asm:\n");
    input = inputString(stdin, 10);

    bool roll[16] = {false};

    printf("Output:\n");

    // loop over input
    char cur = 0;
    for(int i = 0; i < strlen(input); i++)
    {
        switch (input[i])
        {
            case '<':
                cur -= 1;
                break;
            case '>':
                cur += 1;
                break;
            case '!':
                // flip bit
                roll[cur] = !(roll[cur]);
                break;
            case '@':
                // get user input
                printf("\nEnter 0 or 1: ");
                int temp;
                scanf("%d", &temp);
                roll[cur] = temp;
                break;
            case '#':
                // print current bit
                printf("\n%d", roll[cur]);
                break;

            case 'r':
                cur = 0;
                break;

            case 'c': ;
                // print current byte as an ascii character
                int bin = 0;

                // turn an array like {0, 1, 1, 0, 0, 0, 1, 1} to -> (int) 01100011
                int toPow = pow(10, 7);
                for(int i = 0; i < 8; i++)
                {
                    bin += roll[cur + i] * toPow;
                    toPow = toPow / 10;
                }

                printf("%c", binaryToDecimal(bin));
                break;
            default:
                printf("Unknown token: %c\n", input[i]);
                break;
        }
    }

    free(input);

    // stop execution until key pressed so people running via double-clicking the icon can see the output
    getchar();
    return 0;
}
