// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "pch.h"
#include "utils.hpp"
#include "AdminCheck.hpp"
#include "LevelInfoLayer.hpp"
#include "CreatorLayer.hpp"

//bool (__thiscall* MenuLayer_init)(gd::MenuLayer* self);
//bool __fastcall MenuLayer_init_H(gd::MenuLayer* self, void* edx) {
//    if (!MenuLayer_init(self)) return false;

    //auto adminCheckSprite = CCSprite::create("GJ_button_03.png");
    //if (!(adminCheckSprite->initWithFile("accountBtn_pendingRequest_001.png")))
    //{
    //    adminCheckSprite->initWithFile("GJ_button_03.png");
    //    auto albl = CCLabelBMFont::create("A", "bigFont.fnt");
    //    adminCheckSprite->addChild(albl);
    //    albl->setPosition({ 20, 22 });
    //}
    //auto adminCheckButton = gd::CCMenuItemSpriteExtra::create(adminCheckSprite, nullptr, self, menu_selector(AdminCheck::adminCheckCallback));
    //auto menu = CCMenu::create();
    //adminCheckButton->setScale(1.f);
    //menu->addChild(adminCheckButton);
    //menu->setPosition((CCDirector::sharedDirector()->getScreenRight()) - 25, (CCDirector::sharedDirector()->getScreenTop()) - 25);
    //self->addChild(menu);
    
//    return true;
//}

bool(__thiscall* SupportLayer_customSetup)(gd::GJDropDownLayer*);
void __fastcall SupportLayer_customSetupH(gd::GJDropDownLayer* self) {
    SupportLayer_customSetup(self);

    auto winSize = CCDirector::sharedDirector()->getWinSize();

    auto layerMain = (CCLayer*)self->getChildren()->objectAtIndex(0);

    auto menu = CCMenu::create();
    menu->setPosition(winSize.width / 2, winSize.height / 2);

    auto reqSpr = gd::ButtonSprite::create("Req", 24, 0, 0.8f, true, "bigFont.fnt", "GJ_button_04.png", 25.f);
    auto reqBtn = gd::CCMenuItemSpriteExtra::create(reqSpr, reqSpr, self, menu_selector(AdminCheck::adminCheckCallback));
    reqBtn->setPosition(70, -90);

    layerMain->addChild(menu);
    menu->addChild(reqBtn);
}

DWORD WINAPI my_thread(void* hModule) {


    if (MH_Initialize() != MH_OK) {
        FreeLibraryAndExitThread(reinterpret_cast<HMODULE>(hModule), 0);
    }

    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x8ae6b), "\x83\xF8\x61\x0F\x84\xAB\x01\x00\x00\x90\x90\x90\x90\x90\x90\x90\x90\x90", 18, NULL); //HallOfShame onBack patching
    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x4BF000), "\x48\x61\x6c\x6c\x20\x6f\x66\x20\x46\x61\x6d\x65\x00", 13, NULL); //Hall of Shame string
    WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(gd::base + 0x8a7a2), "\x83\xF8\x63\x75\x0A\xBE\x48\x62\x52\x00\xE9\x81\x01\x00\x00\x83\xF8\x62\x75\x0A\xBE\x9C\x61\x52\x00\xE9\x72\x01\x00\x00\x83\xF8\x61\x75\x0A\xBE\x00\xF0\x8B\x00\xE9\x63\x01\x00\x00\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90", 58, NULL);
    
    //83 F8 63 75 0A BE 48 62 52 00 E9 81 01 00 00 83 F8 62 75 0A BE 9C 61 52 00 E9 72 01 00 00 83 F8 61 75 0A BE 00 40 72 00 E9 63 01 00 00 90 90 90 90 90 90 90 90 90 90 90 90 90
    
    //AllocConsole();
    //freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);

    auto cocos = GetModuleHandleA("libcocos2d.dll");
    auto cocos_ext = GetModuleHandleA("libExtensions.dll");

    //MH_CreateHook(
        //reinterpret_cast<void*>(gd::base + 0xaf210),
        //reinterpret_cast<void**>(&MenuLayer_init_H),
        //reinterpret_cast<void**>(&MenuLayer_init));

    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x9bc10),
        LevelInfoLayer::LevelInfoLayer_init_hook,
        reinterpret_cast<void**>(&LevelInfoLayer::LevelInfoLayer_init));

    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0xfd420), 
        SupportLayer_customSetupH, 
        reinterpret_cast<void**>(&SupportLayer_customSetup));

    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x2cea0), 
        CreatorLayer::CreatorLayer_init_H, 
        reinterpret_cast<void**>(&CreatorLayer::CreatorLayer_init));

    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0x2cfc3),
        CreatorLayer::CreatorLayer_menu_H,
        reinterpret_cast<void**>(&CreatorLayer::CreatorLayer_menu));
    /*
    //Hook example
    MH_CreateHook(
        reinterpret_cast<void*>(gd::base + 0xOFFSET),
        reinterpret_cast<void**>(&Class_Function_H),
        reinterpret_cast<void**>(&Class_Function));
    */

    MH_EnableHook(MH_ALL_HOOKS);

    return true;
}


BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(0, 0x1000, my_thread, hModule, 0, 0);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}