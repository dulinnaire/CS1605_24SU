#include "battlefield.h"

#include "define.h"
#include "helper.h"

#include <string>


BattleField::BattleField(int maxRounds): maxRounds_(maxRounds) {}

void BattleField::Reset() {
    this->rounds_ = 0;
    this->actors_[0] = this->actors_[1] = nullptr;
    this->attr.clear();
}

void BattleField::Run() {
    this->BeforeGame_();

    bool ending = false;
    while (!ending) {
        LOG("--------------------------------------------------\n");
        this->StateCheck_();
        LOG("Round", ++this->rounds_); LOG('\n');
        std::vector<ActionInfo> actions = {};
        do {
            actors_[0]->shouldReturnToChooseAction = false;
            actions = this->ChooseActionPhase_();
        } while (actors_[0]->shouldReturnToChooseAction);
        this->PerformActionPhase_(actions);
        this->HandleBeatenPetsPhase_();
        ending = this->ShowInformationPhase_();
    }LOG('\n'); // DEBUG LINE
}

void BattleField::SetActor(Actor *a, Actor *b) {
    this->actors_[0] = a; this->actors_[1] = b;
    a->opp = b; b->opp = a;
    a->field = b->field = this;
}

void BattleField::BeforeGame_() {
    // welcome messages
    LOG("Welcome to Battle of Slimes 2!\n");

    // Task1: 敌人没有药水
    this->attr.insert(std::pair<std::string, int>("revivalPotion", 0));

    // TODO: 游戏开始前，选择宠物
    std::vector<Pet_T> opponentSlimes = this->actors_[1]->ChooseStartingPet();

    this->actors_[1]->petOnCourt = allPets.at(opponentSlimes[0]);
    this->actors_[1]->availPets.insert({opponentSlimes[1], allPets.at(opponentSlimes[1])});
    this->actors_[1]->availPets.insert({opponentSlimes[2], allPets.at(opponentSlimes[2])});

    LOG(this->actors_[1]->GetName(), "has", allPets.at(opponentSlimes[0]).petName + ",",
        allPets.at(opponentSlimes[1]).petName, "and",
        allPets.at(opponentSlimes[2]).petName + ",",
        "starting with", this->actors_[1]->petOnCourt.petName + ".\n");
    

    std::vector<Pet_T> playerSlimes = this->actors_[0]->ChooseStartingPet();

    this->actors_[0]->petOnCourt = allPets.at(playerSlimes[0]);
    this->actors_[0]->availPets.insert({playerSlimes[1], allPets.at(playerSlimes[1])});
    this->actors_[0]->availPets.insert({playerSlimes[2], allPets.at(playerSlimes[2])});

    LOG(this->actors_[0]->GetName(), "have", allPets.at(playerSlimes[0]).petName + ",",
        allPets.at(playerSlimes[1]).petName, "and",
        allPets.at(playerSlimes[2]).petName + ",",
        "starting with", this->actors_[0]->petOnCourt.petName); LOG('\n');


    LOG(Whose(this->actors_[0]->GetName()), this->actors_[0]->petOnCourt.petName
        + ": HP", std::to_string(this->actors_[0]->petOnCourt.health), "||",
        Whose(this->actors_[1]->GetName()), this->actors_[1]->petOnCourt.petName
        + ": HP", std::to_string(this->actors_[1]->petOnCourt.health)); LOG('\n');
    
    // game start messages
    LOG("Battle starts!\n");
}

void FaintCheckHelper_(BattleField *f, Actor *actor) {
    Pet *p0 = &actor->petOnCourt;
    if (p0->attr["faintCnt"] > 1) {
        p0->attr["faintCnt"] = p0->attr["faintCnt"] - 1;
    } else if (p0->attr["faintCnt"] == 1) {
        p0->attr["faint"] = p0->attr["faintCnt"] = 0;
        LOG(f->Whose(actor->GetName()), p0->petName, "wakes up"); LOG('\n');
    }
}

