#ifndef UNIT_H_INCLUDED
#define UNIT_H_INCLUDED

#include <string>

enum TypeEnum {NORMAL, GRASS, WATER, FIRE};

class Skill {
public:
    // Constructor
    Skill(std::string name, TypeEnum type, int power);
    
    int getPower() const ;

    TypeEnum getType() const;

    std::string getName() const;
private:
    // 名称
    std::string name;

    // 属性
    TypeEnum type;

    // 威力
    int power;
};


class Slime {
public:
    // Constructor
    Slime(std::string name, int HP, int ATK, int DEF, int SPD, bool sd,
        TypeEnum type, Skill *p_skill1, Skill *p_skill2);

    std::string getName() const;
    int getHP() const;
    void setHP(int newHP);
    int getATK() const;
    int getDEF() const;
    int getSPD() const;
    bool isEnemy() const;
    TypeEnum getType();
    Skill* getSkill(int n);

    void attack(Skill *p_skill, Slime *rival);

private:
    std::string name;
    int HP, ATK, DEF, SPD;
    bool side;
    TypeEnum type;
    Skill *p_skill1, *p_skill2;
};


bool isTypeAdvantage(TypeEnum selfType, TypeEnum rivalType);

#endif