#include "engine.h"

using namespace std;


// 敌人主动交换史莱姆
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
    // 输出初始信息
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
    bool playerUsedATKPotion = false;
    // 在场敌人使用过强攻药
    bool enemyUsedATKPotion = false;
    bool gameOver = false;
    // 敌人药水数量
    int revivalPotionCount = 1, attackPotionCount = 2;

    while (!gameOver) {
        // 回合开始
        printHP(p_playerSlime, p_enemySlime);
        printRound(roundCount);

        // 选择行动
        Action *p_playerAction = nullptr, *p_enemyAction = nullptr;
        
        // 当前史莱姆（备份）
        Slime *p_playerSlimeCurr = p_playerSlime, *p_enemySlimeCurr = p_enemySlime;

        // 玩家选择行动
        int playerActionChoice = -1;
    
        // 根据剩余史莱姆数量选择行动
        if (slimeAlive(playerTeam) == 1) {
            playerActionChoice = selectActionFrom1();
        } else {
            playerActionChoice = selectActionFrom2();
        }

        // 选择技能
        Skill *p_playerSkill = nullptr;
        if (playerActionChoice == 1) {
            int playerSkillChoice = selectSkill(p_playerSlime);
            p_playerSkill = p_playerSlime->getSkill(playerSkillChoice);
            p_playerAction = new Attack(p_playerSkill, &p_playerSlime, &p_enemySlime,
                playerTeam, enemyTeam, playerSlimeChoice, enemySlimeChoice, playerUsedATKPotion, enemyUsedATKPotion);
            
        } else {
        // 选择交换
            if (slimeAlive(playerTeam) == 2){
                // 只有一个能交换
                int tempAlive = findAlive(playerTeam, playerSlimeChoice);
                playerSlimeChoice = selectSlimeFrom1(playerTeam, tempAlive);
            } else {
                // 两个能交换
                int Slime1, Slime2;
                getSlimeRemain(Slime1, Slime2, playerSlimeChoice);
                playerSlimeChoice = selectSlimeFrom2(playerTeam, Slime1, Slime2);
            } 
            p_playerAction = new Change(playerSlimeChoice, playerTeam, enemyTeam,
                &p_playerSlime, p_enemySlimeCurr);
        }

        // 喝药贪心敌人（3）选择行动

        // 选择喝药
        // 有史莱姆无法战斗时使用复苏药
        bool enemyUseRevivalPotion = false, enemyUseAttackPotion = false;
        if (slimeAlive(enemyTeam) != 3 && revivalPotionCount > 0) {
            enemyUseRevivalPotion = true;
            revivalPotionCount -= 1;
            p_enemyAction = new Potion("Revival", enemyUsedATKPotion, enemyTeam, p_enemySlime);
        }

        // 场上的史莱姆不被玩家场上的史莱姆克制时使用强攻药
        // 优先使用复苏药，不重复用药
        if (!enemyUseRevivalPotion
            && attackPotionCount > 0
            && !enemyUsedATKPotion
            && !isTypeAdvantage(p_playerSlime->getType(), p_enemySlime->getType())) {
            enemyUseAttackPotion = true;
            enemyUsedATKPotion = true;
            attackPotionCount -= 1;
            p_enemyAction = new Potion("Attack", enemyUsedATKPotion, enemyTeam, p_enemySlime);
        }

        // 敌人选择交换
        int enemyChangeChoice = enemySlimeChoice;
        isEnemyChange = false;

        // 场下有史莱姆存活
        if (slimeAlive(enemyTeam) > 1) {
            // 场上史莱姆被玩家克制
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

        // 敌方不会主动换下已具有强攻药效的史莱姆
        if (enemyUsedATKPotion) {
            isEnemyChange = false;
        }

        if (isEnemyChange) {
            enemyActiceChange(enemyTeam, enemySlimeChoice, p_playerSlime, p_enemySlime);
            p_enemyAction = new Change(enemySlimeChoice, enemyTeam, playerTeam,
                &p_enemySlime, p_playerSlimeCurr);
        }

        // 敌人选择技能
        Skill *p_enemySkill = nullptr;
        if (!isEnemyChange && !enemyUseRevivalPotion && !enemyUseAttackPotion) {
            if (isTypeAdvantage(p_enemySlime->getType(), p_playerSlime->getType())) {
                p_enemySkill = p_enemySlime->getSkill(2);
            } else {
                p_enemySkill = p_enemySlime->getSkill(1);
            }
            p_enemyAction = new Attack(p_enemySkill, &p_enemySlime, &p_playerSlime,
                playerTeam, enemyTeam, playerSlimeChoice, enemySlimeChoice, playerUsedATKPotion, enemyUsedATKPotion);
        }


        // 实施当前回合行动
        roundAct(p_playerAction, p_enemyAction, p_playerSlime, p_enemySlime);
        
        delete p_playerAction, p_enemyAction;

        // 有一方全部被击倒或超过100回合
        if (slimeAlive(playerTeam) == 0 || slimeAlive(enemyTeam) == 0 || roundCount >= 100) {
            // 游戏结束
            gameOver = true;
        }
        // 回合结束
        roundCount++;
        
    }

    // 战斗结束
    if (roundCount >= 100) {
        // 平局时输出一次血量信息
        printHP(p_playerSlime, p_enemySlime);
    }
    printEndGame(slimeAlive(playerTeam), roundCount);
}