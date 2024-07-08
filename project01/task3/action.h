#ifndef ACTION_H_INCLUDED
#define ACTION_H_INCLUDED

#include <string>
#include "unit.h"

// 行动基类
class Action {
public:
    // 实施行动
    virtual void act() = 0;

    // 获取行动名称
    virtual std::string getName() = 0;

    // 获取优先级
    virtual int getPriority() const = 0;
private:

};

// 实施一回合行动
void roundAct(Action *p_playerAction, Action *p_enemyAction,
    Slime *p_playerSlime, Slime *p_enemySlime);


// 攻击
class Attack : public Action {
public:
    // Constructor
    Attack(Skill *p_skill, Slime **pp_self, Slime **pp_rival,
        Slime *playerTeam, Slime *enemyTeam,
        int &playerSlimeChoice, int &enemySlimeChoice,
        bool &playerUsedATKPotion, bool &enemyUsedATKPotion);

    // 实施行动
    void act();

    std::string getName();
    int getPriority() const;
private:
    Skill *p_skill;
    Slime **pp_self;
    Slime **pp_rival;
    Slime *playerTeam;
    Slime *enemyTeam;
    int &playerSlimeChoice;
    int &enemySlimeChoice;
    // 是否使用过ATK药水
    bool &playerUsedATKPotion;
    bool &enemyUsedATKPotion;
};


// 换人
class Change : public Action {
public:
    // Constructor
    Change(int choice, Slime *selfTeam, Slime *rivalTeam, Slime **pp_self, Slime *p_rivalCurr);

    // 实施行动
    void act();

    std::string getName();
    int getPriority() const;
private:
    int choice;
    Slime *selfTeam;
    Slime *rivalTeam;
    Slime **pp_self;
    Slime *p_rivalCurrent;
};


// 使用药水
class Potion : public Action {
public:
    // Constructor
    Potion(std::string potionType, bool &usedATKPotion, Slime *enemyTeam, Slime *p_enemySlime);

    // 实施行动
    void act();

    std::string getName();
    int getPriority() const;
private:
    std::string potionType;
    bool &usedATKPotion;
    Slime *enemyTeam;
    Slime *p_enemySlime;
};


// 剩余存活史莱姆数
int slimeAlive(Slime *team);

// 获取curr以外存活史莱姆的编号
int findAlive(Slime *team, int curr);

// 获取死亡史莱姆的编号（仅有一个史莱姆死亡）
int findDead(Slime *team);

// 一个史莱姆死亡后，获取剩余存活史莱姆下标
void getSlimeRemain(int &S1, int &S2, int curr);

#endif