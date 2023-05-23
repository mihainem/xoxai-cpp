#include <iostream>
#include "game.h"

Game::Game(int rows, int cols)
{
    this->rows = rows;
    this->cols = cols;
    this->board = new char *[rows];
    for (int i = 0; i < rows; i++)
    {
        this->board[i] = new char[cols];
        for (int j = 0; j < cols; j++)
        {
            this->board[i][j] = '-';
        }
    }
    this->turn = 'X';
    this->player = 'X';
    this->enemy = 'O';
    // init empty moves_history
}

void Game::reset()
{
    for (int i = 0; i < this->rows; i++)
    {
        for (int j = 0; j < this->cols; j++)
        {
            this->board[i][j] = '-';
        }
    }
    // empty moves_history vector
    this->moves_history.clear();
    this->winner = NULL;
}

char Game::check_play_ended()
{
    if (is_won('X'))
    {
        this->winner = 'X';
    }
    else if (is_won('O'))
    {
        this->winner = 'O';
    }
    else if (is_board_full())
    {
        this->winner = 'T';
    }
    else
    {
        return NULL;
    }

    this->history.push_back(this->winner);
    return this->winner;
}

// convert from the above commented python function to a c++ optimal function
bool Game::is_won(char symbol)
{
    int row = this->moves_history.back().row;
    int col = this->moves_history.back().col;
    char **board = this->board;
    if (board[row][col] == symbol && (board[row][0] == board[row][1] == board[row][2] == symbol || board[0][col] == board[1][col] == board[2][col] == symbol || board[0][0] == board[1][1] == board[2][2] == symbol || board[0][2] == board[1][1] == board[2][0] == symbol))
    {
        return true;
    }
    return false;
}

void Game::undo_move()
{
    if (this->moves_history.size() > 0)
    {
        Cell last_move = this->moves_history.back();
        this->board[last_move.row][last_move.col] = '-';
        this->moves_history.pop_back();
        this->turn = this->turn == 'X' ? 'O' : 'X';
    }
}

