#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <string.h>

//All of our structures
struct cell
{
    int x;
    int y;
    char name[3];
    int energy;
    int player;
    struct players_cell * next;
};
struct blocks
{
    int energy;
    char type;
    int status;
    char type_before;
};
//Creating random name
//-------------------------------------------------------------------------------------------
void rand_string(char str[4])
{
    char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int i;
    for(i = 0; i < 3; i++)
    {
        int key = rand() % 26;
        str[i] = charset[key];
    }
}
//-------------------------------------------------------------------------------------------
//Put color in text
//-------------------------------------------------------------------------------------------
void textcolor (int color)
{
    static int __BACKGROUND;
    HANDLE h = GetStdHandle ( STD_OUTPUT_HANDLE );
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
    GetConsoleScreenBufferInfo(h, &csbiInfo);
    SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE),
                             color + (__BACKGROUND << 2));
}
//-------------------------------------------------------------------------------------------
//Searching in cells
//-------------------------------------------------------------------------------------------
struct cell * search_cell(int i, int j, struct cell * head)
{
    struct cell * current = head->next;
    for(; current != NULL; current = current->next)
    {
        if(current->x == i && current->y == j)
        {
            return current;
        }
    }
    return NULL;
}
//-------------------------------------------------------------------------------------------
//Making cells
//-------------------------------------------------------------------------------------------
struct cell * making_cells(int i, int j, int player)
{
    struct cell * nn = malloc(sizeof(struct cell));
    if(nn == NULL)
    {
        printf("Something went wrong, please restart the game !");
        return;
    }
    nn->x = i;
    nn->y = j;
    rand_string(nn->name);
    nn->energy = 40;
    nn->player = player;
    nn->next = NULL;
    return nn;
}
void add_end(struct cell * head, struct cell * new_cell)
{
    struct cell * current;
    for(current = head; current->next != NULL; current = current->next);
    current->next = new_cell;
}
//-------------------------------------------------------------------------------------------
//This is all we do with data
//-------------------------------------------------------------------------------------------
void creating_data(int n, struct blocks data_dis[n][n], char data_test[])
{
    int i, j;
    int m;
    for(j = 0; j < n; j++)
    {
        for(i = 0; i < n; i++)
        {
            m = j * n + i;
            data_dis[j][i].type = data_test[m];
            if(data_test[m] == '1')
            {
                data_dis[j][i].energy = 100;
            }
            else
            {
                data_dis[j][i].energy = 0;
            }
            data_dis[j][i].status = 0;
            data_dis[j][i].type_before = data_test[m];
        }
    }
}

void placing_cells(int n, struct blocks data[n][n], int cells1, int cells2)
{
    int i, j;
    while(cells1)
    {
        i = rand() % n;
        j = rand() % n;
        if(data[j][i].type == '4')
        {
            data[j][i].type = '5';
            data[j][i].status = 1;
            data[j][i].type_before = '4';
            cells1--;
        }
    }
    while(cells2)
    {
        i = rand() % n;
        j = rand() % n;
        if(data[j][i].type == '4')
        {
            data[j][i].type = '6';
            data[j][i].status = 1;
            data[j][i].type_before = '4';
            cells2--;
        }
    }
    return;
}

void show_cells(int n, struct cell * head, struct blocks data_test[n][n])
{
    struct cell * current = head->next;
    int i = 1;
    int point = 0;
    int x, y;
    if(head->player == 1)
    {
        printf("Your cells are blue.\n\n");
    }
    else
    {
        printf("Your cells are red.\n");
    }
    printf("Player %d cells are :\n", head->player);
    for(; current != NULL; current = current->next, i++)
    {
        printf("[%d] %c%c%c", i, current->name[0], current->name[1], current->name[2]);
        printf(" : energy = %d ", current->energy);
        x = current->x;
        y = current->y;
        if(data_test[y][x].type_before == '1')
        {
            printf("( In ENERGY house , ENERGY = %d)\n", data_test[y][x].energy);
        }
        else if (data_test[y][x].type_before == '2')
        {
            printf("( In MITOSIS house )\n");
        }
        else
        {
            printf("\n");
        }
        point+= current->energy;
    }
    printf("\nYou have %d points.\n\n", point);
}

