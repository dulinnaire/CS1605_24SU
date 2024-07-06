#include <iostream>
#include "iomsg.h"


void printWelcome() {
    cout << "Welcome to Battle of Slimes!" << endl;
    cout << "You have Green, Red and Blue. So does Enemy." << endl;    
}

void pringStart(Slime *p_playerSlime, Slime *p_enemySlime) {
    cout << "You start with " << p_playerSlime->getName() << endl;
    cout << "Enemy starts with " << p_enemySlime->getName() << endl;
    cout << "Battle starts!" << endl;
}

void printRound(int roundCount) {
    cout << "--------------------------------------------------" << endl;
    cout << "Round " << roundCount << endl;
}

void printHP(Slime *p_playerSlime, Slime *p_enemySlime) {
    cout << "Your " << p_playerSlime->getName()
        << ": HP " << p_playerSlime->getHP() <<" || Enemy's "
        << p_enemySlime->getName() << ": HP "
        << p_enemySlime->getHP() << endl;
}

void printSend(Slime *p_slime) {
    if (p_slime->isEnemy()){
        cout << "Enemy sends " << p_slime->getName() << endl;
    } else {
        cout << "You send " << p_slime->getName() << endl;    
    }
}


void printBeaten(Slime *p_slime) {
    if (p_slime->isEnemy()) {
        cout << "Enemy's " << p_slime->getName() << " is beaten" << endl;
    } else {
        cout << "Your " << p_slime->getName() << " is beaten" << endl;
    }
}

void printAttack(Slime *p_slime, string skillName, int damage) {
    if (p_slime->isEnemy()){
        cout << "Enemy's " << p_slime->getName() << " uses " << skillName << "! Damage: " << damage << endl;
    } else {
        cout << "Your " << p_slime->getName() << " uses " << skillName << "! Damage: " << damage << endl;
    }
}

void printAtkPotion(Slime *p_slime) {
    cout << "Enemy uses Attack Potion on " << p_slime->getName() << endl;
}

void printRevivalPotion(Slime *p_slime) {
    cout << "Enemy uses Revival Potion on " << p_slime->getName() << endl;
}

int inputHelper(std::string prompt, int choice1) {
    int input = -1;
    while (input != choice1) {
        cout << prompt;
        cin >> input;
    }
    return input;
}

int inputHelper(std::string prompt, int choice1, int choice2) {
    int input = -1;
    while (input != choice1 && input != choice2) {
        cout << prompt;
        cin >> input;
    }
    return input;
}

int inputHelper(std::string prompt, int choice1, int choice2, int choice3) {
    int input = -1;
    while (input != choice1  && input != choice2 && input != choice3) {
        cout << prompt;
        cin >> input;
    }
    return input;
}

int firstSlimeChoice() {
    return inputHelper("Select your starting slime (1 for Green, 2 for Red, 3 for Blue): ", 1, 2, 3);
}

int selectActionFrom1() {
    return inputHelper("Select your action (1 for skill): ", 1);
}

int selectActionFrom2() {
    return inputHelper("Select your action (1 for skill, 2 for change): ", 1, 2);
}

int selectSkill(Slime *p_slime) {
    string prompt = "Select the skill (1 for " + (p_slime->getSkill(1))->getName()
        + ", 2 for " + (p_slime->getSkill(2))->getName() + "): ";
    return inputHelper(prompt, 1, 2);
}

int selectSlimeFrom1(Slime *team, int n) {
    string prompt = "Select your next slime (" + to_string(n) + " for " + team[n - 1].getName() + "): ";
    return inputHelper(prompt, n);
}

int selectSlimeFrom2(Slime *team, int a, int b) {
    string prompt = "Select your next slime ("
        + to_string(a) + " for " + team[a - 1].getName() + ", "
        + to_string(b) + " for " + team[b - 1].getName() + "): ";
    return inputHelper(prompt, a, b);
}