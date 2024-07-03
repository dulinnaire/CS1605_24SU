#include <iostream>
#include "engine.h"
#include "unit.h"

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


void printHP(Slime *p_playerSlime, Slime *p_enemySlime) {
    cout << "Your " << p_playerSlime->getName()
        << ": HP " << p_playerSlime->getHP() <<" || Enemy's "
        << p_enemySlime->getName() << ": HP "
        << p_enemySlime->getHP() << endl;
}

int playerPassiveChange(Slime *team, int curr) {
    cout << "Your " << team[curr - 1].getName() << " is beaten" << endl;
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
        while (choice != alive) {
            cout << "Select your next slime (" << alive << " for " << team[alive - 1].getName() << "): ";
            cin >> choice;
        }
        cout << "You send " << team[choice - 1].getName() << endl;
        return choice;
        break;
    case 2:
        int Slime1, Slime2;
        getSlimeRemain(Slime1, Slime2, curr);
        while (choice != Slime1 && choice != Slime2) {
            cout << "Select your next slime (" << Slime1 << " for " << team[Slime1 - 1].getName() << ", "
                << Slime2 << " for " << team[Slime2 - 1].getName() << "): ";
            cin >> choice;
        }
        cout << "You send " << team[choice - 1].getName() << endl;
        return choice;
        break;
    default:
        return -1;
        break;
    }
}

int enemyPassiveChange(Slime *team, int curr, Slime *rival) {
    cout << "Enemy's " << team[curr - 1].getName() << " is beaten" << endl;
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
        cout << "Enemy sends " << team[alive - 1].getName() << endl;
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
        cout << "Enemy sends " << team[choice - 1].getName() << endl;
        return choice;
    default:
        return -1;
    }
}

void play(Slime *playerTeam, Slime *enemyTeam) {
// 战斗开始前
    // 初始信息
    cout << "Welcome to Battle of Slimes!" << endl;
    cout << "You have Green, Red and Blue. So does Enemy." << endl;
    
    // 玩家选择Slime
    int playerSlimeChoice = 0;
    while (playerSlimeChoice < 1 || playerSlimeChoice > 3) {
        cout << "Select your starting slime (1 for Green, 2 for Red, 3 for Blue): ";
        cin >> playerSlimeChoice;
    }
    Slime *p_playerSlime = &playerTeam[playerSlimeChoice - 1];
    cout << "You start with " << p_playerSlime->getName() << endl;

    // 敌人选择Slime(选择属性克制的)
    int enemySlimeChoice = playerSlimeChoice % 3 + 1;
    Slime *p_enemySlime = &enemyTeam[enemySlimeChoice - 1];
    cout << "Enemy starts with " << p_enemySlime->getName() << endl;

// 战斗过程中
    cout << "Battle starts!" << endl;
    int roundCount = 1;
    bool gameOver = false;
    while (!gameOver) {
        // 回合开始
        printHP(p_playerSlime, p_enemySlime);
        cout << "--------------------------------------------------" << endl;
        cout << "Round " << roundCount << endl;

        // 选择行动
        // 玩家选择行动
        int playerActionChoice = 0;
    
        if (slimeAlive(playerTeam) == 1) {
            while (playerActionChoice != 1) {
                cout << "Select your action (1 for skill): ";
                cin >> playerActionChoice;
            }
        } else {
            while (playerActionChoice != 1 && playerActionChoice != 2) {
                cout << "Select your action (1 for skill, 2 for change): ";
                cin >> playerActionChoice;
            }
        }

        int playerChangeChoice = playerSlimeChoice, enemyChangeChoice = enemySlimeChoice;
        // 选择技能
        SkillEnum playerSkill = TACKLE;
        if (playerActionChoice == 1) {
            int playerSkillChoice = 0;
            while (playerSkillChoice != 1 && playerSkillChoice != 2) {
                cout << "Select the skill (1 for " << getSkillName(p_playerSlime->getSkill(1))
                    << ", 2 for " << getSkillName(p_playerSlime->getSkill(2)) << "): ";
                cin >> playerSkillChoice;
            }
            playerSkill = p_playerSlime->getSkill(playerSkillChoice);
        
        // 选择交换
        } else {
            // 只有一个能交换
            if (slimeAlive(playerTeam) == 2){
                int temp;
                for (int i = 0; i < 3; i++) {
                    if (i + 1 != playerSlimeChoice && playerTeam[i].getHP() != 0) {
                        temp = i + 1;
                        break;
                    }
                }
                while (playerChangeChoice != temp) {
                    cout << "Select your next slime (" << temp << " for " << playerTeam[temp - 1].getName() << "): ";
                    cin >> playerChangeChoice;
                }
            // 两个能交换
            } else {
                int Slime1, Slime2;
                getSlimeRemain(Slime1, Slime2, playerSlimeChoice);
                while (playerChangeChoice != Slime1 && playerChangeChoice != Slime2) {
                    cout << "Select your next slime ("
                        << Slime1 << " for " << playerTeam[Slime1 - 1].getName() << ", "
                        << Slime2 << " for " << playerTeam[Slime2 - 1].getName() << "): ";
                    cin >> playerChangeChoice;
                }
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
            cout << "You send " << p_playerSlime->getName() << endl;
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