void BattleField::StateCheck_() {
    // TODO: 每回合开始前，检查天气、昏睡回合数等
    // weather
    if (this->attr["weatherCnt"] > 1) {
        this->attr["weatherCnt"] = this->attr["weatherCnt"] - 1;
    } else if (this->attr["weatherCnt"] == 1) {
        this->attr["weather"] = this->attr["weatherCnt"] = 0;
        LOG("Weather becomes normal\n");
    }

    // faint
    FaintCheckHelper_(this, this->actors_[0]);
    FaintCheckHelper_(this, this->actors_[1]);

    // 是否发动过技能
    this->actors_[0]->petOnCourt.attr["haveUsedSkill"] = 0;
    this->actors_[1]->petOnCourt.attr["haveUsedSkill"] = 0;
}

std::vector<ActionInfo> BattleField::ChooseActionPhase_() {
    // TODO: 向各Actor轮询采用的动作并返回

    Action_T humanAction_T = this->actors_[0]->ChooseAction(),
        enemyAction_T = this->actors_[1]->ChooseAction();
    
    ActionInfo humanAction, enemyAction;
    
    humanAction.action = humanAction_T;
    humanAction.actor = this->actors_[0];
    humanAction.priority = allActions.at(humanAction_T).priority;

    enemyAction.action = enemyAction_T;
    enemyAction.actor = this->actors_[1];
    enemyAction.priority = allActions.at(enemyAction_T).priority;

    switch (humanAction.action) {
        case 0: // Escape
            this->attr["escape"] = 1;
            break;
        case 1: // Skill
            humanAction.u.skill = this->actors_[0]->ChooseSkill();
            humanAction.priority = allSkills.at(humanAction.u.skill).priority;
            break;
        case 2: // Change
            humanAction.u.pet = this->actors_[0]->ChoosePet(true);
            break;
        case 3: // Potion 玩家没有药水
            // humanAction.u.potion = this->actors_[0]->ChoosePotion();
            break;
        default:
            break;
    }

    switch (enemyAction.action) {
        case 0: // Escape 敌人不能逃跑
            break;
        case 1: // Skill
            
            enemyAction.u.skill = this->actors_[1]->ChooseSkill();
            enemyAction.priority = allSkills.at(enemyAction.u.skill).priority;
            break;
        case 2: // Change
            enemyAction.u.pet = this->actors_[1]->ChoosePet(true);
            break;
        case 3: // Potion
            enemyAction.u.potion = this->actors_[1]->ChoosePotion();
            break;
        default:
            break;
    }

    std::vector<ActionInfo> actions = {humanAction, enemyAction};
    return actions;
}

void PerformActionHelper_(ActionInfo &actionInfo) {
    Actor *currActor = actionInfo.actor;
    if (currActor->field->attr["escape"] || currActor->petOnCourt.health == 0) {
        return;
    }
    switch (actionInfo.action) {
        case 0: // Escape
            currActor->petOnCourt.attr["haveUsedSkill"] = 0;
            break;
        case 1: // Skill
            // Sleeping时攻击无效
            if (currActor->petOnCourt.attr["faint"]) {
                currActor->petOnCourt.attr["haveUsedSkill"] = 0;
                LOG(currActor->field->Whose(currActor->GetName()),
                    currActor->petOnCourt.petName,
                    "is sleeping"); LOG('\n');
                return;
            }
            {
            Skill skillCopy = allSkills.at(actionInfo.u.skill);
            SkillCbReturn cbReturn = allSkills.at(actionInfo.u.skill).cb(
                currActor->field, currActor, &skillCopy); // 在callback函数中处理
            if (cbReturn.damage) {
                currActor->opp->petOnCourt.health = std::max(0,
                    currActor->opp->petOnCourt.health - cbReturn.damage);
            }
            LOG(currActor->field->Whose(currActor->GetName()),
                currActor->petOnCourt.petName,
                "uses", skillCopy.skillName + '!',
                cbReturn.prompt); LOG('\n');
            }
            break;
        case 2: // Change
            currActor->availPets.insert(
                std::pair<Pet_T, Pet>(currActor->petOnCourt.pet, currActor->petOnCourt));
            currActor->petOnCourt = currActor->availPets.at(actionInfo.u.pet);
            currActor->availPets.erase(actionInfo.u.pet);
            currActor->petOnCourt.attr["haveUsedSkill"] = 0;
            LOG(currActor->GetName(),
                currActor->field->WhoSend(currActor->GetName()),
                currActor->petOnCourt.petName); LOG('\n');
            break;
        case 3: // Potion
            currActor->usePotion(actionInfo.u.potion);
            currActor->petOnCourt.attr["haveUsedSkill"] = 0;
            break;
        default:
            break;
    }
}


