#include "unit.h"
#include "iomsg.h"

Skill::Skill(std::string name, TypeEnum type, int power) : 
    name(name), type(type), power(power) {}

std::string Skill::getName() const {
    return name;
}

int Skill::getPower() const {
    return power;
}

TypeEnum Skill::getType() const {
    return type;
}

double typeBonus(TypeEnum type1, TypeEnum type2);

Slime::Slime(std::string name, int maxHP, int ATK, int DEF, int SPD, bool sd,
    TypeEnum type, Skill *p_skill1, Skill *p_skill2) :
        name(name), maxHP(maxHP), HP(maxHP), ATK(ATK), DEF(DEF), SPD(SPD), side(sd),
        type(type), p_skill1(p_skill1), p_skill2(p_skill2)
{}

std::string Slime::getName() const {return name;}

int Slime::getMaxHP() const {return maxHP;}

int Slime::getHP() const {return HP;}

void Slime::setHP(int newHP) {HP = newHP;}

int Slime::getATK() const {return ATK;}

int Slime::getDEF() const {return DEF;}

int Slime::getSPD() const {return SPD;}

bool Slime::isEnemy() const {return side;}

TypeEnum Slime::getType() {return type;}

Skill* Slime::getSkill(int n) {
    if (n == 1) {
        return p_skill1;
    }
    else if (n == 2) {
        return p_skill2;
    }
    else {
        return nullptr;
    }
}

int max(int a, int b) {
    if (a > b) {
        return a;
    }
    else {
        return b;
    }
}


bool isTypeAdvantage(TypeEnum selfType, TypeEnum rivalType) {
    return (selfType == GRASS && rivalType == WATER) ||
        (selfType == WATER && rivalType == FIRE) ||
        (selfType == FIRE && rivalType == GRASS);
}


double typeBonus(TypeEnum attackType, TypeEnum slimeType) {
    if (isTypeAdvantage(attackType, slimeType)) {
        return 2.0;
    } else {
        return 0.5;
    }
}
