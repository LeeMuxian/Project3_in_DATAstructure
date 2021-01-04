#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "algorithm.h"
//#include "../include/algorithm.h"

#define corner 28
#define edge 14
#define interior 8

using namespace std;

#define ROW 5
#define COL 6

class My_Cell{
    private:
        int orbs_num;   // The number of the orbs on the cell 
        int capacity;   // The max number of the orbs of a cell
        char color;     // The color of the cell, 'w'(No one has placed the orbs), 'r'(Red Player) and 'b'(Blue player)
        bool explode;

    public:
        My_Cell();

        // The basic functions of the Cell
        int get_orbs_num(); 
        int get_capacity();
        char get_color();
        bool get_explode();

        void set_orbs_num(int orbs);
        void set_capacity(int cap);
        void set_color(char col);
        void set_explode(bool tof);
};

class My_Board{
    private:
        void cell_reaction_marker();                // After the explosion, mark all the cell that  will explode in next iteration
        bool cell_is_full(My_Cell* cell);              // Check wether the cell is full of orbs and set the explosion variable to be true
        void add_orb(int i, int j, char color);     // Add orb into the cell (i, j)
        void cell_reset(int i, int j);              // Reset the cell to the initial state (color = 'w', orb_num = 0)
        void cell_explode(int i, int j);            // The explosion of cell (i, j). It will add the orb into neighbor cells
        void cell_chain_reaction(char color);    // If there is aa explosion, check wether the chain reaction is active

    public:
        My_Cell cells[ROW][COL];                       // The 5*6 board whose index (0,0) is start from the upper left corner
        My_Board();
        
        // The basic functions of the Board
        int get_orbs_num(int i, int j);
        int get_capacity(int i, int j);
        char get_cell_color(int i, int j);

        void place_orb(int i, int j, char color);      // Use this function to place a orb into a cell

        bool win_the_game(char color);                  // The function that is used to check wether the player wins the game after his/her placemnet operation
};


/******************************************************
 * In your algorithm, you can just use the the funcitons
 * listed by TA to get the board information.(functions 
 * 1. ~ 4. are listed in next block)
 * 
 * The STL library functions is not allowed to use.
******************************************************/

/*************************************************************************
 * 1. int board.get_orbs_num(int row_index, int col_index)
 * 2. int board.get_capacity(int row_index, int col_index)
 * 3. char board.get_cell_color(int row_index, int col_index)
 * 4. void board.print_current_board(int row_index, int col_index, int round)
 * 
 * 1. The function that return the number of orbs in cell(row, col)
 * 2. The function that return the orb capacity of the cell(row, col)
 * 3. The function that return the color fo the cell(row, col)
 * 4. The function that print out the current board statement
*************************************************************************/

int predict_next_coor(int, My_Board, char);
int predict(int, My_Board, char);
int fweight(My_Board, char);
int judge(int, int, My_Board);

void algorithm_A(Board board, Player player, int index[]){

    //////your algorithm design///////////
    int temp;
    char color = player.get_color();
    My_Board my_board;
    int first_half = 0;

    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 6; j++) {
            my_board.cells[i][j].set_orbs_num(board.get_orbs_num(i, j));
            first_half += board.get_orbs_num(i, j);
            my_board.cells[i][j].set_color(board.get_cell_color(i, j));
            my_board.cells[i][j].set_explode(false);
        }

    if (first_half <= 16)
        temp = predict_next_coor(3, my_board, color);
    else
        temp = predict_next_coor(4, my_board, color);

    index[0] = temp / 10;
    index[1] = temp % 10;
}

