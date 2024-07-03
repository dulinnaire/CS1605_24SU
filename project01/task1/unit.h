#ifndef UNIT_H_INCLUDED
#define UNIT_H_INCLUDED

#include <string>

enum TypeEnum {GRASS, WATER, FIRE};
enum SkillEnum {TACKLE, LEAF, FLAME, STREAM};

class Slime {
public:
    Slime(std::string name, int HP, int ATK, int DEF, int SPD, bool sd,
        TypeEnum type, SkillEnum skill1, SkillEnum skill2)
        : name(name), HP(HP), ATK(ATK), DEF(DEF), SPD(SPD), side(sd),
            type(type), skill1(skill1), skill2(skill2)
    {}

    std::string getName() const;
    int getHP() const;
    void setHP(int newHP);
    int getATK() const;
    int getDEF() const;
    int getSPD() const;
    bool isEnemy() const;
    TypeEnum getType();
    SkillEnum getSkill(int n);

    void attack(SkillEnum skill, Slime *rival);

private:
    std::string name;
    int HP, ATK, DEF, SPD;
    bool side;
    TypeEnum type;
    SkillEnum skill1, skill2;
};

std::string getSkillName(SkillEnum skill);

bool isTypeAdvantage(TypeEnum selfType, TypeEnum rivalType);

#endif