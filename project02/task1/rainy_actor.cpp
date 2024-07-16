#include "actor.h"
#include "battlefield.h"


RainyActor::RainyActor() {
    this->attr["revivalPotion"] = 1;
}

std::vector<Pet_T> RainyActor::ChooseStartingPet() {
    // TODO: 返回雨天队的出场宠物
    // 均匀随机选择首发宠物，例如可以将GBY的列表随机打乱，则各宠物首发概率相同

    // TODO: 打乱

    return {Pet_T::G, Pet_T::B, Pet_T::Y};
}

Action_T RainyActor::ChooseAction() {
    // TODO: 返回选取的动作


    return Action_T::Skill;
}

Pet_T RainyActor::ChoosePet(bool active) {
    // TODO: 返回即将交换上场的宠物
    if (active) {

    } else {
        
    }

    return Pet_T::G;
}

Skill_T RainyActor::ChooseSkill() {
    // TODO: 选择场上宠物使用的技能


    return Skill_T::Tackle;
}


PotionInfo RainyActor::ChoosePotion() {
    // TODO: 返回使用的药剂
    return PotionInfo{Potion_T::Revival, Pet_T::G};
}

void RainyActor::usePotion(PotionInfo potionInfo) {
    // TODO: 使用药剂
}

std::string RainyActor::GetName() {
    return "Enemy";//"Rainy Team";
}

