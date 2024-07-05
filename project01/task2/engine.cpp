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
    if ((*pp_self)->getHP() > 0) {
        return;
    }
    // 被击倒
    printPlayerBeaten(*pp_self);
    int alive = 0;
    switch (slimeAlive(playerTeam))
    {
    case 1:
        alive = findAlive(playerTeam, playerSlimeChoice);
        playerSlimeChoice = selectSlimeFrom1(playerTeam, alive);
        break;
    case 2:
        int Slime1, Slime2;
        getSlimeRemain(Slime1, Slime2, playerSlimeChoice);
        playerSlimeChoice = selectSlimeFrom2(playerTeam, Slime1, Slime2);
        break;
    default:
        return;
        break;
    }
    *pp_self = &playerTeam[playerSlimeChoice - 1];
    printSend(*pp_self);
}

void enemyPassiveChange(Slime *enemyTeam, int &enemySlimeChoice, Slime *p_rival, Slime **pp_self) {
    // 没有被击倒
    if ((*pp_self)->getHP() > 0) {
        return;
    }
    // 被击倒
    printEnemyBeaten(*pp_self);
    int choice = 0, alive = 0;
    switch (slimeAlive(enemyTeam))
    {
    case 1:
        alive = findAlive(enemyTeam, enemySlimeChoice);
        choice = alive;
        break;
    case 2:
        int Slime1, Slime2;
        getSlimeRemain(Slime1, Slime2, enemySlimeChoice);
        if (isTypeAdvantage(enemyTeam[Slime1 - 1].getType(), p_rival->getType())) {
            choice = Slime1;
        } else if (isTypeAdvantage(enemyTeam[Slime2 - 1].getType(), p_rival->getType())) {
            choice = Slime2;
        } else if (enemyTeam[Slime1 - 1].getType() == p_rival->getType()) {
            choice = Slime1;
        } else {
            choice = Slime2;
        }
        break;
    default:
        return;
        break;
    }
    enemySlimeChoice = choice;
    *pp_self = &enemyTeam[enemySlimeChoice - 1];
    printSend(*pp_self);
}