void BattleField::PerformActionPhase_(std::vector<ActionInfo> &actions) {
    // TODO: 执行Actor选取的动作
    if (actions[0].priority > actions[1].priority
        || (actions[0].action == 1 && actions[1].action == 1
            && actions[0].priority == actions[1].priority
            && actions[0].actor->petOnCourt.speed > actions[1].actor->petOnCourt.speed)) {
        // 玩家先手 or 玩家和敌人同时行动，若同时使用skill，速度快的史莱姆先执行
        PerformActionHelper_(actions[0]);
        PerformActionHelper_(actions[1]);
    } else {
        // 敌人先手
        PerformActionHelper_(actions[1]);
        PerformActionHelper_(actions[0]);
    }
}

void HandleBeatenPetsHelper_(Actor *actor) {
    if (actor->petOnCourt.health <= 0) {
        LOG(actor->field->Whose(actor->GetName()), actor->petOnCourt.petName, "is beaten\n");
        actor->grave.insert(
            std::pair<Pet_T, Pet>(actor->petOnCourt.pet, actor->petOnCourt));
        if (actor->grave.size() == 3) {
            return;
        }
        Pet_T changePet = actor->ChoosePet(false);
        actor->petOnCourt = actor->availPets.at(changePet);
        actor->availPets.erase(changePet);
        actor->petOnCourt.attr["haveUsedSkill"] = 0;
        LOG(actor->GetName(),
            actor->field->WhoSend(actor->GetName()),
            actor->petOnCourt.petName); LOG('\n');
    }
}

void BattleField::HandleBeatenPetsPhase_() {
    // TODO: 检查并处理本轮中被击败的宠物（被动交换）
    if (this->attr["escape"]) {
        return;
    }
    HandleBeatenPetsHelper_(this->actors_[0]);
    HandleBeatenPetsHelper_(this->actors_[1]);
}

bool BattleField::ShowInformationPhase_() {
    // TODO: 显示剩余HP等信息，并返回游戏是否分出胜负
    // 游戏结束的情况：escape, lose, win, draw
    bool gameOver = false;

    // Escape
    if (this->attr["escape"]) {
        LOG("You Escape");
        gameOver = true;
    }
    // Win
    if (this->actors_[1]->grave.size() == 3) {
        LOG("You Win");
        gameOver = true;
    }
    // Lose
    if (this->actors_[0]->grave.size() == 3) {
        LOG("You Lose");
        gameOver = true;
    }

    // Show HP
    if (!gameOver) {
        LOG(Whose(this->actors_[0]->GetName()), this->actors_[0]->petOnCourt.petName
            + ": HP", std::to_string(this->actors_[0]->petOnCourt.health), "||",
            Whose(this->actors_[1]->GetName()), this->actors_[1]->petOnCourt.petName
            + ": HP", std::to_string(this->actors_[1]->petOnCourt.health)); LOG('\n');
    }
    
    // Draw
    if (rounds_ >= maxRounds_) {
        LOG("Draw");
        gameOver = true;    
    }

    return gameOver;
}

std::string BattleField::Whose(std::string name) {
    return (name == "You" ? "Your" : name + "'s");
}

std::string BattleField::WhoSend(std::string name) {
    return (name == "You" ? "send" : "sends");
}
