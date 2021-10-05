#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#define num_bgjobs 9
char bgjobs[num_bgjobs + 1][100];

void lex_sort(int *arr)
{ // bubble sort
    int temp = 0;
    for (int i = 1; i < num_bgjobs; i++)
    {
        printf("%d - i\n", i);
        for (int j = 1; j <= num_bgjobs - i; j++)
        {
            printf("%d - j\n", j);
            if (strcmp(bgjobs[arr[j]], bgjobs[arr[j + 1]]) > 0)
            {
                // a < b
                temp = arr[j + 1];
                arr[j + 1] = arr[j];
                arr[j] = temp;
            }
        }
    }
    return;
}

int main()
{
    // int arr[100];
    // for (int i = 0; i < num_bgjobs + 1; i++)
    // {
    //     arr[i] = i;
    // }
    strcpy(bgjobs[1], "zebra");
    strcpy(bgjobs[2], "name");
    strcpy(bgjobs[3], "is");
    strcpy(bgjobs[4], "mihir");
    strcpy(bgjobs[5], "bani");
    strcpy(bgjobs[6], "am zz");
    strcpy(bgjobs[7], "i");
    strcpy(bgjobs[8], "am z");
    strcpy(bgjobs[9], "hello");
    // printf("%d\n\n", strcmp(bgjobs[9], bgjobs[1]));
    // lex_sort(arr);
    // for (int i = 0; i < num_bgjobs + 1; i++)
    // {
    //     printf("%d - ", arr[i]);
    //     printf("%s\n", bgjobs[arr[i]]);
    // }
    if ((bgjobs[9][0] == 'h'))
    {
        printf("hielo\n");
    }
    return 0;
}