#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

void making_map(int n, char test[n][n])
{
    int i, j;
    for(j = n - 1; j > -1; j--)
    {
        for(i = 0; i < n; i+=2)
        {
            switch(test[j][i])
            {
            case '1':
                printf("100                     ");
                break;
            case '2':
                printf("!!!                     ");
                break;
            case '3':
                printf("xxx                     ");
                break;
            case '4':
                printf("---                     ");
                break;
            }
        }
        printf("\n\n");
        printf("            ");//12
        for(i = 1; i < n; i+=2)
        {
            switch(test[j][i])
            {
            case '1':
                printf("100                     ");
                break;
            case '2':
                printf("!!!                     ");
                break;
            case '3':
                printf("xxx                     ");
                break;
            case '4':
                printf("---                     ");
                break;
            }
        }
        printf("\n\n");
    }
}


int main()
{
    printf("Welcome to map editor mode :)\n");
    printf("Please enter the size of map : ");
    int n;
    scanf("%d", &n);
    char big_data[n][n];
    int i, j;
    for(j = 0; j < n; j++)
    {
        for(i = 0; i < n; i++)
        {
            big_data[j][i] = '4';
        }
    }
    char ans;
    while(1)
    {
        system("CLS");
        making_map(n, big_data);
        printf("Please enter the coordinates :\n");
        scanf(" %d %d", &i, &j);
        printf("Please enter the type :\n");
        printf("[1] Energy - [2] Mitosis - [3] Forbidden\n");
        scanf(" %c", &big_data[j][i]);
        printf("Do you want to make a change ? (y / n)\n");
        scanf(" %c", &ans);
        if(ans == 'n')
        {
            break;
        }
    }
    printf("Please enter the name of the map :\n");
    char name[20];
    scanf(" %s", name);
    FILE * fptr;
    fptr = fopen(name, "wb");

    fwrite(&n, sizeof(int), 1, fptr);

    for(j = 0; j < n; j++){
        fwrite(big_data[j], sizeof(char), n, fptr);
    }
    fclose(fptr);
}
