#include <iostream>
#include "engine.h"

using namespace std;

int totalHP(Slime *team) {
    int total = 0;
    for (int i = 0; i < 3; i++) {
        total += team[i].getHP();
    }
    return total;
}

int slimeAlive(Slime *team) {
    int aliveCount = 0;
    for (int i = 0; i < 3; i++) {
        if (team[i].getHP() > 0) {aliveCount++;}
    }
    return aliveCount;
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



int playerPassiveChange(Slime *team, int curr) {
    printPlayerBeaten(&team[curr - 1]);
    int choice = 0, alive = 0;
    switch (slimeAlive(team))
    {
    case 1:
        for (int i = 0; i < 3; i++) {
            if (team[i].getHP() > 0) {
                alive = i + 1;
                break;
            }
        }
        choice = selectSlimeFrom1(team, alive);
        printPlayerSend(&team[choice - 1]);
        return choice;
        break;
    case 2:
        int Slime1, Slime2;
        getSlimeRemain(Slime1, Slime2, curr);
        choice = selectSlimeFrom2(team, Slime1, Slime2);
        printPlayerSend(&team[choice - 1]);
        return choice;
        break;
    default:
        return -1;
        break;
    }
}

int enemyPassiveChange(Slime *team, int curr, Slime *rival) {
    printEnemyBeaten(&team[curr - 1]);
    int choice = 0, alive = 0;
    switch (slimeAlive(team))
    {
    case 1:
        for (int i = 0; i < 3; i++) {
            if (team[i].getHP() > 0) {
                alive = i + 1;
                break;
            }
        }
        printEmenySend(&team[alive - 1]);
        return alive;
    case 2:
        int Slime1, Slime2;
        getSlimeRemain(Slime1, Slime2, curr);
        if (isTypeAdvantage(team[Slime1 - 1].getType(), rival->getType())) {
            choice = Slime1;
        } else if (isTypeAdvantage(team[Slime2 - 1].getType(), rival->getType())) {
            choice = Slime2;
        } else if (team[Slime1].getType() == rival->getType()) {
            choice = Slime1;
        } else {
            choice = Slime2;
        }
        printEmenySend(&team[choice - 1]);
        return choice;
    default:
        return -1;
    }
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

        int playerChangeChoice = playerSlimeChoice, enemyChangeChoice = enemySlimeChoice;
        // 选择技能
        SkillEnum playerSkill = TACKLE;
        if (playerActionChoice == 1) {
            int playerSkillChoice = selectSkill(p_playerSlime);
            playerSkill = p_playerSlime->getSkill(playerSkillChoice);
        
        // 选择交换
        } else {
            // 只有一个能交换
            if (slimeAlive(playerTeam) == 2){
                int tempAlive;
                for (int i = 0; i < 3; i++) {
                    if (i + 1 != playerSlimeChoice && playerTeam[i].getHP() != 0) {
                        tempAlive = i + 1;
                        break;
                    }
                }
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
            printPlayerSend(p_playerSlime);
            p_enemySlime->attack(enemySkill, p_playerSlime);
            if (p_playerSlime->getHP() <= 0) {
                playerChangeChoice = playerPassiveChange(playerTeam, playerSlimeChoice);

                if (playerChangeChoice == -1) break;

                playerSlimeChoice = playerChangeChoice;
                p_playerSlime = &playerTeam[playerSlimeChoice - 1];
            }
        // 玩家选择攻击
        } else {
            if (p_playerSlime->getSPD() > p_enemySlime->getSPD()){
                p_playerSlime->attack(playerSkill, p_enemySlime);
                if (p_enemySlime->getHP() <= 0) {
                    enemyChangeChoice = enemyPassiveChange(enemyTeam, enemySlimeChoice, p_playerSlime);
                    if (enemyChangeChoice == -1) break;
                    enemySlimeChoice = enemyChangeChoice;
                    p_enemySlime = &enemyTeam[enemySlimeChoice - 1];
                        
                } else {
                    p_enemySlime->attack(enemySkill, p_playerSlime);
                    if (p_playerSlime->getHP() <= 0) {
                        playerChangeChoice = playerPassiveChange(playerTeam, playerSlimeChoice);

                        if (playerChangeChoice == -1) break;

                        playerSlimeChoice = playerChangeChoice;
                        p_playerSlime = &playerTeam[playerSlimeChoice - 1];
                    }
                }
            } else {
                p_enemySlime->attack(enemySkill, p_playerSlime);
                if (p_playerSlime->getHP() <= 0) {
                    playerChangeChoice = playerPassiveChange(playerTeam, playerSlimeChoice);

                    if (playerChangeChoice == -1) break;

                    playerSlimeChoice = playerChangeChoice;
                    p_playerSlime = &playerTeam[playerSlimeChoice - 1];
                        
                } else {
                    p_playerSlime->attack(playerSkill, p_enemySlime);
                    if (p_enemySlime->getHP() <= 0) {
                        enemyChangeChoice = enemyPassiveChange(enemyTeam, enemySlimeChoice, p_playerSlime);
                        if (enemyChangeChoice == -1) break;
                        enemySlimeChoice = enemyChangeChoice;
                        p_enemySlime = &enemyTeam[enemySlimeChoice - 1];
                    }
                }
            }
        }

        if (totalHP(playerTeam) == 0 || totalHP(enemyTeam) == 0 || roundCount > 100) {
            gameOver = true;
        }
        // 回合结束
        roundCount++;
        
    }

// 战斗结束
    if (roundCount > 100) {
        cout << "Draw" << endl;
    } else if (totalHP(playerTeam) > 0) {
        cout << "You Win" << endl;
    } else {
        cout << "You Lose" << endl;
    }
}