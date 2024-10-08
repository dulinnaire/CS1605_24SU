#ifndef ACTION_H_INCLUDED
#define ACTION_H_INCLUDED

#include <string>
#include "unit.h"

// DEBUG
#include <iostream>

class Action {
public:
    // 实施行动
    virtual void act() {
        std::cout << "Action Class called" << std::endl;
    }

    // 获取行动名称
    virtual std::string getName() = 0;

    // 获取优先级
    virtual int getPriority() const = 0;
private:

};

// 实施行动
void roundAct(Action *p_playerAction, Action *p_enemyAction,
    Slime *p_playerSlime, Slime *p_enemySlime);


class Attack : public Action {
public:
    // Constructor
    Attack(Skill *p_skill, Slime **pp_self, Slime **pp_rival,
        Slime *playerTeam, Slime *enemyTeam,
        int &playerSlimeChoice, int &enemySlimeChoice);

    // 实施行动
    void act(/*args*/);

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
};


class Change : public Action {
public:
    // Constructor
    Change(int choice, Slime *selfTeam, Slime *rivalTeam, Slime **pp_self, Slime *p_rivalCurr);

    // 实施行动
    void act(/*args*/);

    std::string getName();
    int getPriority() const;
private:
    int choice;
    Slime *selfTeam;
    Slime *rivalTeam;
    Slime **pp_self;
    Slime *p_rivalCurrent;
};


class Potion : public Action {
public:
    // Constructor
    Potion(/*args*/);

    // 实施行动
    void act(/*args*/);

    std::string getName();
    int getPriority() const;
private:

};



int slimeAlive(Slime *team);

int findAlive(Slime *team, int curr);

void getSlimeRemain(int &S1, int &S2, int curr);

#endif