void making_link_lists(int n, struct blocks data[n][n], struct cell * head_1, struct cell * head_2)
{
    int i, j;
    struct cell * current;
    for(j = 0; j < n; j++)
    {
        for(i = 0; i < n; i++)
        {
            switch(data[j][i].type)
            {
            case '5':
                current = making_cells(i, j, 1);
                add_end(head_1, current);
                break;
            case '6':
                current = making_cells(i, j, 2);
                add_end(head_2, current);
                break;
            }
        }
    }
    return;
}
//-------------------------------------------------------------------------------------------
//Menu functions
//-------------------------------------------------------------------------------------------
void main_menu()
{
    textcolor(14);
    printf("[1] Move\n[2] Split a cell\n[3] Boost energy\n");
    printf("[4] Save\n[5] Exit\n\n");
    textcolor(15);
}
void moving_menu()
{
    textcolor(11);
    printf("[1] North\n[2] South\n[3] North-East\n");
    printf("[4] North-West\n[5] South-East\n[6] South-West\n\n");
    textcolor(15);
}
void game_menu()
{
    printf("[1] Load\n[2] New single player game\n");
    printf("[3] New Multiplayer game\n[4] Map Editor\n[5] Exit\n");
}
void guide_menu()
{
    printf("\n");
    printf("[1]ENERGY : WHITE  - [2]MITOSIS : YELLOW  - [3]FORBIDDEN : xxx  - [4]NORMAL : GREEN\n\n");
}
//-------------------------------------------------------------------------------------------
//Move functions
//-------------------------------------------------------------------------------------------
int check_place(int n, struct blocks data_test[n][n], int i, int j)
{
    if(i < 0 || j < 0 || i >= n || j >= n)
    {
        return 0;
    }
    if(data_test[j][i].status == 0 && data_test[j][i].type != '3')
    {
        return 1;
    }
    return 0;
}