int predict_next_coor(int N, My_Board my_board, char color) {
    int coor, weight = -10000000, temp;
    char color_temp;
    My_Board last_board;

    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 6; j++) {
            last_board.cells[i][j].set_orbs_num(my_board.get_orbs_num(i, j));
            last_board.cells[i][j].set_color(my_board.get_cell_color(i, j));
            last_board.cells[i][j].set_explode(false);
        }


    color_temp = (color == 'r') ? 'b' : 'r';

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 6; j++) {
            if (color == my_board.get_cell_color(i, j) || 'w' == my_board.get_cell_color(i, j)) {
                my_board.place_orb(i, j, color);
                if (my_board.win_the_game(color)) return (i * 10 + j);
                temp = predict(N - 1, my_board, color_temp);
                if (temp >= weight) {
                    weight = temp;
                    coor = i * 10 + j;
                }
                for (int i = 0; i < 5; i++)
                    for (int j = 0; j < 6; j++) {
                        my_board.cells[i][j].set_orbs_num(last_board.get_orbs_num(i, j));
                        my_board.cells[i][j].set_color(last_board.get_cell_color(i, j));
                        my_board.cells[i][j].set_explode(false);
                    }
            }
        }
    }

    return coor;
}

int predict(int N, My_Board my_board, char color) {     // N is the remaining steps that need to be predicted
    int weight_r = -1000000, weight_b = -1000000, weight = 0, temp;
    char color_temp = (color == 'r') ? 'b' : 'r';
    My_Board last_board;

    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 6; j++) {
            last_board.cells[i][j].set_orbs_num(my_board.get_orbs_num(i, j));
            last_board.cells[i][j].set_color(my_board.get_cell_color(i, j));
            last_board.cells[i][j].set_explode(false);
        }

    
    if (N > 0) {
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 6; j++) {
                if (my_board.get_cell_color(i, j) == color || my_board.get_cell_color(i, j) == 'w') {
                    my_board.place_orb(i, j, color);
                    if (my_board.win_the_game(color)) return -100000;
                    temp = predict(N - 1, my_board, color_temp);
                    if (color_temp == 'r') {
                        weight_r = (temp > weight_r) ? temp : weight_r;
                    }
                    else {
                        weight_b = (temp > weight_b) ? temp : weight_b;
                    }
                    for (int i = 0; i < 5; i++)
                        for (int j = 0; j < 6; j++) {
                            my_board.cells[i][j].set_orbs_num(last_board.get_orbs_num(i, j));
                            my_board.cells[i][j].set_color(last_board.get_cell_color(i, j));
                            my_board.cells[i][j].set_explode(false);
                        }
                }
                
            }
        }
        if (color == 'r') weight = weight_r - weight_b;
        else weight = weight_b - weight_r;
        
        return -weight;
    } else {
        return -fweight(my_board, color);
    }
}

int fweight(My_Board my_board, char color) {
    int weight, weight_me = 0, weight_enemy = 0;

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 6; j++) {
            if (color == my_board.get_cell_color(i, j)) {
                if ((i == 0 && j == 0) || (i == 0 && j == 5) || (i == 4 && j == 0) || (i == 4 && j == 5)) {
                    weight_me += corner * my_board.get_orbs_num(i, j) - corner * judge(i, j, my_board);
                } else if (i == 0 || i == 4 || j == 0 || j == 5) {
                    weight_me += edge * my_board.get_orbs_num(i, j) - edge * judge(i, j, my_board);
                } else {
                    weight_me += interior * my_board.get_orbs_num(i, j) - interior * judge(i, j, my_board);
                }
            } else if (my_board.get_cell_color(i, j) == 'w') {
                // do nothing
            } else {
                if ((i == 0 && j == 0) || (i == 0 && j == 5) || (i == 4 && j == 0) || (i == 4 && j == 5)) {
                    weight_enemy += corner * my_board.get_orbs_num(i, j) - corner * judge(i, j, my_board);
                } else if (i == 0 || i == 4 || j == 0 || j == 5) {
                    weight_enemy += edge * my_board.get_orbs_num(i, j) - edge * judge(i, j, my_board);
                } else {
                    weight_enemy += interior * my_board.get_orbs_num(i, j) - interior * judge(i, j, my_board);
                }
            }
        }
    }
    weight = weight_me - weight_enemy;

    return weight;
}

