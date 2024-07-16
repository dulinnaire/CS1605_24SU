#include "actor.h"
#include "battlefield.h"


std::vector<Pet_T> SunnyActor::ChooseStartingPet() {
    // TODO: 返回晴天队的出场宠物
    // 晴天队有PGR，永远首发P
    return {Pet::P, Pet::G, Pet::R};
}

Action_T SunnyActor::ChooseAction() {
    // TODO: 返回选取的动作
    switch (this->petOnCourt.pet)
    {
    case Pet_T::P:
        return Action_T::Skill;
        break;
    case Pet_T::G:
        if (this->petOnCourt.health < 0.5 * allPets.at(this->petOnCourt.pet).health
            && this->availPets.size() > 0) {
            return Action_T::Change;
        } else {
            return Action_T::Skill;
        }
        break;
    case Pet_T::R:
        return Action_T::Skill;
        break;
    default:
        return Action_T::Skill;
        break;
    }
}

Pet_T SunnyActor::ChoosePet(bool active) {
    // TODO: 返回即将交换上场的宠物
    if (active) {
        return Pet::R;
    } else {
        switch (this->petOnCourt.pet)
        {
        case Pet_T::P:
            return Pet::G;
            break;
        case Pet_T::R:
            return Pet::G;
            break;
        default:
            return this->petOnCourt.pet;
            break;
        }
    }
}

Skill_T SunnyActor::ChooseSkill() {
    // TODO: 选择场上宠物使用的技能
    switch (this->petOnCourt.pet)
    {
    case Pet_T::P:
        //当玩家的史莱姆已失去最大 HP 的 2/3 以上时
        if (this->opp->petOnCourt.health < 1.0 / 3 * allPets.at(this->opp->petOnCourt.pet).health
            && (this->opp->petOnCourt.attr["faint"] != 0 || this->opp->petOnCourt.attr["graffiti"] != 0)) {
            return Skill_T::Slap;
        } else if (this->opp->petOnCourt.attr["faint"] == 0){
            return Skill_T::Sing;
        } else if (this->opp->petOnCourt.attr["graffiti"] == 0){
            return Skill_T::Paint;
        } else {
            return Skill_T::Slap;
        }
        break;
    case Pet_T::G:
        if (this->petOnCourt.health >= 0.5 * allPets.at(this->petOnCourt.pet).health
            || (this->petOnCourt.health > this->opp->petOnCourt.health && this->availPets.size() == 0)) {
            if (this->opp->petOnCourt.type == Type_T::Water) {
                return Skill_T::Leaf;
            } else {
                return Skill_T::Tackle;
            }
        } else {
            return Skill_T::Synthesis;
        }
        break;
    case Pet_T::R:
        if (this->opp->petOnCourt.health < 0.2 * allPets.at(this->opp->petOnCourt.pet).health
            && (this->opp->petOnCourt.type == Type_T::Fire || this->opp->petOnCourt.type == Type_T::Water)) {
            return Skill_T::Claw;
        } else {
            if (this->field->attr["weather"] == Weather_T::Sunny) {
                return Skill_T::Flame;
            } else {
                return Skill_T::Sunny;
            }
        }
        break;
    default:
        return Skill_T::Tackle;
        break;
    }
}

std::string SunnyActor::GetName() {
    return "Enemy";//"Sunny Team";
}
