#include "display.h"

int calculateScore(int hitsInARow)
{
    // Simple scoring function: score is square of hits in a row
    return hitsInARow * hitsInARow;
}