int judge(int row, int col, My_Board my_board) {
    char color = my_board.get_cell_color(row, col);
    int judge = 0, temp = 0, num_orb = my_board.get_orbs_num(row, col), capacity = my_board.get_capacity(row, col);

    if (row - 1 >= 0 && color != my_board.get_cell_color(row - 1, col) && my_board.get_cell_color(row - 1, col) != 'w')
        if ((capacity - num_orb) > (my_board.get_capacity(row - 1, col) - my_board.get_orbs_num(row - 1, col)))
            judge = num_orb;
        else 
            judge = 0;

    if (row + 1 < 5 && color != my_board.get_cell_color(row + 1, col) && my_board.get_cell_color(row + 1, col) != 'w')
        if ((capacity - num_orb) > (my_board.get_capacity(row + 1, col) - my_board.get_orbs_num(row + 1, col)))
            temp = num_orb;
        else 
            temp = 0;
    if (judge < temp) judge = temp;

    if (col - 1 >= 0 && color != my_board.get_cell_color(row, col - 1) && my_board.get_cell_color(row, col - 1) != 'w')
        if ((capacity - num_orb) > (my_board.get_capacity(row, col - 1) - my_board.get_orbs_num(row, col - 1)))
            temp = num_orb;
        else 
            temp = 0;
    if (judge < temp) judge = temp;

    if (col + 1 < 6 && color != my_board.get_cell_color(row, col + 1) && my_board.get_cell_color(row, col + 1) != 'w')
        if ((capacity - num_orb) > (my_board.get_capacity(row, col + 1) - my_board.get_orbs_num(row, col + 1)))
            temp = num_orb;
        else 
            temp = 0;
    if (judge < temp) judge = temp;

    if (row - 1 >= 0 && col - 1 >= 0 && color != my_board.get_cell_color(row - 1, col - 1) && my_board.get_cell_color(row - 1, col - 1) != 'w')
        if ((capacity - num_orb) > (my_board.get_capacity(row - 1, col - 1) - my_board.get_orbs_num(row - 1, col - 1)))
            temp = num_orb;
        else 
            temp = 0;
    if (judge < temp) judge = temp;

    if (row - 1 >= 0 && col + 1 < 6 && color != my_board.get_cell_color(row - 1, col + 1) && my_board.get_cell_color(row - 1, col + 1) != 'w')
        if ((capacity - num_orb) > (my_board.get_capacity(row - 1, col + 1) - my_board.get_orbs_num(row - 1, col + 1)))
            temp = num_orb;
        else 
            temp = 0;
    if (judge < temp) judge = temp;

    if (row + 1 < 5 && col - 1 >= 0 && color != my_board.get_cell_color(row + 1, col - 1) && my_board.get_cell_color(row + 1, col - 1) != 'w')
        if ((capacity - num_orb) > (my_board.get_capacity(row + 1, col - 1) - my_board.get_orbs_num(row + 1, col - 1)))
            temp = num_orb;
        else 
            temp = 0;
    if (judge < temp) judge = temp;

    if (row + 1 < 5 && col + 1 < 6 && color != my_board.get_cell_color(row + 1, col + 1) && my_board.get_cell_color(row + 1, col + 1) != 'w')
        if ((capacity - num_orb) > (my_board.get_capacity(row + 1, col + 1) - my_board.get_orbs_num(row + 1, col + 1)))
            temp = num_orb;
        else 
            temp = 0;
    if (judge < temp) judge = temp;

    return judge;
}

My_Cell::My_Cell(){
    orbs_num = 0, capacity = 8, color='w', explode = false;
}

int My_Cell::get_capacity(){
    return this->capacity;
}

int My_Cell::get_orbs_num(){
    return this->orbs_num;
}

char My_Cell::get_color(){
    return this->color;
}

bool My_Cell::get_explode(){
    return this->explode;
}

void My_Cell::set_orbs_num(int orbs){
    this->orbs_num = orbs;
}

void My_Cell::set_capacity(int cap){
    this->capacity = cap;
}

void My_Cell::set_color(char col){
    this->color = col;
}

void My_Cell::set_explode(bool tof){
    this->explode = tof;
}

