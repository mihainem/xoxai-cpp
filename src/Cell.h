#ifndef CELL_H
#define CELL_H

typedef struct
{
    int row = -1;
    int col = -1;
    
    bool is_empty() const {
        return row == -1 && col == -1;
    };
} Cell;
#endif

