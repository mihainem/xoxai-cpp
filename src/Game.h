#ifndef GAME_H
#define GAME_H
#include <random>
#include <vector>
#include "Cell.h"

class Game
{
public:
    std::vector<char> history;
    std::vector<Cell> moves_history;
    char turn;
    char winner;
    char** board;

    Game(int rows = 3, int cols = 3);

    void reset();

    char check_play_ended();

    void undo_move();

    void print();

    void set(std::vector<std::vector<char>> board);

    bool is_valid(int row, int col);

    bool is_board_full();

    void make_move(Cell move, char symbol);

    Cell best_move( char symbol);

    Cell wrong_move(char symbol);

    Cell next_best_move(char player_symbol, char enemy_symbol);

    Cell get_hint_move();
private:

    int rows;
    int cols;
    char player;
    char enemy;

    std::vector<Cell> get_corners();

    std::vector<Cell> get_edges();

    std::vector<Cell> get_cells(std::vector<Cell> cells, bool valid);

    std::vector<Cell> get_valid_corners();

    std::vector<Cell> get_invalid_corners();

    std::vector<Cell> get_invalid_edges();

    Cell get_random_corner();

    Cell get_random_valid_move();

    std::vector<Cell> get_valid_edges();

    bool is_won(char symbol);

    bool is_winning_move(Cell move, char symbol);

    std::vector<Cell> get_all_valid_moves();

    bool are_cells_adjacent(Cell cell1, Cell cell2);

    bool are_opponents_cells_adjacent(Cell cell1, Cell cell2, char current_player_symbol);

    Cell opposite_corner(Cell cell1, Cell cell2);

    std::vector<Cell> adjacent_corners(Cell cell1, Cell cell2);

    Cell random_adjacent_corner(Cell cell1, Cell cell2);

};
#endif
