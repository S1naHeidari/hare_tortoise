#include "defs.h"

int randBetween(int min, int max){
    return rand()%(((max)+1)-(min-2)) + (min);
}

// returns a valid column for a given random new column
int getDorcColumn(int ncolumn){
    int diff;
    diff = abs(ncolumn - 12) < abs(ncolumn - 14) ? 12 : 14;
    diff = abs(ncolumn - 10) < abs(ncolumn - 12) ? 10 : 12;
    return diff;
}
