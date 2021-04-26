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
//-------------------------------------------------------------------------------------------
void rand_string(char str[4]);
void textcolor (int color);
struct cell * search_cell(int i, int j, struct cell * head);
struct cell * making_cells(int i, int j, int player);
void add_end(struct cell * head, struct cell * new_cell);
void creating_data(int n, struct blocks data_dis[n][n], char data_test[]);
void placing_cells(int n, struct blocks data[n][n], int cells1, int cells2);
void show_cells(int n, struct cell * head, struct blocks data_test[n][n]);
void making_link_lists(int n, struct blocks data[n][n], struct cell * head_1, struct cell * head_2);
void main_menu();
void moving_menu();
void game_menu();
void guide_menu();
int check_place(int n, struct blocks data_test[n][n], int i, int j);
int move_cell(int number, int n, struct cell * head, struct blocks data_test[n][n]);
int gain_energy(int n, struct blocks data_test[n][n], int input, struct cell * head);
int find_place(int xx, int yy, int * x, int * y, int n, struct blocks data_test[n][n]);
int cell_divition(int n, int number, struct cell * head, struct blocks data_test[n][n]);
void making_map(int n, struct blocks test[n][n], struct cell * head_1, struct cell * head_2);
int save_game(char name[], int player, int n, struct blocks data_test[n][n], int cells1, struct cell * head_1, int cells2, struct cell * head_2, int turn);
int load_map(int * n, char * input_data, char name[]);
int load_data(int * players, int n, struct blocks data_test[n][n], struct cell * head_1, struct cell * head_2, int * turn);
void map_editor_making(int n, char test[n][n]);
void map_editor();