int move_cell(int number, int n, struct cell * head, struct blocks data_test[n][n])
{
    struct cell * current = head->next;
    int i = 1;

    for(; i < number; i++, current = current->next);
    moving_menu();

    int input;
    scanf("%d", &input);

    int x_holder = current->x;
    int y_holder = current->y;

    int x_after = x_holder;
    int y_after = y_holder;

    switch(input)
    {
    case 1:
        y_after += 1;
        if(check_place(n, data_test, x_after, y_after) == 1)
        {
            data_test[y_holder][x_holder].type = data_test[y_holder][x_holder].type_before;
            data_test[y_after][x_after].type_before = data_test[y_after][x_after].type;
            data_test[y_holder][x_holder].status = 0;
            data_test[y_after][x_after].status = 1;
            if(current->player == 1)
            {
                data_test[y_after][x_after].type = '5';
            }
            else
            {
                data_test[y_after][x_after].type = '6';
            }
            current->y = y_after;
            current->x = x_after;
        }
        else
        {
            printf("Invalid move ! \n");
            Sleep(1000);
            return 0;
        }
        break;
    case 2:
        y_after -= 1;
        if(check_place(n, data_test, x_after, y_after) == 1)
        {
            data_test[y_holder][x_holder].type = data_test[y_holder][x_holder].type_before;
            data_test[y_after][x_after].type_before = data_test[y_after][x_after].type;
            data_test[y_holder][x_holder].status = 0;
            data_test[y_after][x_after].status = 1;
            if(current->player == 1)
            {
                data_test[y_after][x_after].type = '5';
            }
            else
            {
                data_test[y_after][x_after].type = '6';
            }
            current->y = y_after;
            current->x = x_after;
        }
        else
        {
            printf("Invalid move ! \n");
            Sleep(1000);
            return 0;
        }
        break;
    case 3:
        if(current->x % 2 == 0)
        {
            y_after += 1;
            x_after += 1;
        }
        else
        {
            x_after += 1;
        }
        if(check_place(n, data_test, x_after, y_after) == 1)
        {
            data_test[y_holder][x_holder].type = data_test[y_holder][x_holder].type_before;
            data_test[y_after][x_after].type_before = data_test[y_after][x_after].type;
            data_test[y_holder][x_holder].status = 0;
            data_test[y_after][x_after].status = 1;
            if(current->player == 1)
            {
                data_test[y_after][x_after].type = '5';
            }
            else
            {
                data_test[y_after][x_after].type = '6';
            }
            current->y = y_after;
            current->x = x_after;
        }
        else
        {
            printf("Invalid move ! \n");
            Sleep(1000);
            return 0;
        }
        break;
    case 4:
        if(current->x % 2 == 0)
        {
            y_after += 1;
            x_after -= 1;
        }
        else
        {
            x_after -= 1;
        }
        if(check_place(n, data_test, x_after, y_after) == 1)
        {
            data_test[y_holder][x_holder].type = data_test[y_holder][x_holder].type_before;
            data_test[y_after][x_after].type_before = data_test[y_after][x_after].type;
            data_test[y_holder][x_holder].status = 0;
            data_test[y_after][x_after].status = 1;
            if(current->player == 1)
            {
                data_test[y_after][x_after].type = '5';
            }
            else
            {
                data_test[y_after][x_after].type = '6';
            }
            current->y = y_after;
            current->x = x_after;
        }
        else
        {
            printf("Invalid move ! \n");
            Sleep(1000);
            return 0;
        }
        break;
    case 5:
        if(current->x % 2 == 0)
        {
            x_after += 1;
        }
        else
        {
            y_after -= 1;
            x_after += 1;
        }
        if(check_place(n, data_test, x_after, y_after) == 1)
        {
            data_test[y_holder][x_holder].type = data_test[y_holder][x_holder].type_before;
            data_test[y_after][x_after].type_before = data_test[y_after][x_after].type;
            data_test[y_holder][x_holder].status = 0;
            data_test[y_after][x_after].status = 1;
            if(current->player == 1)
            {
                data_test[y_after][x_after].type = '5';
            }
            else
            {
                data_test[y_after][x_after].type = '6';
            }
            current->y = y_after;
            current->x = x_after;
        }
        else
        {
            printf("Invalid move ! \n");
            Sleep(1000);
            return 0;
        }
        break;
    case 6:
        if(current->x % 2 == 0)
        {
            x_after -= 1;
        }
        else
        {
            y_after -= 1;
            x_after -= 1;
        }
        if(check_place(n, data_test, x_after, y_after) == 1)
        {
            data_test[y_holder][x_holder].type = data_test[y_holder][x_holder].type_before;
            data_test[y_after][x_after].type_before = data_test[y_after][x_after].type;
            data_test[y_holder][x_holder].status = 0;
            data_test[y_after][x_after].status = 1;
            if(current->player == 1)
            {
                data_test[y_after][x_after].type = '5';
            }
            else
            {
                data_test[y_after][x_after].type = '6';
            }
            current->y = y_after;
            current->x = x_after;
        }
        else
        {
            printf("Invalid move ! \n");
            Sleep(1000);
            return 0;
        }
        break;
    }
    return 1;
}
//Gaining energy
//-------------------------------------------------------------------------------------------
int gain_energy(int n, struct blocks data_test[n][n], int input, struct cell * head)
{
    struct cell * current = head->next;
    int i;
    for(i = 1; i < input; i++, current = current->next);
    int xx = current->x;
    int yy = current->y;
    if(data_test[yy][xx].type_before == '1' && data_test[yy][xx].energy > 0 && current->energy <= 85)
    {
        if(data_test[yy][xx].energy >= 15)
        {
            data_test[yy][xx].energy -= 15;
            current->energy += 15;
            return 1;
        }
        else
        {
            current->energy += data_test[yy][xx].energy;
            data_test[yy][xx].energy = 0;
            return 1;
        }
    }
    printf("Failed in gaining energy !\n");
    Sleep(1000);
    return 0;
}
//-------------------------------------------------------------------------------------------
//Dividing cells
//-------------------------------------------------------------------------------------------
int find_place(int xx, int yy, int * x, int * y, int n, struct blocks data_test[n][n])
{
    int x_holder = xx;
    int y_holder = yy;
    yy += 1;
    if(check_place(n, data_test, xx, yy) == 1)
    {
        * x = xx;
        * y = yy;
        return 1;
    }
    yy = y_holder;
    xx = x_holder;

    yy -= 1;
    if(check_place(n, data_test, xx, yy) == 1)
    {
        * x = xx;
        * y = yy;
        return 1;
    }
    yy = y_holder;
    xx = x_holder;

    if(xx % 2 == 0)
    {
        yy += 1;
        xx += 1;
    }
    else
    {
        xx += 1;
    }
    if(check_place(n, data_test, xx, yy) == 1)
    {
        * x = xx;
        * y = yy;
        return 1;
    }
    yy = y_holder;
    xx = x_holder;

    if(xx % 2 == 0)
    {
        yy += 1;
        xx -= 1;
    }
    else
    {
        xx -= 1;
    }
    if(check_place(n, data_test, xx, yy) == 1)
    {
        * x = xx;
        * y = yy;
        return 1;
    }
    yy = y_holder;
    xx = x_holder;

    if(xx % 2 == 0)
    {
        xx += 1;
    }
    else
    {
        yy -= 1;
        xx += 1;
    }
    if(check_place(n, data_test, xx, yy) == 1)
    {
        * x = xx;
        * y = yy;
        return 1;
    }
    yy = y_holder;
    xx = x_holder;

    if(xx % 2 == 0)
    {
        xx -= 1;
    }
    else
    {
        yy -= 1;
        xx -= 1;
    }
    if(check_place(n, data_test, xx, yy) == 1)
    {
        * x = xx;
        * y = yy;
        return 1;
    }
    return 0;
}

