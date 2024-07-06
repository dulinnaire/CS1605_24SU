#include <iostream>
#include "engine.h"

using namespace std;


void enemyActiceChange(Slime *enemyTeam, int &enemySlimeChoice, Slime *p_rival, Slime *p_self) {
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
        Action *p_playerAction = nullptr, *p_enemyAction = nullptr;

        Slime *p_playerSlimeCurr = p_playerSlime, *p_enemySlimeCurr = p_enemySlime;

        // 玩家选择行动
        int playerActionChoice = -1;
    
        if (slimeAlive(playerTeam) == 1) {
            playerActionChoice = selectActionFrom1();
        } else {
            playerActionChoice = selectActionFrom2();
        }

        int playerChangeChoice = playerSlimeChoice;
        // 选择技能
        Skill *p_playerSkill = nullptr;
        if (playerActionChoice == 1) {
            int playerSkillChoice = selectSkill(p_playerSlime);
            p_playerSkill = p_playerSlime->getSkill(playerSkillChoice);
            p_playerAction = new Attack(p_playerSkill, &p_playerSlime, &p_enemySlime,
                playerTeam, enemyTeam, playerSlimeChoice, enemySlimeChoice);
            
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
            playerSlimeChoice = playerChangeChoice;
            p_playerAction = new Change(playerSlimeChoice, playerTeam, enemyTeam,
                &p_playerSlime, p_enemySlimeCurr);
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
        
        if (isEnemyChange) {
            enemyActiceChange(enemyTeam, enemySlimeChoice, p_playerSlime, p_enemySlime);
            p_enemyAction = new Change(enemySlimeChoice, enemyTeam, playerTeam,
                &p_enemySlime, p_playerSlimeCurr);
        }

        // 选择技能
        Skill *p_enemySkill = nullptr;
        if (!isEnemyChange) {
            if (isTypeAdvantage(p_enemySlime->getType(), p_playerSlime->getType())) {
                p_enemySkill = p_enemySlime->getSkill(2);
            } else {
                p_enemySkill = p_enemySlime->getSkill(1);
            }
            p_enemyAction = new Attack(p_enemySkill, &p_enemySlime, &p_playerSlime,
                playerTeam, enemyTeam, playerSlimeChoice, enemySlimeChoice);
        }


        // 实施行动
        roundAct(p_playerAction, p_enemyAction, p_playerSlime, p_enemySlime);
        
        delete p_playerAction, p_enemyAction;

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