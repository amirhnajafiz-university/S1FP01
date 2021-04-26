#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <string.h>
#include "header.h"


int main()
{
    srand(time(NULL));
    system("CLS");
    game_menu();
    int input;
    scanf("%d", &input);

    //These are the headers for cells link list
    struct cell * header_1 = making_cells(-1, -1, 1);
    struct cell * header_2 = making_cells(-1, -1, 2);

    int n;

    int cells_1 = 0;
    int cells_2 = 0;
    int turn = 1;
    int players_num;
    int cell;
    int option;
    int result;
    char save;

    int check = 0;

    char * input_map_data;
    char name[20];

    switch(input)
    {
    case 1:
        check = load_map(&n, input_map_data, name);
        if(check == 0)
        {
            system("CLS");
            return main();
        }
        break;
    case 2:
        players_num = 1;
        printf("Please enter the number of cells : ");
        scanf(" %d", &cells_1);
        break;
    case 3:
        players_num = 2;
        printf("Please enter the number of cells for player 1 : ");
        scanf(" %d", &cells_1);
        printf("Please enter the number of cells for player 2 : ");
        scanf(" %d", &cells_2);
        break;
    case 4:
        map_editor();
        return main();
    case 5:
        system("CLS");
        printf("\n\n\n    Thanks for playing :) \n\n\n");
        return 0;
    }

    //Opening a file
    if(input != 1)
    {
        printf("Please enter the name of the map : \n");
        scanf("%s", name);
        FILE * fptr = fopen(name, "rb");
        if(fptr == NULL)
        {
            printf("Can't find file");
            return main();
        }
        fread(&n, sizeof(unsigned), 1, fptr);
        input_map_data = malloc(n * n * sizeof(char));
        //char input_map_data[n * n];
        fread(input_map_data, sizeof(char), n * n, fptr);
        // i * c + j
        fclose(fptr);

    }
    //This is the main array for saving data
    struct blocks big_data[n][n];

    //This is for a saved game
    if(check == 1)
    {
        check = load_data(&players_num, n, big_data, header_1, header_2, &turn);
        if(check == 0)
        {
            printf("No saved data !\n");
            system("CLS");
            return main();
        }
    }


    //This is for reading a from a file
    if(input != 1)
    {
        creating_data(n, big_data, input_map_data);
        //We import the data in our map
        placing_cells(n, big_data, cells_1, cells_2);
        //Creating data in memory
        making_link_lists(n, big_data, header_1, header_2);
    }
    while(1)
    {
        system("CLS");
        making_map(n, big_data, header_1, header_2);
        //Showing players info
        if(turn % 2 == 1)
        {
            show_cells(n, header_1, big_data);
        }
        else
        {
            show_cells(n, header_2, big_data);
        }
        printf("Please select a cell : ");
        scanf(" %d", &cell);

        main_menu();
        scanf(" %d", &option);

        switch(option)
        {
        case 1:
            if(turn % 2 == 1)
            {
                result = move_cell(cell, n, header_1, big_data);
                if(players_num == 2)
                {
                    if(result == 1)
                    {
                        turn++;
                    }
                }
            }
            else
            {
                result = move_cell(cell, n, header_2, big_data);
                if(players_num == 2)
                {
                    if(result == 1)
                    {
                        turn++;
                    }
                }
            }
            break;
        case 2:
            if(turn % 2 == 1)
            {
                result = cell_divition(n, cell, header_1, big_data);
                if(players_num == 2)
                {
                    if(result == 1)
                    {
                        turn++;
                    }
                }
            }
            else
            {
                result = cell_divition(n, cell, header_2, big_data);
                if(players_num == 2)
                {
                    if(result == 1)
                    {
                        turn++;
                    }
                }
            }
            break;
        case 3:
            if(turn % 2 == 1)
            {
                result = gain_energy(n, big_data, cell, header_1);
                if(players_num == 2)
                {
                    if(result == 1)
                    {
                        turn++;
                    }
                }
            }
            else
            {
                result = gain_energy(n, big_data, cell, header_2);
                if(players_num == 2)
                {
                    if(result == 1)
                    {
                        turn++;
                    }
                }
            }
            break;
        case 4:
            result = save_game(name, players_num, n, big_data, cells_1, header_1, cells_2, header_2, turn);
            if(result == 1)
            {
                printf("The game saved successfully !\n");
                Sleep(1000);
            }
            break;
        case 5:
            printf("Do you want to save the game ? (y / n)\n");
            scanf(" %c", &save);
            if(save == 'y')
            {
                result = save_game(name, players_num, n, big_data, cells_1, header_1, cells_2, header_2, turn);
            }
            system("CLS");
            printf("\n\n\n    Thanks for playing !\n\n\n");
            return 0;
        }
    }
}