int cell_divition(int n, int number, struct cell * head, struct blocks data_test[n][n])
{

    struct cell * current = head->next;
    struct cell * holder;
    int i;

    for(i = 1; i < number; i++, current = current->next);

    int x_holder = current->x;
    int y_holder = current->y;
    int x_after;
    int y_after;

    if(data_test[y_holder][x_holder].type_before == '2' && current->energy >= 80)
    {
        i = find_place(x_holder, y_holder, &x_after, &y_after, n, data_test);
        if(i == 1)
        {
            data_test[y_after][x_after].type_before = data_test[y_after][x_after].type;
            if(data_test[y_holder][x_holder].type == '5')
            {
                data_test[y_after][x_after].type = '5';
                data_test[y_after][x_after].status = 1;
                rand_string(current->name);
                current->energy = 40;
                holder = making_cells(x_after, y_after, 1);
                add_end(head, holder);
            }
            else
            {
                data_test[y_after][x_after].type = '6';
                data_test[y_after][x_after].status = 1;
                rand_string(current->name);
                current->energy = 40;
                holder = making_cells(x_after, y_after, 1);
                add_end(head, holder);
            }
            return 1;
        }
        else
        {
            printf("Invalid move\n");
            Sleep(1000);
            return 0;
        }
    }
    else
    {
        printf("Invalid Move\n");
        Sleep(1000);
        return 0;
    }
    return 1;
}
//-------------------------------------------------------------------------------------------
//Every output we have
//-------------------------------------------------------------------------------------------
void making_map(int n, struct blocks test[n][n], struct cell * head_1, struct cell * head_2)
{
    guide_menu();
    int i, j;
    struct cell * holder;
    for(j = n - 1; j > -1; j--)
    {
        for(i = 0; i < n; i+=2)
        {
            switch(test[j][i].type)
            {
            case '1':
                if(test[j][i].energy == 100)
                {
                    textcolor(128);
                    printf("  %d  ", test[j][i].energy);
                    textcolor(15);
                    printf("                     ");
                }
                else if(test[j][i].energy < 10)
                {
                    textcolor(128);
                    printf("   %d   ", test[j][i].energy);
                    textcolor(15);
                    printf("                     ");
                }
                else
                {
                    textcolor(128);
                    printf("  %d   ", test[j][i].energy);
                    textcolor(15);
                    printf("                     ");
                }
                break;
            case '2':
                textcolor(110);
                printf("       ");
                textcolor(15);
                printf("                     ");
                break;
            case '3':
                textcolor(12);
                printf("  xxx  ");
                textcolor(15);
                printf("                     ");
                break;
            case '4':
                textcolor(47);
                printf("  ---  ");
                textcolor(15);
                printf("                     ");
                break;
            case '5':
                holder = search_cell(i, j, head_1);
                textcolor(31);
                printf("  %c%c%c  ", holder->name[0], holder->name[1], holder->name[2]);
                textcolor(15);
                printf("                     ");
                break;
            case '6':
                holder = search_cell(i, j, head_2);
                textcolor(71);
                printf("  %c%c%c  ", holder->name[0], holder->name[1], holder->name[2]);
                textcolor(15);
                printf("                     ");
                break;
            }
        }
        printf("\n\n");
        printf("              ");//12
        for(i = 1; i < n; i+=2)
        {
            switch(test[j][i].type)
            {
            case '1':
                if(test[j][i].energy == 100)
                {
                    textcolor(128);
                    printf("  %d  ", test[j][i].energy);
                    textcolor(15);
                    printf("                     ");
                }
                else if (test[j][i].energy < 10)
                {
                    textcolor(128);
                    printf("   %d   ", test[j][i].energy);
                    textcolor(15);
                    printf("                     ");
                }
                else
                {
                    textcolor(128);
                    printf("  %d   ", test[j][i].energy);
                    textcolor(15);
                    printf("                     ");
                }
                break;
            case '2':
                textcolor(110);
                printf("       ");
                textcolor(15);
                printf("                     ");
                break;
            case '3':
                textcolor(12);
                printf("  xxx  ");
                textcolor(15);
                printf("                     ");
                break;
            case '4':
                textcolor(47);
                printf("  ---  ");
                textcolor(15);
                printf("                     ");
                break;
            case '5':
                holder = search_cell(i, j, head_1);
                textcolor(31);
                printf("  %c%c%c  ", holder->name[0], holder->name[1], holder->name[2]);
                textcolor(15);
                printf("                     ");
                break;
            case '6':
                holder = search_cell(i, j, head_2);
                textcolor(71);
                printf("  %c%c%c  ", holder->name[0], holder->name[1], holder->name[2]);
                textcolor(15);
                printf("                     ");
                break;
            }
        }
        printf("\n\n");
    }
}
//-------------------------------------------------------------------------------------------
//Saving the game
//-------------------------------------------------------------------------------------------
int save_game(char name[], int player, int n, struct blocks data_test[n][n], int cells1, struct cell * head_1, int cells2, struct cell * head_2, int turn)
{
    //We want to save
    FILE * fname = fopen("name.txt", "w");
    fputs(name, fname);
    fclose(fname);

    FILE * fmap = fopen(name, "rb");
    if(fmap == NULL)
    {
        printf("Something went wrong the game did not save !\n");
        Sleep(1000);
        return 0;
    }
    FILE * fout = fopen("last_map.bin", "wb");
    if(fout == NULL)
    {
        printf("Something went wrong the game did not save !\n");
        Sleep(1000);
        return 0;
    }
    int m;
    fread(&m, sizeof(int), 1, fmap);

    char * input_map_data = malloc(m * m);

    fread(input_map_data, sizeof(char), m * m, fmap);

    fclose(fmap);


    fwrite(&m, sizeof(int), 1, fout);
    fwrite(input_map_data, sizeof(char), m * m, fout);


    FILE * fptr;
    fptr = fopen("last_save.bin", "wb");
    if(fptr == NULL)
    {
        printf("Something went wrong the game did not save !\n");
        Sleep(1000);
        return 0;
    }


    fwrite(&player, sizeof(int), 1, fptr);
    fwrite(&n, sizeof(int), 1, fptr);


    int i, j;
    for(j = 0; j < n; j++)
    {
        fwrite(data_test[j], sizeof(struct blocks), n, fptr);
    }

    struct cell * current = head_1->next;

    fwrite(&cells1, sizeof(int), 1, fptr);
    for(i = 0; i < cells1; i++)
    {
        fwrite(current, sizeof(struct cell), 1, fptr);
        current = current->next;
    }

    current = head_2->next;

    fwrite(&cells2, sizeof(int), 1, fptr);
    for(i = 0; i < cells2; i++)
    {
        fwrite(current, sizeof(struct cell), 1, fptr);
        current = current->next;
    }

    fwrite(&turn, sizeof(int), 1, fptr);
    fclose(fptr);
    return 1;
}
//-------------------------------------------------------------------------------------------
//Load game
//-------------------------------------------------------------------------------------------
int load_map(int * n, char * input_data, char name[])
{
    FILE * fmap = fopen("name.txt", "r");
    fgets(name, 20, fmap);
    fclose(fmap);
    //Opening a file
    FILE * fptr = fopen("last_map.bin", "rb");
    if(fptr == NULL)
    {
        printf("Can't find file");
        return 0;
    }

    int m;
    fread(&m, sizeof(int), 1, fptr);
    *n = m;
    input_data = malloc(m * m * sizeof(char));
    fread(input_data, sizeof(char), m * m, fptr);
    fclose(fptr);
    return 1;
}

