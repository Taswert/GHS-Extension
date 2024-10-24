#include "pch.h"
#include "utils.hpp"
#include "AdminCheck.hpp"
#pragma warning(disable : 4996)

void AdminCheck::onAdminCallbackReq(CCHttpClient* client, CCHttpResponse* response) {
    if (!response) {
        gd::FLAlertLayer::create(nullptr, "No response", "Server don't responding.", "Ok", nullptr, 300.f, false, 0)->show();
        std::cerr << "No Response" << std::endl;
        return;
    }

    if (!response->isSucceed()) {
        gd::FLAlertLayer::create(nullptr, "Request Error", response->getErrorBuffer(), "Ok", nullptr, 300.f, false, 0)->show();
        //std::cerr << "Ошибка запроса: " << response->getErrorBuffer() << std::endl;
        return;
    }
    auto manager = gd::GameManager::sharedState();
    // Получаем данные ответа
    std::vector<char>* responseData = response->getResponseData();
    std::string readBuffer(responseData->begin(), responseData->end());

    if (readBuffer == "1")
    {
        gd::FLAlertLayer::create(nullptr, "Congratulations!", "You have <cb>moderator</c> privilegies!", "Ok", nullptr, 260.f, false, 0)->show();
        manager->setIntGameVariable("0457", 1);
    }
    else if (readBuffer == "2")
    {
        gd::FLAlertLayer::create(nullptr, "Congratulations!", "You have <cp>elder moderator</c> privilegies!", "Ok", nullptr, 280.f, false, 0)->show();
        manager->setIntGameVariable("0457", 2);
    }
    else if (readBuffer == "3")
    {
        gd::FLAlertLayer::create(nullptr, "Congratulations!", "You have <cr>administrator</c> privilegies!", "Ok", nullptr, 280.f, false, 0)->show();
        manager->setIntGameVariable("0457", 3);
    }
    else if (readBuffer == "4")
    {
        gd::FLAlertLayer::create(nullptr, "Congratulations!", "You have <cy>owner</c> privilegies!", "Ok", nullptr, 260.f, false, 0)->show();
        manager->setIntGameVariable("0457", 4);
    }
    else if (readBuffer == "-1")
    {
        gd::FLAlertLayer::create(nullptr, "Admin Check", "You have no moderator privilegies.", "Ok", nullptr, 260.f, false, 0)->show();
        manager->setIntGameVariable("0457", 0);
    }

    //std::cout << "readBuffer (server response): " << readBuffer << std::endl;
}

void AdminCheck::onAdminInitReq(CCHttpClient* client, CCHttpResponse* response) {
    if (!response) return;

    if (!response->isSucceed()) return;
    auto manager = gd::GameManager::sharedState();
    // Получаем данные ответа
    std::vector<char>* responseData = response->getResponseData();
    std::string readBuffer(responseData->begin(), responseData->end());

    if (readBuffer == "1")
        manager->setIntGameVariable("0457", 1);
    else if (readBuffer == "2")
        manager->setIntGameVariable("0457", 2);
    else if (readBuffer == "3")
        manager->setIntGameVariable("0457", 3);
    else if (readBuffer == "4")
        manager->setIntGameVariable("0457", 4);
    else if (readBuffer == "-1")
        manager->setIntGameVariable("0457", 0);
}

void AdminCheck::adminCheckCallback(CCObject* btn)
{
    std::string udid = (gd::GameManager::sharedState())->getPlayerUDID();
    //std::cout << btn << std::endl;
    std::string postfield = "deviceId=" + udid;
    auto checkReq = new CCHttpRequest();
    checkReq->setUrl("http://85.209.2.73:25568/AdminPanel/CheckAdmin");
    checkReq->setRequestType(CCHttpRequest::HttpRequestType::kHttpPost);
    checkReq->setRequestData(postfield.c_str(), postfield.size());

    checkReq->setResponseCallback(this, callfuncND_selector(AdminCheck::onAdminCallbackReq));

    //std::cout << "ADMIN CHECK PROTOCOL" << std::endl;
    //std::cout << udid << " - User device id" << std::endl << std::endl;

    CCHttpClient::getInstance()->send(checkReq);
    checkReq->release();
}

void AdminCheck::adminInitCheck(CCObject* self)
{
    std::string udid = (gd::GameManager::sharedState())->getPlayerUDID();

    std::string postfield = "deviceId=" + udid;

    auto checkReq = new CCHttpRequest();
    checkReq->setUrl("http://85.209.2.73:25568/AdminPanel/CheckAdmin");
    checkReq->setRequestType(CCHttpRequest::HttpRequestType::kHttpPost);
    checkReq->setRequestData(postfield.c_str(), postfield.size());

    checkReq->setResponseCallback(self, callfuncND_selector(AdminCheck::onAdminInitReq));

    CCHttpClient::getInstance()->send(checkReq);
    checkReq->release();
    
    //std::cout << "ADMIN INIT CHECK PROTOCOL" << std::endl;
    //std::cout << postfield << std::endl;
    //std::cout << readBuffer << std::endl;
    //std::cout << udid << " - User device id" << std::endl << std::endl;
}