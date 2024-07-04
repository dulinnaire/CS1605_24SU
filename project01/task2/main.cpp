#include <iostream>
#include "engine.h"

using namespace std;

int main()
{
    Slime playerTeam[3] = {
        Slime("Green", 110, 10, 10, 10, false, GRASS, TACKLE, LEAF),
        Slime("Red", 100, 11, 10, 11, false, FIRE, TACKLE, FLAME),
        Slime("Blue", 100, 10, 11, 9, false, WATER, TACKLE, STREAM)
    };

    Slime enemyTeam[3] = {
        Slime("Green", 110, 10, 10, 10, true, GRASS, TACKLE, LEAF),
        Slime("Red", 100, 11, 10, 11, true, FIRE, TACKLE, FLAME),
        Slime("Blue", 100, 10, 11, 9, true, WATER, TACKLE, STREAM)
    };

    play(playerTeam, enemyTeam);

    return 0;
}