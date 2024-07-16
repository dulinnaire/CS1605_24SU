#include "actor.h"

#include <sstream>
#include <unordered_set>


HumanActor::HumanActor() {
    this->GetName();
}

std::vector<Pet_T> HumanActor::ChooseStartingPet() {
    // TODO: 从用户输入中得到上场的宠物
    
    std::vector<int> petIndex = {1, 2, 3, 4, 5};
    LOG("You can select three from five (1 for Green, 2 for Red, 3 for Blue, 4 for Yellow, 5 for Pink).\n");
    
    int choice;
    for (int i = 0; i < 3; i++) {
        while (true) {
            switch (i)
            {
            case 0:
                LOG("Select your 1st slime: ");
                break;
            case 1:
                LOG("Select your 2nd slime: ");
                break;
            case 2:
                LOG("Select your 3rd slime: ");
                break;
            default:
                break;
            }
            std::cin >> choice;
            if (std::find(petIndex.begin(), petIndex.end(), choice) != petIndex.end()) {
                this->allPetTs.push_back(Pet_T(choice));
                petIndex.erase(std::find(petIndex.begin(), petIndex.end(), choice));
                break;
            }
        }
    }
    this->availPetTs = this->allPetTs;
    this->availPetTs.erase(availPetTs.begin());
    return this->allPetTs;
}

Action_T HumanActor::ChooseAction() {
    // TODO: 返回选取的动作
    int choice;
    if (this->availPets.size() == 0) {
        // No pet available
        while (true) {
            LOG("Select your action (0 for escape, 1 for skill): ");
            std::cin >> choice;
            if (choice == 0) {
                return Action_T::Escape;
            } else if (choice == 1) {
                return Action_T::Skill;
            }
        }
    } else {
        while (true) {
            LOG("Select your action (0 for escape, 1 for skill, 2 for change): ");
            std::cin >> choice;
            if (choice == 0) {
                return Action_T::Escape;
            } else if (choice == 1) {
                return Action_T::Skill;
            } else if (choice == 2) {
                return Action_T::Change;
            }
        }
    }
}

Pet_T HumanActor::ChoosePet(bool active) {
    // TODO: 返回即将交换上场的宠物
    // active 代表<主动交换宠物>or<场上宠物阵亡后被动交换宠物>

    // 同步 availPetTs 和 availPets
    this->availPetTs.clear();
    for (auto it = this->availPets.begin(); it != this->availPets.end(); it++) {
        this->availPetTs.push_back(it->first);
    }

    int choice = 1;
    if (active) {
        std::vector<int> availIndex = {};

        if (this->availPets.size() == 1) {
            availIndex.push_back(1 + std::distance(this->allPetTs.begin(),
                std::find(this->allPetTs.begin(), this->allPetTs.end(), this->availPetTs[0])));
        } else {
            availIndex.push_back(1 + std::distance(this->allPetTs.begin(),
                std::find(this->allPetTs.begin(), this->allPetTs.end(), this->availPetTs[0])));
            availIndex.push_back(1 + std::distance(this->allPetTs.begin(),
                std::find(this->allPetTs.begin(), this->allPetTs.end(), this->availPetTs[1])));
            std::sort(availIndex.begin(), availIndex.end()); // 从小到大排序
        }
        while (true) {
            if (this->availPets.size() == 1) {
                LOG("Select your next slime (0 for back,",
                    availIndex[0],
                    "for", availPets.at(this->allPetTs[availIndex[0] - 1]).petName + "-HP"
                    + std::to_string(availPets.at(this->allPetTs[availIndex[0] - 1]).health) + "): ");
            } else {
                LOG("Select your next slime (0 for back,",
                    availIndex[0],
                    "for", availPets.at(this->allPetTs[availIndex[0] - 1]).petName + "-HP"
                    + std::to_string(availPets.at(this->allPetTs[availIndex[0] - 1]).health) + ",",
                    availIndex[1],
                    "for", availPets.at(this->allPetTs[availIndex[1] - 1]).petName + "-HP"
                    + std::to_string(availPets.at(this->allPetTs[availIndex[1] - 1]).health) + "): ");
            }
            std::cin >> choice;
            if (choice == 0) {
                this->shouldReturnToChooseAction = true;
                return this->petOnCourt.pet;
            } else if (std::find(availIndex.begin(), availIndex.end(), choice) != availIndex.end()){
                return this->allPetTs[choice - 1];
            }
        }
    } else {
        std::vector<int> availIndex = {};

        if (this->availPets.size() == 1) {
            availIndex.push_back(1 + std::distance(this->allPetTs.begin(),
                std::find(this->allPetTs.begin(), this->allPetTs.end(), this->availPetTs[0])));
        } else {
            availIndex.push_back(1 + std::distance(this->allPetTs.begin(),
                std::find(this->allPetTs.begin(), this->allPetTs.end(), this->availPetTs[0])));
            availIndex.push_back(1 + std::distance(this->allPetTs.begin(),
                std::find(this->allPetTs.begin(), this->allPetTs.end(), this->availPetTs[1])));
            std::sort(availIndex.begin(), availIndex.end());
        }
        while (true) {
            if (this->availPets.size() == 1) {
                LOG("Select your next slime (" + std::to_string(availIndex[0]),
                    "for", availPets.at(this->allPetTs[availIndex[0] - 1]).petName + "-HP"
                    + std::to_string(availPets.at(this->allPetTs[availIndex[0] - 1]).health) + "): ");
            } else {
                LOG("Select your next slime (" + std::to_string(availIndex[0]),
                    "for", availPets.at(this->allPetTs[availIndex[0] - 1]).petName + "-HP"
                    + std::to_string(availPets.at(this->allPetTs[availIndex[0] - 1]).health) + ",",
                    availIndex[1],
                    "for", availPets.at(this->allPetTs[availIndex[1] - 1]).petName + "-HP"
                    + std::to_string(availPets.at(this->allPetTs[availIndex[1] - 1]).health) + "): ");
            }
            std::cin >> choice;
            if (std::find(availIndex.begin(), availIndex.end(), choice) != availIndex.end()){
                return this->allPetTs[choice - 1];
            }
        }
    }
}

Skill_T HumanActor::ChooseSkill() {
    // TODO: 选择场上宠物使用的技能
    int choice = 1;
    while (true) {
        LOG("Select the skill (0 for back, 1 for", allSkills.at(this->petOnCourt.skills[0]).skillName
            + ", 2 for", allSkills.at(this->petOnCourt.skills[1]).skillName
            + ", 3 for", allSkills.at(this->petOnCourt.skills[2]).skillName
            + "): ");
        std::cin >> choice;
        if (choice == 0) {
            this->shouldReturnToChooseAction = true;
            return Skill_T::Tackle;
        } else {
            return this->petOnCourt.skills[choice - 1];
        }
    }
}

std::string HumanActor::GetName() {
    return "You";
}
