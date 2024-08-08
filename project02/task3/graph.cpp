#include "graph.h"

GraphSlime::GraphSlime(Actor* left, Actor* right)
    : player(left), enemy(right)
{
    // TODO: 初始化图像画面
    easy2d::Window::setSize(WINDOW_WIDTH, WINDOW_HEIGHT);

}

void GraphSlime::Update() {
    // TODO: 更新图像画面
    this->battle_scene = easy2d::gcnew easy2d::Scene;

    auto background = easy2d::gcnew easy2d::Sprite("Background.png");
    background->setPos(0, 0);

    battle_scene->addChild(background);

    player_slime = SetSlimeOnPitch(true, SLIME_X_PLAYER, SLIME_Y);
    enemy_slime = SetSlimeOnPitch(false, SLIME_X_ENEMY, SLIME_Y);

    battle_scene->addChild(player_slime);
    battle_scene->addChild(enemy_slime);

    easy2d::Timer::add([=]() {
        easy2d::Game::quit();  // 将控制权退出图形窗口，返回控制台（图形窗口仍存在）
    });

    easy2d::SceneManager::enter(battle_scene);
    easy2d::Game::start();
}

easy2d::Sprite* GraphSlime::SetSlimeOnPitch(bool PorE, float x, float y) {  // 参数PorE表示玩家或敌方
    // TODO: 设置史莱姆图片，调用设置HP图片和状态图标的函数
    // 史莱姆图片缩放 30%， 坐标 x=180(左)或 970(右)， y=500
    
    // 创建Slime Sprite
    Pet *pet;
    int HP_x = 0, HP_y = y + 200;
    int ICON_x = 0, ICON_y = y + 93;
    if (PorE) {
        pet = &(player->petOnCourt);
        HP_x = x + 85;
        ICON_x = x + 10;
    }
    else {
        pet = &(enemy->petOnCourt);
        HP_x = x + 85;
        ICON_x = x + 242;
    }
    auto slime = easy2d::gcnew easy2d::Sprite(GetSlimeImg(pet, PorE));
    slime->setPos(x, y);
    slime->setScale(0.3);

    // 设置HP与状态
    SetHPBar(slime, pet->health, allPets.at(pet->pet).health, HP_x, HP_y);

    if (pet->attr["faint"] || pet->attr["graffiti"]) {
        SetIcons(slime, pet->attr, ICON_x, ICON_y);
    }
    return slime;
}

void GraphSlime::SetHPBar(easy2d::Sprite* slimeImg, int hp, int hpMax, float x, float y) {
    // TODO: 设置HP图片（包括HP文字）
    // HP 图片缩放 300%，不透明度 60%，坐标 x+105(左)或 x-695(右)， y+200
    // HP 文字为默认字体， 字体大小 90， 加粗， 每行长度 380， 居中
    auto HPBar = easy2d::gcnew easy2d::Sprite("Menu.png");
    HPBar->setScale(0.9);
    HPBar->setOpacity(0.6);


    auto HPText = easy2d::gcnew easy2d::Text(std::to_string(hp) + '/' + std::to_string(hpMax));
    HPText->setAnchor(0.5, 0.5);

    easy2d::TextStyle HPTextStyle;
    HPTextStyle.alignment = easy2d::TextAlign::Center;
    HPTextStyle.wrappingWidth = 380;
    easy2d::Font HPTextFont;
    HPTextFont.size = 26;
    HPTextFont.weight = easy2d::Font::Weight::Bold;

    HPText->setTextStyle(HPTextStyle);
    HPText->setFont(HPTextFont);

    HPBar->setPos(x, y);
    HPText->setPos(HPBar->getPosX() + HPBar->getWidth() * 0.5, HPBar->getPosY() + HPBar->getHeight() * 0.5);

    this->battle_scene->addChild(HPBar); 
    this->battle_scene->addChild(HPText);
}

void GraphSlime::SetIcons(easy2d::Sprite* slimeImg, DefaultedMap<std::string, int, 0>& attr, float x, float y) {
    // TODO: 设置状态图标
    // 状态图片缩放 300%, 坐标 x-150(左)或 x-155(右), y-200,
    // // 上下两个状态的图标 y 轴相差 100
    auto iconSleep = easy2d::gcnew easy2d::Sprite("IconSleep.png");
    auto iconPaint = easy2d::gcnew easy2d::Sprite("IconPaint.png");
    iconSleep->setPos(x, y);
    iconPaint->setPos(x, y);
    iconSleep->setScale(1.0);
    iconPaint->setScale(1.0);

    if (attr["faint"] && attr["graffiti"]) {
        iconSleep->setPosY(iconSleep->getPosY() + 34);
        this->battle_scene->addChild(iconSleep);
        this->battle_scene->addChild(iconPaint);
    }
    else if (attr["faint"]) {
        this->battle_scene->addChild(iconSleep);
    }
    else if (attr["graffiti"]) {
        this->battle_scene->addChild(iconPaint);
    }
    else {
        return;
    }
}

std::string GraphSlime::GetSlimeImg(const Pet* slime, bool PorE) {
    // TODO: 获取史莱姆图片的文件名
    std::string typeTName = "Slime";
    switch (slime->type) {
    case Type_T::Normal:
        typeTName += "Normal";
        break;
    case Type_T::Fire:
        typeTName += "Fire";
        break;
    case Type_T::Grass:
        typeTName += "Grass";
        break;
    case Type_T::Water:
        typeTName += "Water";
        break;
    case Type_T::Thunder:
        typeTName += "Thunder";
        break;
    default:
        return "Null";
        break;
    }
    return (PorE == true ? typeTName + ".png" : typeTName + "_r.png");
}

GraphSlime::~GraphSlime() {
    delete battle_scene;
}
