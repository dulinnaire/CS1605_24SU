#include "action.h"
#include "iomsg.h"

bool ROUNDEND = false;

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

int findDead(Slime *team) {
    for (int i = 0; i < 3; i++) {
        if (team[i].getHP() == 0) {
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

// 玩家被动交换史莱姆
void playerPassiveChange(Slime *playerTeam, int& playerSlimeChoice, Slime **pp_playerSlime) {
    printBeaten(*pp_playerSlime);
    int alive = 0;
    // 根据剩余史莱姆数量选择
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
    *pp_playerSlime = &playerTeam[playerSlimeChoice - 1];
    ROUNDEND = true;
    printSend(*pp_playerSlime);
}

void enemyPassiveChange(Slime *enemyTeam, int &enemySlimeChoice, Slime *p_playerSlime, Slime **pp_enemySlime) {
    printBeaten(*pp_enemySlime);
    int choice = 0, alive = 0;
    // 根据剩余史莱姆数量选择
    switch (slimeAlive(enemyTeam))
    {
    case 1:
        alive = findAlive(enemyTeam, enemySlimeChoice);
        choice = alive;
        break;
    case 2:
        // 优先选择克制玩家的史莱姆，其次选择不被玩家克制的史莱姆
        int Slime1, Slime2;
        getSlimeRemain(Slime1, Slime2, enemySlimeChoice);
        if (isTypeAdvantage(enemyTeam[Slime1 - 1].getType(), p_playerSlime->getType())) {
            choice = Slime1;
        } else if (isTypeAdvantage(enemyTeam[Slime2 - 1].getType(), p_playerSlime->getType())) {
            choice = Slime2;
        } else if (enemyTeam[Slime1 - 1].getType() == p_playerSlime->getType()) {
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
    *pp_enemySlime = &enemyTeam[enemySlimeChoice - 1];
    ROUNDEND = true;
    printSend(*pp_enemySlime);
}


Attack::Attack(Skill *p_skill, Slime **pp_self, Slime **pp_rival,
    Slime *playerTeam, Slime *enemyTeam,
    int &playerSlimeChoice, int &enemySlimeChoice,
    bool &playerUsedATKPotion, bool &enemyUsedATKPotion) :

    p_skill(p_skill),
    pp_self(pp_self),
    pp_rival(pp_rival),
    playerTeam(playerTeam),
    enemyTeam(enemyTeam),
    playerSlimeChoice(playerSlimeChoice),
    enemySlimeChoice(enemySlimeChoice),
    playerUsedATKPotion(playerUsedATKPotion),
    enemyUsedATKPotion(enemyUsedATKPotion)
{}

int Attack::getPriority() const {
    return 0;
}

std::string Attack::getName() {
    return "Attack";
}

// 四舍五入
int roundToInt(double num) {
    int intPart = num;
    if (num - intPart >= 0.5) {
        return intPart + 1;
    } else {
        return intPart;
    }
}

void Attack::act() {
    // 被击倒的史莱姆不能攻击，被动交换上场的史莱姆不能攻击
    if ((*pp_self)->getHP() == 0 || ROUNDEND == true) {
        return;
    }
    // 使用了强攻药
    int potionBonus = 1;
    if ((*pp_self)->isEnemy()) {
        if (enemyUsedATKPotion) {
            potionBonus = 2;
        }
    } else {
        if (playerUsedATKPotion) {
            potionBonus = 2;
        }
    }
    // 攻击
    double d_damage = 0;
    if (p_skill->getName() == "Tackle") {
        d_damage = double(p_skill->getPower()) * (*pp_self)->getATK() / (*pp_rival)->getDEF() * 1 * potionBonus;
    } else {
        d_damage = double(p_skill->getPower()) * (*pp_self)->getATK() / (*pp_rival)->getDEF()
            * typeBonus((*pp_self)->getType(), (*pp_rival)->getType()) * potionBonus;
    }
    int damage = roundToInt(d_damage);
    (*pp_rival)->setHP(max((*pp_rival)->getHP() - damage, 0));
    printAttack((*pp_self), p_skill->getName(), damage);
    
    // 判断敌方是否被击倒并需要被动交换，被动交换上场的史莱姆不能攻击
    if ((*pp_rival)->getHP() == 0) {
        if ((*pp_rival)->isEnemy()) {
            // 敌方被击倒，强攻药失效
            enemyUsedATKPotion = false;
            enemyPassiveChange(enemyTeam, enemySlimeChoice, *pp_self, pp_rival);
        } else {
            // 玩家被击倒，强攻药失效
            playerUsedATKPotion = false;
            playerPassiveChange(playerTeam, playerSlimeChoice, pp_rival);
        }
    }
}

Change::Change(int choice, Slime *selfTeam, Slime *rivalTeam, Slime **pp_self, Slime *p_rivalCurrent) :
    
    choice(choice),
    selfTeam(selfTeam),
    rivalTeam(rivalTeam),
    pp_self(pp_self),
    p_rivalCurrent(p_rivalCurrent)
{}

int Change::getPriority() const {
    return 6;
}

std::string Change::getName() {
    return "Change";
}

void Change::act() {
    *pp_self = &selfTeam[choice - 1];
    printSend(*pp_self);
}


Potion::Potion(std::string potionType, bool &usedATKPotion, Slime *enemyTeam, Slime *p_enemySlime) :
    
    potionType(potionType),
    usedATKPotion(usedATKPotion),
    enemyTeam(enemyTeam),
    p_enemySlime(p_enemySlime)
{}

int Potion::getPriority() const {
    return 5;
}

std::string Potion::getName() {
    return "Potion";
}

void Potion::act() {
    if (potionType == "Attack") {
        // 强攻药
        usedATKPotion = true;
        printAtkPotion(p_enemySlime);
    } else if (potionType == "Revival") {
        // 复苏药
        Slime *p_deadSlime = &(enemyTeam[findDead(enemyTeam) - 1]);
        p_deadSlime->setHP(p_deadSlime->getMaxHP() * 0.5);
        printRevivalPotion(p_deadSlime);
    } else {
        return;
    }
}

void roundAct(Action *p_playerAction, Action *p_enemyAction,
    Slime *p_playerSlime, Slime *p_enemySlime) {
    ROUNDEND = false;
    if (p_playerAction->getPriority() > p_enemyAction->getPriority()) {
        // 玩家先手
        p_playerAction->act();
        p_enemyAction->act();
    } else if (p_playerAction->getPriority() < p_enemyAction->getPriority()) {
        // 敌方先手
        p_enemyAction->act();
        p_playerAction->act();
    } else {
        // 优先级相同
        if (p_playerAction->getName() == "Attack") {
            if (p_playerSlime->getSPD() > p_enemySlime->getSPD()) {
                // 玩家先手
                p_playerAction->act();
                p_enemyAction->act();
            } else {
                // 敌方先手
                p_enemyAction->act();
                p_playerAction->act();
            }
        } else {
            // SPD相同
            p_playerAction->act();
            p_enemyAction->act();
        }
    }
}