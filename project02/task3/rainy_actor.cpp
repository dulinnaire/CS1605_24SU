#include "actor.h"
#include "battlefield.h"


RainyActor::RainyActor() {
    this->attr["revivalPotionCnt"] = 1;
}

std::vector<Pet_T> RainyActor::ChooseStartingPet() {
    // TODO: 返回雨天队的出场宠物
    // 均匀随机选择首发宠物，例如可以将GBY的列表随机打乱，则各宠物首发概率相同
    std::vector<Pet_T> ret = {Pet_T::G, Pet_T::B, Pet_T::Y};
    ShuffleVector(ret);
    return ret;
}

Action_T RainyActor::ChooseAction() {
    // TODO: 返回选取的动作
    switch (this->petOnCourt.pet)
    {
    case Pet_T::G:
        if (this->availPets.find(Pet_T::B) == this->availPets.end() && attr["revivalPotionCnt"] > 0) {
            // 小蓝被打倒，使用复苏药
            return Action_T::Potion;
        }
        if (this->opp->petOnCourt.type == Type_T::Fire
            && this->availPets.find(Pet_T::B) != this->availPets.end()) {
            // 小蓝HP不为0，换小蓝 
            return Action_T::Change;
        } else {
            return Action_T::Skill;   
        }
        break;
    case Pet_T::B:
        if ((this->field->attr["weather"] != Weather_T::Sunny
            && this->opp->petOnCourt.type == Type_T::Thunder)
            || (this->field->attr["weather"] == Weather_T::Rainy
            && this->opp->petOnCourt.type == Type_T::Grass)) {

            if (this->availPets.find(Pet_T::G) != this->availPets.end()) {
                return Action_T::Change;
            } else {
                if (this->field->attr["weather"] == Weather_T::Rainy
                    && this->availPets.find(Pet_T::Y) != this->availPets.end()) {
                    return Action_T::Change;
                } else {
                    return Action_T::Skill;
                }
            }
        } else {
            return Action_T::Skill;
        }
        break;
    case Pet_T::Y:
        if (this->availPets.find(Pet_T::B) == this->availPets.end() && attr["revivalPotionCnt"] > 0) {
            // 小蓝被打倒，使用复苏药
            return Action_T::Potion;
        }
        if (this->opp->petOnCourt.type == Type_T::Fire) {
            if (this->availPets.find(Pet_T::B) != this->availPets.end()) {
                return Action_T::Change;
            } else {
                return Action_T::Skill;
            }
        } else {
            if (this->field->attr["weather"] != Weather_T::Rainy
                && this->availPets.find(Pet_T::G) != this->availPets.end()) {
                return Action_T::Change;
            } else {
                return Action_T::Skill;
            }
        }
        break;
    default:
        break;
    }

    return Action_T::Skill;
}

Pet_T RainyActor::ChoosePet(bool active) {
    // TODO: 返回即将交换上场的宠物
    if (active) {
        switch (this->petOnCourt.pet)
        {
        case Pet_T::G:
            return Pet_T::B;
            break;
        case Pet_T::B:
            if (this->availPets.find(Pet_T::G) != this->availPets.end()) {
                return Pet_T::G;
            } else {
                return Pet_T::Y;
            }
            break;
        case Pet_T::Y:
            if (this->opp->petOnCourt.type == Type_T::Fire) {
                return Pet_T::B;
            } else {
                return Pet_T::G;
            }
            break;
        default:
            break;
        }
    } else {
        switch (this->petOnCourt.pet)
        {
        case Pet_T::G:
            return Pet_T::B;
            break;
        case Pet_T::B:
            return Pet_T::G;
            break;
        case Pet_T::Y:
            return Pet_T::B;
            break;
        default:
            break;
        }
    }

    return Pet_T::G;
}

Skill_T RainyActor::ChooseSkill() {
    // TODO: 选择场上宠物使用的技能
    switch (this->petOnCourt.pet)
    {
    case Pet_T::G:
        if (this->opp->petOnCourt.type == Type_T::Fire) {
            if (this->petOnCourt.health >= 2. / 3. * double(allPets.at(this->petOnCourt.pet).health)) {
                return Skill_T::Tackle;
            } else {
                return Skill_T::Synthesis;
            }
        } else if (this->opp->petOnCourt.type == Type_T::Grass) {
            if (this->petOnCourt.health >= 0.5 * double(allPets.at(this->petOnCourt.pet).health)) {
                return Skill_T::Tackle;
            } else {
                return Skill_T::Synthesis;
            }
        } else {
            if (this->petOnCourt.health >= 0.5 * double(allPets.at(this->petOnCourt.pet).health)
                || this->opp->petOnCourt.health < 0.2 * double(allPets.at(this->opp->petOnCourt.pet).health)) {
                return Skill_T::Leaf;
            } else {
                return Skill_T::Synthesis;
            }
        }
        break;
    case Pet_T::B:
        if (this->field->attr["weather"] == Weather_T::Sunny) {
            return Skill_T::Rainy;
        }
        
        if ((this->field->attr["weather"] != Weather_T::Sunny
            && this->opp->petOnCourt.type == Type_T::Thunder)
            || (this->field->attr["weather"] == Weather_T::Rainy
            && this->opp->petOnCourt.type == Type_T::Grass)
            && this->availPets.find(Pet_T::G) == this->availPets.end()) {

            if (!(this->field->attr["weather"] == Weather_T::Rainy
                && this->availPets.find(Pet_T::Y) != this->availPets.end())) {
                return Skill_T::Stream;
            }
        }

        if (this->field->attr["weather"] != Weather_T::Sunny
            && this->opp->petOnCourt.type == Type_T::Water) {
            return Skill_T::Tackle;
        }

        if (this->field->attr["weather"] == Weather_T::Rainy) {
            return Skill_T::Stream;
        } else {
            return Skill_T::Rainy;
        }
        break;
    case Pet_T::Y:
        if (this->opp->petOnCourt.health < 0.2 * double(allPets.at(this->opp->petOnCourt.pet).health)){
            return Skill_T::Quick;
        } else {
            if (this->petOnCourt.type == Type_T::Fire) {
                return Skill_T::Volt;
            } else {
                if (this->field->attr["weather"] == Weather_T::Rainy) {
                    return Skill_T::Thunder;
                } else {
                    if (this->opp->petOnCourt.type == Type_T::Thunder || this->opp->petOnCourt.type == Type_T::Grass) {
                        return Skill_T::Quick;
                    } else {
                        return Skill_T::Volt;
                    }
                }
            }
        }
        break;
    default:
        break;
    }

    return Skill_T::Tackle;
}


PotionInfo RainyActor::ChoosePotion() {
    // TODO: 返回使用的药剂
    return PotionInfo{Potion_T::Revival, Pet_T::B};
}

void RainyActor::usePotion(PotionInfo potionInfo) {
    // TODO: 使用药剂
    switch (potionInfo.potionT)
    {
    case Potion_T::Revival:
        this->attr["revivalPotionCnt"] = (std::max)(attr["revivalPotionCnt"] - 1, 0);
        this->grave.at(potionInfo.petT).health = int(0.5 * allPets.at(potionInfo.petT).health);
        this->availPets.insert(std::pair<Pet_T, Pet>(potionInfo.petT, grave.at(potionInfo.petT)));
        this->grave.erase(potionInfo.petT);
        break;
    case Potion_T::Attack:
        break;
    default:
        break;
    }
}

std::string RainyActor::GetName() {
    return "Enemy";//"Rainy Team";
}