int load_data(int * players, int n, struct blocks data_test[n][n], struct cell * head_1, struct cell * head_2, int * turn)
{
    FILE * fptr = fopen("last_save.bin", "rb");
    if(fptr == NULL)
    {
        printf("The saved file did not find.\n");
        return 0;
    }

    fread(players, sizeof(int), 1, fptr);
    fread(&n, sizeof(int), 1, fptr);

    int i, j;
    for(j = 0; j < n; j++)
    {
        fread(data_test[j], sizeof(struct blocks), n, fptr);
    }
    int cells;
    fread(&cells, sizeof(int), 1, fptr);
    struct cell * holder;
    for(i = 0; i < cells; i++)
    {
        holder = malloc(sizeof(struct cell));
        fread(holder, sizeof(struct cell), 1, fptr);
        holder->next = NULL;
        add_end(head_1, holder);
    }
    fread(&cells, sizeof(int), 1, fptr);
    for(i = 0; i < cells; i++)
    {
        holder = malloc(sizeof(struct cell));
        fread(holder, sizeof(struct cell), 1, fptr);
        holder->next = NULL;
        add_end(head_2, holder);
    }
    fread(turn, sizeof(int), 1, fptr);
    fclose(fptr);
    return 1;
}
//-------------------------------------------------------------------------------------------
//Map editor functions
//-------------------------------------------------------------------------------------------
void map_editor_making(int n, char test[n][n])
{
    int i, j;
    for(j = n - 1; j > -1; j--)
    {
        for(i = 0; i < n; i+=2)
        {
            switch(test[j][i])
            {
            case '1':
                textcolor(128);
                printf("  %d  ", 100);
                textcolor(15);
                printf("                     ");
                break;
            case '2':
                textcolor(110);
                printf("       ");
                textcolor(15);
                printf("                     ");
                break;
            case '3':
                textcolor(12);
                printf("  xxx  ");
                textcolor(15);
                printf("                     ");
                break;
            case '4':
                textcolor(47);
                printf("  ---  ");
                textcolor(15);
                printf("                     ");
                break;
            }
        }
        printf("\n\n");
        printf("              ");//12
        for(i = 1; i < n; i+=2)
        {
            switch(test[j][i])
            {
            case '1':
                textcolor(128);
                printf("  %d  ", 100);
                textcolor(15);
                printf("                     ");
                break;
            case '2':
                textcolor(110);
                printf("       ");
                textcolor(15);
                printf("                     ");
                break;
            case '3':
                textcolor(12);
                printf("  xxx  ");
                textcolor(15);
                printf("                     ");
                break;
            case '4':
                textcolor(47);
                printf("  ---  ");
                textcolor(15);
                printf("                     ");
                break;
            }
        }
        printf("\n\n");
    }
}

void map_editor()
{
    system("CLS");
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
        map_editor_making(n, big_data);
        printf("Do you want to make a change ? (y / n)\n");
        scanf(" %c", &ans);
        if(ans == 'n')
        {
            break;
        }
        printf("Please enter the coordinates :\n");
        scanf(" %d %d", &i, &j);
        printf("Please enter the type :\n");
        printf("[1] Energy - [2] Mitosis - [3] Forbidden\n");
        scanf(" %c", &big_data[j][i]);
        system("CLS");
        map_editor_making(n, big_data);
    }
    printf("Please enter the name of the map :\n");
    char name[20];
    scanf(" %s", name);
    FILE * fptr;
    fptr = fopen(name, "wb");

    fwrite(&n, sizeof(int), 1, fptr);

    for(j = 0; j < n; j++)
    {
        fwrite(big_data[j], sizeof(char), n, fptr);
    }
    fclose(fptr);
    system("CLS");
}
//-------------------------------------------------------------------------------------------
