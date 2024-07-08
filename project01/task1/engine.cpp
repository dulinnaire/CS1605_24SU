#include <iostream>
#include "engine.h"

using namespace std;


int slimeAlive(Slime *team) {
    int aliveCount = 0;
    for (int i = 0; i < 3; i++) {
        if (team[i].getHP() > 0) {aliveCount++;}
    }
    return aliveCount;
}

int findAlive(Slime *team, int curr) {
    for (int i = 0; i < 3; i++) {
        if (i == curr - 1) {continue;}
        if (team[i].getHP() > 0) {
            return i + 1;
        }
    }
    return -1;
}

void getSlimeRemain(int &S1, int &S2, int curr) {
    switch (curr)
    {
    case 1:
        S1 = 2;
        S2 = 3;
        break;
    case 2:
        S1 = 1;
        S2 = 3;
        break;
    case 3:
        S1 = 1;
        S2 = 2;
        break;
    default:
        break;
    }
}



void playerPassiveChange(Slime *playerTeam, int& playerSlimeChoice, Slime **pp_self) {
    // 没有被击倒
    if (playerTeam[playerSlimeChoice - 1].getHP() > 0) {
        return;
    }
    // 被击倒
    printPlayerBeaten(&playerTeam[playerSlimeChoice - 1]);
    int choice = 0, alive = 0;
    switch (slimeAlive(playerTeam))
    {
    case 1:
        alive = findAlive(playerTeam, playerSlimeChoice);
        choice = selectSlimeFrom1(playerTeam, alive);
        printSend(&playerTeam[choice - 1]);
        playerSlimeChoice = choice;
        break;
    case 2:
        int Slime1, Slime2;
        getSlimeRemain(Slime1, Slime2, playerSlimeChoice);
        choice = selectSlimeFrom2(playerTeam, Slime1, Slime2);
        printSend(&playerTeam[choice - 1]);
        playerSlimeChoice = choice;
        break;
    default:
        return;
        break;
    }
    *pp_self = &playerTeam[playerSlimeChoice - 1];
}

void enemyPassiveChange(Slime *enemyTeam, int &enemySlimeChoice, Slime *p_rival, Slime **pp_self) {
    // 没有被击倒
    if (enemyTeam[enemySlimeChoice - 1].getHP() > 0) {
        return;
    }
    // 被击倒
    printEnemyBeaten(&enemyTeam[enemySlimeChoice - 1]);
    int choice = 0, alive = 0;
    switch (slimeAlive(enemyTeam))
    {
    case 1:
        alive = findAlive(enemyTeam, enemySlimeChoice);
        printSend(&enemyTeam[alive - 1]);
        enemySlimeChoice = alive;
        break;
    case 2:
        int Slime1, Slime2;
        getSlimeRemain(Slime1, Slime2, enemySlimeChoice);
        if (isTypeAdvantage(enemyTeam[Slime1 - 1].getType(), p_rival->getType())) {
            choice = Slime1;
        } else if (isTypeAdvantage(enemyTeam[Slime2 - 1].getType(), p_rival->getType())) {
            choice = Slime2;
        } else if (enemyTeam[Slime1].getType() == p_rival->getType()) {
            choice = Slime1;
        } else {
            choice = Slime2;
        }
        printSend(&enemyTeam[choice - 1]);
        enemySlimeChoice = choice;
        break;
    default:
        return;
    }
    *pp_self = &enemyTeam[enemySlimeChoice - 1];
}

void play(Slime *playerTeam, Slime *enemyTeam) {
// 战斗开始前
    // 初始信息
    printWelcome();
    
    // 玩家选择Slime
    int playerSlimeChoice = firstSlimeChoice();
    Slime *p_playerSlime = &playerTeam[playerSlimeChoice - 1];

    // 敌人选择Slime(选择属性克制的)
    int enemySlimeChoice = playerSlimeChoice % 3 + 1;
    Slime *p_enemySlime = &enemyTeam[enemySlimeChoice - 1];

// 战斗过程中
    pringStart(p_playerSlime, p_enemySlime);
    int roundCount = 1;
    bool gameOver = false;
    while (!gameOver) {
        // 回合开始
        printHP(p_playerSlime, p_enemySlime);
        printRound(roundCount);
        // 选择行动
        // 玩家选择行动
        int playerActionChoice = -1;
    
        if (slimeAlive(playerTeam) == 1) {
            playerActionChoice = selectActionFrom1();
        } else {
            playerActionChoice = selectActionFrom2();
        }

        int playerChangeChoice = playerSlimeChoice;
        // 选择技能
        SkillEnum playerSkill = TACKLE;
        if (playerActionChoice == 1) {
            int playerSkillChoice = selectSkill(p_playerSlime);
            playerSkill = p_playerSlime->getSkill(playerSkillChoice);
        
        // 选择交换
        } else {
            // 只有一个能交换
            if (slimeAlive(playerTeam) == 2){
                int tempAlive = findAlive(playerTeam, playerSlimeChoice);
                playerChangeChoice = selectSlimeFrom1(playerTeam, tempAlive);
            // 两个能交换
            } else {
                int Slime1, Slime2;
                getSlimeRemain(Slime1, Slime2, playerSlimeChoice);
                playerChangeChoice = selectSlimeFrom2(playerTeam, Slime1, Slime2);
            } 
        }
        // 简单敌人（1）选择行动
        SkillEnum enemySkill = TACKLE;
        if (isTypeAdvantage(p_enemySlime->getType(), p_playerSlime->getType())) {
            enemySkill = p_enemySlime->getSkill(2);
        } else {
            enemySkill = p_enemySlime->getSkill(1);
        }

        // 实施行动
        // 玩家选择主动交换史莱姆
        if (playerActionChoice == 2) {
            playerSlimeChoice = playerChangeChoice;
            p_playerSlime = &playerTeam[playerSlimeChoice - 1];
            printSend(p_playerSlime);
            p_enemySlime->attack(enemySkill, p_playerSlime);
            playerPassiveChange(playerTeam, playerSlimeChoice, &p_playerSlime);
        // 玩家选择攻击
        } else {
            if (p_playerSlime->getSPD() > p_enemySlime->getSPD()){
                p_playerSlime->attack(playerSkill, p_enemySlime);
                if (p_enemySlime->getHP() <= 0) {
                    enemyPassiveChange(enemyTeam, enemySlimeChoice, p_playerSlime, &p_enemySlime);
                } else {
                    p_enemySlime->attack(enemySkill, p_playerSlime);
                    playerPassiveChange(playerTeam, playerSlimeChoice, &p_playerSlime);
                }
            } else {
                p_enemySlime->attack(enemySkill, p_playerSlime);
                if (p_playerSlime->getHP() <= 0) {
                    playerPassiveChange(playerTeam, playerSlimeChoice, &p_playerSlime);    
                } else {
                    p_playerSlime->attack(playerSkill, p_enemySlime);
                    enemyPassiveChange(enemyTeam, enemySlimeChoice, p_playerSlime, &p_enemySlime);
                }
            }
        }

        if (slimeAlive(playerTeam) == 0 || slimeAlive(enemyTeam) == 0 || roundCount >= 100) {
            gameOver = true;
        }
        // 回合结束
        roundCount++;
        
    }

// 战斗结束
    if (roundCount >= 100) {
        printHP(p_playerSlime, p_enemySlime);
        cout << "Draw" << endl;
    } else if (slimeAlive(playerTeam) > 0) {
        cout << "You Win" << endl;
    } else {
        cout << "You Lose" << endl;
    }
}