void enemyActiceChange(Slime *enemyTeam, int &enemySlimeChoice, Slime *p_rival, Slime **pp_self) {
    // 没有可交换的史莱姆
    if (slimeAlive(enemyTeam) <= 1) {
        return;
    }
    // 有可交换的史莱姆
    int choice = -1;

    // 场下存活史莱姆有克制玩家的
    for (int i = 0; i < 3; i++) {
        if (enemySlimeChoice != i + 1
            && isTypeAdvantage(enemyTeam[i].getType(), p_rival->getType())
            && enemyTeam[i].getHP() > 0) {
            choice = i + 1;
            break;
        }
    }

    // 场下史莱姆没有克制玩家的，选择相同属性的
    if (choice == -1) {
        for (int i = 0; i < 3; i++) {
            if (enemySlimeChoice != i + 1
                && enemyTeam[i].getType() == p_rival->getType()
                && enemyTeam[i].getHP() > 0) {
                choice = i + 1;
                break;
            }
        }
    }
    
    // 未能交换
    if (choice == -1) {
        return;
    }

    enemySlimeChoice = choice;
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
    bool isEnemyChange = false;
    bool gameOver = false;
    while (!gameOver) {
        // 回合开始
        printHP(p_playerSlime, p_enemySlime);
        printRound(roundCount);

        /*
        // 输出player和emeny队伍中存活的slime名称
        cout << "Player's team alive:" << endl;
        for (int i = 0; i < 3; i++) {
            if (playerTeam[i].getHP() > 0) {
                cout << playerTeam[i].getName() << " ";
            }
        }
        cout << endl;

        cout << "Enemy's team alive:" << endl;
        for (int i = 0; i < 3; i++) {
            if (enemyTeam[i].getHP() > 0) {
                cout << enemyTeam[i].getName() << " ";
            }
        }
        cout << endl;
        */

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
        
        
        } else {
            // 选择交换
            if (slimeAlive(playerTeam) == 2){
                // 只有一个能交换
                int tempAlive = findAlive(playerTeam, playerSlimeChoice);
                playerChangeChoice = selectSlimeFrom1(playerTeam, tempAlive);
            } else {
                // 两个能交换
                int Slime1, Slime2;
                getSlimeRemain(Slime1, Slime2, playerSlimeChoice);
                playerChangeChoice = selectSlimeFrom2(playerTeam, Slime1, Slime2);
            } 
        }

        // 贪心敌人（2）选择行动
        // 选择交换
        int enemyChangeChoice = enemySlimeChoice;
        isEnemyChange = false;

        // 场下有史莱姆存活
        if (slimeAlive(enemyTeam) > 1) {
            // 被玩家克制
            if (isTypeAdvantage(p_playerSlime->getType(), p_enemySlime->getType())) {
                isEnemyChange = true;
            }
            // 场下存活史莱姆有克制玩家的
            for (int i = 0; i < 3; i++) {
                if (enemySlimeChoice != i + 1
                    && isTypeAdvantage(enemyTeam[i].getType(), p_playerSlime->getType())
                    && enemyTeam[i].getHP() > 0) {
                    isEnemyChange = true;
                    break;
                }
            }
        }
        
        // 选择技能
        SkillEnum enemySkill = TACKLE;
        if (!isEnemyChange) {
            if (isTypeAdvantage(p_enemySlime->getType(), p_playerSlime->getType())) {
                enemySkill = p_enemySlime->getSkill(2);
            } else {
                enemySkill = p_enemySlime->getSkill(1);
            }
        }

        // 实施行动
        /*
        if (playerActionChoice == 2) {
            // 玩家选择交换
            playerSlimeChoice = playerChangeChoice;
            p_playerSlime = &playerTeam[playerSlimeChoice - 1];
            printSend(p_playerSlime);

            if (isEnemyChange) {
                // 敌人选择交换
                enemyActiceChange(enemyTeam, enemySlimeChoice, p_playerSlime, &p_enemySlime);
            } else {
                // 敌人选择攻击
                p_enemySlime->attack(enemySkill, p_playerSlime);
                playerPassiveChange(playerTeam, playerSlimeChoice, &p_playerSlime);
            }
        } else {
            // 玩家选择攻击
            if (isEnemyChange) {
                // 敌人选择交换
                enemyActiceChange(enemyTeam, enemySlimeChoice, p_playerSlime, &p_enemySlime);
                p_playerSlime->attack(playerSkill, p_enemySlime);
                enemyPassiveChange(enemyTeam, enemySlimeChoice, p_playerSlime, &p_enemySlime);
            } else {
                // 敌人选择攻击
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
        }
        */
        if (isEnemyChange) {
            // 敌人选择交换
            if (playerActionChoice == 2) {
                // 玩家选择交换
                enemyActiceChange(enemyTeam, enemySlimeChoice, p_playerSlime, &p_enemySlime);

                playerSlimeChoice = playerChangeChoice;
                p_playerSlime = &playerTeam[playerSlimeChoice - 1];
                
                printSend(p_playerSlime);
                printSend(p_enemySlime);
            } else {
                // 玩家选择攻击
                enemyActiceChange(enemyTeam, enemySlimeChoice, p_playerSlime, &p_enemySlime);
                printSend(p_enemySlime);
                p_playerSlime->attack(playerSkill, p_enemySlime);
                enemyPassiveChange(enemyTeam, enemySlimeChoice, p_playerSlime, &p_enemySlime);
            }
        } else {
            // 敌人选择攻击
            if (playerActionChoice == 2) {
                // 玩家选择交换
                playerSlimeChoice = playerChangeChoice;
                p_playerSlime = &playerTeam[playerSlimeChoice - 1];
                printSend(p_playerSlime);
                
                p_enemySlime->attack(enemySkill, p_playerSlime);
                playerPassiveChange(playerTeam, playerSlimeChoice, &p_playerSlime);
            } else {
                // 玩家选择攻击
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
        }
        
        if (slimeAlive(playerTeam) == 0 || slimeAlive(enemyTeam) == 0 || roundCount > 100) {
            gameOver = true;
        }
        // 回合结束
        roundCount++;
        
    }

    // 战斗结束
    if (roundCount > 100) {
        cout << "Draw" << endl;
    } else if (slimeAlive(playerTeam) > 0) {
        cout << "You Win" << endl;
    } else {
        cout << "You Lose" << endl;
    }
}