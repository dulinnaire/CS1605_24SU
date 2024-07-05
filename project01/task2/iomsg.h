#ifndef IOMSG_H_INCLUDED
#define IOMSG_H_INCLUDED

#include <string>
#include "unit.h"

using namespace std;

void printWelcome();

void pringStart(Slime *p_playerSlime, Slime *p_enemySlime);

void printHP(Slime *p_playerSlime, Slime *p_enemySlime);

void printPlayerBeaten(Slime *p_slime);

void printEnemyBeaten(Slime *p_slime);

void printRound(int n);

void printSend(Slime *p_slime);

void printAttack(Slime *p_slime, string skillName, int damage);

void printAtkPotion(Slime *p_slime);

void printRevivalPotion(Slime *p_slime);

void printEndGame();

int inputHelper(string prompt, int choice1);

int inputHelper(string prompt, int choice1, int choice2);

int inputHelper(string prompt, int choice1, int choice2, int choice3);

int firstSlimeChoice();

int selectActionFrom1();

int selectActionFrom2();

int selectSkill(Slime *p_slime);

int selectSlimeFrom1(Slime *team, int n);

int selectSlimeFrom2(Slime *team, int a, int b);




#endif