#include "CreatorLayer.hpp"

CCMenu* creatorLayerMenu = nullptr;

void CreatorLayer::Callback::onHallOfFame(CCObject*) {
    auto search = gd::GJSearchObject::create(97);
    auto lvlbrowserscene = cocos2d::CCScene::create();
    auto lvlbrowser = gd::LevelBrowserLayer::create(search);
    auto list = from<gd::GJListLayer*>(lvlbrowser, 0x124);
    std::cout << lvlbrowser << std::endl;
    lvlbrowserscene->addChild(lvlbrowser);

    auto finalScene = cocos2d::CCTransitionFade::create(0.5f, lvlbrowserscene);
    cocos2d::CCDirector::sharedDirector()->replaceScene(finalScene);
}

bool __fastcall CreatorLayer::CreatorLayer_init_H(gd::CreatorLayer* self) {
    creatorLayerMenu = nullptr;
    if (!CreatorLayer_init(self)) return false;
    auto btnArray = from<CCArray*>(creatorLayerMenu, 0xA8);
    auto createBtn = reinterpret_cast<gd::CCMenuItemSpriteExtra*>(btnArray->objectAtIndex(0));
    auto savedBtn = reinterpret_cast<gd::CCMenuItemSpriteExtra*>(btnArray->objectAtIndex(1));
    auto leaderBtn = reinterpret_cast<gd::CCMenuItemSpriteExtra*>(btnArray->objectAtIndex(2));
    auto featureBtn = reinterpret_cast<gd::CCMenuItemSpriteExtra*>(btnArray->objectAtIndex(3));
    auto mappackBtn = reinterpret_cast<gd::CCMenuItemSpriteExtra*>(btnArray->objectAtIndex(4));
    auto searchBtn = reinterpret_cast<gd::CCMenuItemSpriteExtra*>(btnArray->objectAtIndex(5));

    auto hofSprite = CCSprite::createWithSpriteFrameName("GJ_featuredBtn_001.png");
    if (!hofSprite->initWithFile("ghs-extension/GJ_fameBtn_001.png"))
    {
        hofSprite->initWithSpriteFrameName("GJ_featuredBtn_001.png");
        hofSprite->setColor({ 255, 255, 0 });
    }
    hofSprite->setScale(0.9f);
    auto hofBtn = gd::CCMenuItemSpriteExtra::create(hofSprite, nullptr, self, menu_selector(CreatorLayer::Callback::onHallOfFame));
    creatorLayerMenu->addChild(hofBtn);
    hofBtn->setPosition(0, 0);

    createBtn->setPosition(createBtn->getPositionX(), createBtn->getPositionY() - 5.f);
    from<CCSprite*>(createBtn, 0x108)->setScale(0.9f);

    savedBtn->setPosition(savedBtn->getPositionX(), savedBtn->getPositionY() + 35.f);
    from<CCSprite*>(savedBtn, 0x108)->setScale(0.9f);

    leaderBtn->setPosition(leaderBtn->getPositionX(), leaderBtn->getPositionY() - 5.f);
    from<CCSprite*>(leaderBtn, 0x108)->setScale(0.9f);

    featureBtn->setPosition(featureBtn->getPositionX(), featureBtn->getPositionY() + 5.f);
    from<CCSprite*>(featureBtn, 0x108)->setScale(0.9f);

    mappackBtn->setPosition(mappackBtn->getPositionX(), mappackBtn->getPositionY() - 35.f);
    from<CCSprite*>(mappackBtn, 0x108)->setScale(0.9f);

    searchBtn->setPosition(searchBtn->getPositionX(), searchBtn->getPositionY() + 5.f);
    from<CCSprite*>(searchBtn, 0x108)->setScale(0.9f);

    return true;
}

void __fastcall CreatorLayer::CreatorLayer_menu_H() {
    _asm {
        mov creatorLayerMenu, eax
    }
    CreatorLayer::CreatorLayer_menu();
}