void Game::print()
{
    for (int i = 0; i < this->rows; i++)
    {
        for (int j = 0; j < this->cols; j++)
        {
            std::cout << this->board[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void Game::set(std::vector<std::vector<char>> board)
{
    for (int i = 0; i < this->rows; i++)
    {
        for (int j = 0; j < this->cols; j++)
        {
            this->board[i][j] = board[i][j];
        }
    }
}

bool Game::is_valid(int row, int col)
{
    if (row < 0 || row > 2 || col < 0 || col > 2)
    {
        return false;
    }
    if (this->board[row][col] != '-')
    {
        return false;
    }
    return true;
}

bool Game::is_board_full()
{
    for (int i = 0; i < this->rows; i++)
    {
        for (int j = 0; j < this->cols; j++)
        {
            if (this->board[i][j] == '-')
            {
                return false;
            }
        }
    }
    return true;
}

bool Game::is_winning_move(Cell move, char symbol)
{
    int row = move.row;
    int col = move.col;
    char **board = this->board;
    if (board[row][col] == symbol && (board[row][0] == board[row][1] == board[row][2] == symbol || board[0][col] == board[1][col] == board[2][col] == symbol || board[0][0] == board[1][1] == board[2][2] == symbol || board[0][2] == board[1][1] == board[2][0] == symbol))
    {
        return true;
    }
    return false;
}

std::vector<Cell> Game::get_corners()
{
    return std::vector<Cell>{{0, 0}, {0, 2}, {2, 0}, {2, 2}};
}

std::vector<Cell> Game::get_edges()
{
    std::vector<Cell> edges;
    for (int i = 0; i < this->rows; i++)
    {
        for (int j = 0; j < this->cols; j++)
        {
            if ((i == 0 || i == 2) && (j == 1))
            {
                edges.push_back({i, j});
            }
            else if ((j == 0 || j == 2) && (i == 1))
            {
                edges.push_back({i, j});
            }
        }
    }
    return edges;
}

std::vector<Cell> Game::get_cells(std::vector<Cell> cells, bool valid)
{
    std::vector<Cell> valid_cells;
    for (int i = 0; i < cells.size(); i++)
    {
        if (this->is_valid(cells[i].row, cells[i].col) == valid)
        {
            valid_cells.push_back(cells[i]);
        }
    }
    return valid_cells;
}

std::vector<Cell> Game::get_valid_corners()
{
    return this->get_cells(this->get_corners(), true);
}

std::vector<Cell> Game::get_invalid_corners()
{
    return this->get_cells(this->get_corners(), false);
}

std::vector<Cell> Game::get_invalid_edges()
{
    return this->get_cells(this->get_edges(), false);
}

Cell Game::get_random_corner()
{
    std::vector<Cell> valid_corners = this->get_valid_corners();
    return valid_corners[rand() % valid_corners.size()];
}

std::vector<Cell> Game::get_all_valid_moves()
{
    std::vector<Cell> valid_moves;
    for (int i = 0; i < this->rows; i++)
    {
        for (int j = 0; j < this->cols; j++)
        {
            if (this->is_valid(i, j))
            {
                valid_moves.push_back({i, j});
            }
        }
    }
    return valid_moves;
}

Cell Game::get_random_valid_move()
{
    std::vector<Cell> valid_moves = this->get_all_valid_moves();
    return valid_moves[rand() % valid_moves.size()];
}

std::vector<Cell> Game::get_valid_edges()
{
    return this->get_cells(this->get_edges(), true);
}

void Game::make_move(Cell move, char symbol)
{
    this->board[move.row][move.col] = symbol;
    this->moves_history.push_back(move);
}

bool Game::are_cells_adjacent(Cell cell1, Cell cell2)
{
    if (cell1.row == cell2.row && abs(cell1.col - cell2.col) == 1)
    {
        return true;
    }
    if (cell1.col == cell2.col && abs(cell1.row - cell2.row) == 1)
    {
        return true;
    }
    return false;
}

bool Game::are_opponents_cells_adjacent(Cell cell1, Cell cell2, char current_player_symbol)
{
    if (cell1.row == cell2.row && abs(cell1.col - cell2.col) == 1)
    {
        if (this->board[cell1.row][cell1.col] != current_player_symbol && this->board[cell2.row][cell2.col] != current_player_symbol)
        {
            return true;
        }
    }
    if (cell1.col == cell2.col && abs(cell1.row - cell2.row) == 1)
    {
        if (this->board[cell1.row][cell1.col] != current_player_symbol && this->board[cell2.row][cell2.col] != current_player_symbol)
        {
            return true;
        }
    }
    return false;
}

Cell Game::opposite_corner(Cell cell1, Cell cell2)
{

    for (Cell corner : get_valid_corners())
    {
        if (!are_cells_adjacent(cell1, corner) && !are_cells_adjacent(cell2, corner))
        {
            return corner;
        }
    }
}

std::vector<Cell> Game::adjacent_corners(Cell cell1, Cell cell2)
{
    std::vector<Cell> adjacent_corners;
    for (Cell corner : get_valid_corners())
    {
        if (are_cells_adjacent(cell1, corner) || are_cells_adjacent(cell2, corner))
        {
            adjacent_corners.push_back(corner);
        }
    }
    return adjacent_corners;
}

Cell Game::random_adjacent_corner(Cell cell1, Cell cell2)
{
    std::vector<Cell> adjacent_corners = this->adjacent_corners(cell1, cell2);
    return adjacent_corners[rand() % adjacent_corners.size()];
}
/*

    def best_move(self, symbol):
        if self.is_valid(1, 1):
            return [1, 1]

        for move in self.get_all_valid_moves():
            if self.is_winning_move(move, symbol):
                return move

        #at this stage the next move decides the game
        if len(self.moves_history) == 3:
            #this counteracts two-edges-strategy where the opponent makes two edges and
            # if the current player doesn't chose correctly, the opponent moves in the adjacent corner to both edges to get double chances
            invalid_edges = self.get_invalid_edges()
            if len(invalid_edges) == 2:
                cell1, cell2 = invalid_edges
                if self.are_opponents_cells_adjacent(cell1, cell2, symbol):
                    return self.random_adjacent_corner(cell1, cell2)
            elif len(invalid_edges) == 3:
                cell1, cell2, cell3 = invalid_edges
                if self.are_opponents_cells_adjacent(cell1, cell2, symbol):
                    return self.random_adjacent_corner(cell1, cell2)
                if self.are_opponents_cells_adjacent(cell1, cell3, symbol):
                    return self.random_adjacent_corner(cell1, cell3)
                if self.are_opponents_cells_adjacent(cell2, cell3, symbol):
                    return self.random_adjacent_corner(cell2, cell3)

            #this counteracts two-corners-strategy where the opponent marks two opposing corners in diagonal and
            # if the current player doesn't chose correctly, the opponent moves in a third corner to get double chances
            invalid_corners = self.get_invalid_corners()
            if len(invalid_corners) == 2 :
                [r1,c1], [r2, c2] = invalid_corners
                if self.board[r1][c1] == self.board[r2][c2] != symbol and r1!=r2 and c1 != c2:
                    return random.choice(self.get_valid_edges())

        print(f"no best move found for {symbol}")
        return None

    def wrong_move(self, symbol):
        for move in [[0,1], [1,0], [1,2], [2,1]]:
            row, col = move
            if self.is_valid(row, col) and not self.is_winning_move(move, symbol):
                return move
        for move in self.get_valid_corners(): #.get_cells(self.get_corners()):
            row, col = move
            if not self.is_winning_move(move, symbol):
                return move
        return [1, 1]

    def next_best_move(self, player_symbol, enemy_symbol):
        return self.best_move(player_symbol) or self.best_move(enemy_symbol)  or self.get_random_corner() or self.get_random_valid_move()


    def get_hint_move(self):
        return self.next_best_move(self.player, self.enemy)
        */

Cell Game::best_move(char symbol)
{
    if (is_valid(1, 1))
    {
        return {1, 1};
    }

    for (Cell move : get_all_valid_moves())
    {
        if (is_winning_move(move, symbol))
        {
            return move;
        }
    }

    // at this stage the next move decides the game
    if (this->moves_history.size() == 3)
    {
        // this counteracts two-edges-strategy where the opponent makes two edges and
        //  if the current player doesn't chose correctly, the opponent moves in the adjacent corner to both edges to get double chances
        std::vector<Cell> invalid_edges = get_invalid_edges();
        if (invalid_edges.size() == 2)
        {
            Cell cell1 = invalid_edges[0];
            Cell cell2 = invalid_edges[1];
            if (are_opponents_cells_adjacent(cell1, cell2, symbol))
            {
                return random_adjacent_corner(cell1, cell2);
            }
        }
        else if (invalid_edges.size() == 3)
        {
            Cell cell1 = invalid_edges[0];
            Cell cell2 = invalid_edges[1];
            Cell cell3 = invalid_edges[2];
            if (are_opponents_cells_adjacent(cell1, cell2, symbol))
            {
                return random_adjacent_corner(cell1, cell2);
            }
            if (are_opponents_cells_adjacent(cell1, cell3, symbol))
            {
                return random_adjacent_corner(cell1, cell3);
            }
            if (are_opponents_cells_adjacent(cell2, cell3, symbol))
            {
                return random_adjacent_corner(cell2, cell3);
            }
        }

        // this counteracts two-corners-strategy where the opponent marks two opposing corners in diagonal and
        //  if the current player doesn't chose correctly, the opponent moves in a third corner to get double chances
        std::vector<Cell> invalid_corners = get_invalid_corners();
        if (invalid_corners.size() == 2)
        {
            Cell cell1 = invalid_corners[0];
            Cell cell2 = invalid_corners[1];
            if (this->board[cell1.row][cell1.col] == this->board[cell2.row][cell2.col] != symbol && cell1.row != cell2.row && cell1.col != cell2.col)
            {
                int valid_edges_count = get_valid_edges().size();
                return get_valid_edges()[rand() % valid_edges_count];
            }
        }
    }
}

Cell Game::wrong_move(char symbol)
{
    for (Cell move : {{0, 1}, {1, 0}, {1, 2}, {2, 1}})
    {
        int row = move.row;
        int col = move.col;
        if (is_valid(row, col) && !is_winning_move(move, symbol))
        {
            return move;
        }
    }
    for (Cell move : get_valid_corners())
    {
        int row = move.row;
        int col = move.col;
        if (!is_winning_move(move, symbol))
        {
            return move;
        }
    }
    return {1, 1};
}

Cell Game::next_best_move(char player_symbol, char enemy_symbol)
{
    Cell result = best_move(player_symbol);
    if (!result.is_empty())
    {
        return result;
    }

    result = best_move(enemy_symbol);
    if (!result.is_empty())
    {
        return result;
    }

    result = get_random_corner();
    if (!result.is_empty())
    {
        return result;
    }

    return get_random_valid_move();
}

Cell Game::get_hint_move()
{
    return next_best_move(this->player, this->enemy);
}

Game::~Game()
{
    for (int i = 0; i < this->rows; i++)
    {
        delete[] this->board[i];
    }
    delete[] this->board;
}