My_Board::My_Board(){

    ////// Initialize the borad with correct capacity //////
    // The corners of the board
    cells[0][0].set_capacity(3), cells[0][5].set_capacity(3),
    cells[4][0].set_capacity(3), cells[4][5].set_capacity(3);

    // The edges of the board
    cells[0][1].set_capacity(5), cells[0][2].set_capacity(5), cells[0][3].set_capacity(5), cells[0][4].set_capacity(5),
    cells[1][0].set_capacity(5), cells[2][0].set_capacity(5), cells[3][0].set_capacity(5),
    cells[1][5].set_capacity(5), cells[2][5].set_capacity(5), cells[3][5].set_capacity(5),
    cells[4][1].set_capacity(5), cells[4][2].set_capacity(5), cells[4][3].set_capacity(5), cells[4][4].set_capacity(5);

}

void My_Board::place_orb(int i, int j, char color){
    
    int temp = cells[i][j].get_orbs_num();
    temp += 1;
    cells[i][j].set_orbs_num(temp);
    cells[i][j].set_color(color);

    if(cell_is_full(&cells[i][j])){
        cell_explode(i, j);
        cell_reaction_marker();
        cell_chain_reaction(color);
    }
}

bool My_Board::cell_is_full(My_Cell* cell){
    if(cell->get_orbs_num() >= cell->get_capacity()){
        cell->set_explode(true);
        return true;
    }
    else return false;
}

void My_Board::add_orb(int i, int j, char color){
    int orb_num = cells[i][j].get_orbs_num();
    orb_num ++;
    cells[i][j].set_orbs_num(orb_num);
    cells[i][j].set_color(color);
}

void My_Board::cell_reset(int i, int j){
    cells[i][j].set_orbs_num(0);
    cells[i][j].set_explode(false);
    cells[i][j].set_color('w');
}

void My_Board::cell_explode(int i, int j){

    int orb_num;
    char color = cells[i][j].get_color();

    cell_reset(i, j);

    if( i + 1 < ROW){
        add_orb(i+1, j, color);
    }

    if( j + 1 < COL){
        add_orb(i, j+1, color);
    }

    if( i - 1 >= 0){
        add_orb(i-1, j, color);
    }

    if( j - 1 >= 0){
        add_orb(i, j-1, color);
    }
    if( i + 1 < ROW && j - 1 >= 0){
        add_orb(i+1, j-1, color);
    }
    if( i - 1 >= 0 && j - 1 >= 0){
        add_orb(i-1, j-1, color);
    }
    if( i + 1 < ROW && j + 1 < COL){
        add_orb(i+1, j+1, color);
    }
    if( i - 1 >= 0 && j + 1 < COL){
        add_orb(i-1, j+1, color);
    }
}

void My_Board::cell_reaction_marker(){

    // Mark the next cell whose number of orbs is equal to the capacity
    for(int i = 0; i < ROW; i++){
            for(int j = 0; j < COL; j++){
                cell_is_full(&cells[i][j]);
            }
        }     
}

void My_Board::cell_chain_reaction(char color){
    
    bool chain_reac = true;

    while(chain_reac){

        chain_reac = false;

        for(int i = 0; i < ROW; i++){
            for(int j = 0; j < COL; j++){
                if(cells[i][j].get_explode()){
                    cell_explode(i ,j);
                    chain_reac = true;
                }
            }
        }

        if(win_the_game(color)){
            return;
        }

        cell_reaction_marker();
    }
}

bool My_Board::win_the_game(char color){

    bool win = true;

    for(int i = 0; i < ROW; i++){
        for(int j = 0; j < COL; j++){
            if(cells[i][j].get_color() == color || cells[i][j].get_color() == 'w') continue;
            else{
                win = false;
                break;
            }
        }
        if(!win) break;
    }
    return win;
}

int My_Board::get_orbs_num(int i, int j){
    return cells[i][j].get_orbs_num();
}

int My_Board::get_capacity(int i, int j){
    return cells[i][j].get_capacity();
}

char My_Board::get_cell_color(int i, int j){
    return cells[i][j].get_color();
}