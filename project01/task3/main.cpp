#include "engine.h"

int main()
{
    // Skill lists
    Skill Tackle = Skill("Tackle", NORMAL, 20),
        Leaf = Skill("Leaf", GRASS, 20),
        Flame = Skill("Flame", FIRE, 20),
        Stream = Skill("Stream", WATER, 20);

    // Team
    Slime playerTeam[3] = {
        Slime("Green", 110, 10, 10, 10, false, GRASS, &Tackle, &Leaf),
        Slime("Red", 100, 11, 10, 11, false, FIRE, &Tackle, &Flame),
        Slime("Blue", 100, 10, 11, 9, false, WATER, &Tackle, &Stream)
    };

    Slime enemyTeam[3] = {
        Slime("Green", 110, 10, 10, 10, true, GRASS, &Tackle, &Leaf),
        Slime("Red", 100, 11, 10, 11, true, FIRE, &Tackle, &Flame),
        Slime("Blue", 100, 10, 11, 9, true, WATER, &Tackle, &Stream)
    };

    // Play
    play(playerTeam, enemyTeam);

    return 0;
}