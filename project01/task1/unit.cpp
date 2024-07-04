#include "unit.h"
#include "iomsg.h"

double typeBonus(TypeEnum type1, TypeEnum type2);
std::string Slime::getName() const {return name;}

int Slime::getHP() const {return HP;}

void Slime::setHP(int newHP) {HP = newHP;}

int Slime::getATK() const {return ATK;}

int Slime::getDEF() const {return DEF;}

int Slime::getSPD() const {return SPD;}

bool Slime::isEnemy() const {return side;}

TypeEnum Slime::getType() {return type;}

SkillEnum Slime::getSkill(int n) {
    if (n == 1) {
        return skill1;
    }
    else if (n == 2) {
        return skill2;
    }
    else {
        return TACKLE;
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

void Slime::attack(SkillEnum skill, Slime *rival) {
    double damage = 0;
    if (skill == TACKLE) {
        damage = 20 * ATK / rival->getDEF() * 1;
    } else {
        damage = 20 * ATK / rival->getDEF() * typeBonus(type, rival->getType());
    }
    rival->setHP(max(rival->getHP() - damage, 0));
    printAttack(this, getSkillName(skill), damage);
}

std::string getSkillName(SkillEnum skill) {
    switch (skill)
    {
    case 0:
        return "Tackle";
    case 1:
        return "Leaf";
    case 2:
        return "Flame";
    case 3:
        return "Stream";
    default:
        return "None";
    }
}

bool isTypeAdvantage(TypeEnum selfType, TypeEnum rivalType) {
    return (selfType == GRASS && rivalType == WATER) ||
        (selfType == WATER && rivalType == FIRE) ||
        (selfType == FIRE && rivalType == GRASS);
}


double typeBonus(TypeEnum attackType, TypeEnum slimeType){
    if (isTypeAdvantage(attackType, slimeType)) {
        return 2.0;
    } else {
        return 0.5;
    }
}
