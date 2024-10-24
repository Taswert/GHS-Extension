#include "pch.h"
#include "utils.hpp"
#include "LevelInfoLayer.hpp"
//#include "MyRateLayer.h"
#include <fstream>
//#include "HTTPRequest.hpp"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#pragma warning(disable : 4996)


int pass;
int globalSelectedStars;
int globalSelectedDiff;
bool globalIsFeatured;
gd::GJGameLevel* level;
gd::GameLevelManager* lvlmngr;
CCArray* savedlvls;
gd::LevelInfoLayer* gjlayer;
CCLayer* rateLayer;
CCLayer* infoFlalert;

CURL* curl;
CURLcode res;
std::string readBuffer;

//class LevelInfoLayer : CCLayer
//{
//public:
//	void onLevelUpdate(CCObject* btn) {
//		return reinterpret_cast<void(__fastcall*)(CCObject*)>(gd::base + 0x9e1e0)(btn);
//	}
//};

size_t curlWriteCallback(char* contents, size_t size, size_t nmemb, void* userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

class MyRequests {
public:
	void onUnrateRequest(CCHttpClient* client, CCHttpResponse* response) {
		if (!response) {
			gd::FLAlertLayer::create(nullptr, "No response", "Server don't responding.", "Ok", nullptr, 300.f, false, 0)->show();
			return;
		}
		if (!response->isSucceed()) {
			gd::FLAlertLayer::create(nullptr, "Request Error", response->getErrorBuffer(), "Ok", nullptr, 300.f, false, 0)->show();
			return;
		}
		std::vector<char>* responseData = response->getResponseData();
		std::string readBuffer(responseData->begin(), responseData->end());

		if (readBuffer == "1") gd::FLAlertLayer::create(nullptr, "Unrated!", "Level is succesfully <cr>unrated</c>!", "Ok", nullptr, 240.f, false, 0)->show();
		else if (readBuffer == "-1")  gd::FLAlertLayer::create(nullptr, "Unrate failure.", "You have no permissions to unrate levels.", "Ok", nullptr, 260.f, false, 0)->show();
		else if (readBuffer == "-2") gd::FLAlertLayer::create(nullptr, "Unrate failure.", "Level does not exist on servers.", "Ok", nullptr, 260.f, false, 0)->show();
		else gd::FLAlertLayer::create(nullptr, "Something went wrong...", "You have no internet connection or servers are down.", "Ok", nullptr, 300.f, false, 0)->show();
		if (gjlayer) gjlayer->downloadLevel();
	}

	void onDiffrateRequest(CCHttpClient* client, CCHttpResponse* response) {
		if (!response) {
			gd::FLAlertLayer::create(nullptr, "No response", "Server don't responding.", "Ok", nullptr, 300.f, false, 0)->show();
			return;
		}
		if (!response->isSucceed()) {
			gd::FLAlertLayer::create(nullptr, "Request Error", response->getErrorBuffer(), "Ok", nullptr, 300.f, false, 0)->show();
			return;
		}
		std::vector<char>* responseData = response->getResponseData();
		std::string readBuffer(responseData->begin(), responseData->end());

		if (readBuffer == "1") gd::FLAlertLayer::create(nullptr, "Success!", "Level now has <cy>difficulty</c>!", "Ok", nullptr, 240.f, false, 0)->show();
		else if (readBuffer == "-1")  gd::FLAlertLayer::create(nullptr, "Failure.", "You have no permissions to give difficulties.", "Ok", nullptr, 260.f, false, 0)->show();
		else if (readBuffer == "-2") gd::FLAlertLayer::create(nullptr, "Failure.", "Level does not exist on servers.", "Ok", nullptr, 260.f, false, 0)->show();
		else gd::FLAlertLayer::create(nullptr, "Something went wrong...", "You have no internet connection or servers are down.", "Ok", nullptr, 300.f, false, 0)->show();
		if (gjlayer) gjlayer->downloadLevel();
	}

	void onStarRequest(CCHttpClient* client, CCHttpResponse* response) {
		if (!response) {
			gd::FLAlertLayer::create(nullptr, "No response", "Server don't responding.", "Ok", nullptr, 300.f, false, 0)->show();
			return;
		}
		if (!response->isSucceed()) {
			gd::FLAlertLayer::create(nullptr, "Request Error", response->getErrorBuffer(), "Ok", nullptr, 300.f, false, 0)->show();
			return;
		}
		std::vector<char>* responseData = response->getResponseData();
		std::string readBuffer(responseData->begin(), responseData->end());

		if (readBuffer == "1") gd::FLAlertLayer::create(nullptr, "Rated!", "Level is <cy>star rated</c>!", "Ok", nullptr, 240.f, false, 0)->show();
		else if (readBuffer == "-1")  gd::FLAlertLayer::create(nullptr, "Rate failure.", "You have no permissions to rate levels.", "Ok", nullptr, 260.f, false, 0)->show();
		else if (readBuffer == "-2") gd::FLAlertLayer::create(nullptr, "Rate failure.", "Level does not exist on servers.", "Ok", nullptr, 260.f, false, 0)->show();
		else gd::FLAlertLayer::create(nullptr, "Something went wrong...", "You have no internet connection or servers are down.", "Ok", nullptr, 300.f, false, 0)->show();
		if (gjlayer) gjlayer->downloadLevel();
	}

	void onFeatureRequest(CCHttpClient* client, CCHttpResponse* response) {
		if (!response) {
			gd::FLAlertLayer::create(nullptr, "No response", "Server don't responding.", "Ok", nullptr, 300.f, false, 0)->show();
			return;
		}
		if (!response->isSucceed()) {
			gd::FLAlertLayer::create(nullptr, "Request Error", response->getErrorBuffer(), "Ok", nullptr, 300.f, false, 0)->show();
			return;
		}
		std::vector<char>* responseData = response->getResponseData();
		std::string readBuffer(responseData->begin(), responseData->end());

		if (readBuffer == "1") gd::FLAlertLayer::create(nullptr, "Featured!", "Level is <cp>featured</c>!", "Ok", nullptr, 240.f, false, 0)->show();
		else if (readBuffer == "-1")  gd::FLAlertLayer::create(nullptr, "Rate failure.", "You have no permissions to rate levels.", "Ok", nullptr, 260.f, false, 0)->show();
		else if (readBuffer == "-2") gd::FLAlertLayer::create(nullptr, "Rate failure.", "Level does not exist on servers.", "Ok", nullptr, 260.f, false, 0)->show();
		else gd::FLAlertLayer::create(nullptr, "Something went wrong...", "You have no internet connection or servers are down.", "Ok", nullptr, 300.f, false, 0)->show();
		if (gjlayer) gjlayer->downloadLevel();
	}

	void onBlockRequest(CCHttpClient* client, CCHttpResponse* response) {
		if (!response) {
			gd::FLAlertLayer::create(nullptr, "No response", "Server don't responding.", "Ok", nullptr, 300.f, false, 0)->show();
			return;
		}
		if (!response->isSucceed()) {
			gd::FLAlertLayer::create(nullptr, "Request Error", response->getErrorBuffer(), "Ok", nullptr, 300.f, false, 0)->show();
			return;
		}
		std::vector<char>* responseData = response->getResponseData();
		std::string readBuffer(responseData->begin(), responseData->end());

		if (readBuffer == "1") gd::FLAlertLayer::create(nullptr, "Blocked!", "Level is now <cr>blocked</c>!", "Ok", nullptr, 240.f, false, 0)->show();
		else if (readBuffer == "2") gd::FLAlertLayer::create(nullptr, "Unblocked!", "Level is now <cg>unblocked</c>!", "Ok", nullptr, 240.f, false, 0)->show();
		else if (readBuffer == "-1")  gd::FLAlertLayer::create(nullptr, "Block failure.", "You have no permissions to block levels.", "Ok", nullptr, 260.f, false, 0)->show();
		else if (readBuffer == "-2") gd::FLAlertLayer::create(nullptr, "Rate failure.", "Level does not exist on servers.", "Ok", nullptr, 260.f, false, 0)->show();
		else gd::FLAlertLayer::create(nullptr, "Something went wrong...", "You have no internet connection or servers are down.", "Ok", nullptr, 300.f, false, 0)->show();
		if (gjlayer) gjlayer->downloadLevel();
	}

	void onDeleteRequest(CCHttpClient* client, CCHttpResponse* response) {
		if (!response) {
			gd::FLAlertLayer::create(nullptr, "No response", "Server don't responding.", "Ok", nullptr, 300.f, false, 0)->show();
			return;
		}
		if (!response->isSucceed()) {
			gd::FLAlertLayer::create(nullptr, "Request Error", response->getErrorBuffer(), "Ok", nullptr, 300.f, false, 0)->show();
			return;
		}
		std::vector<char>* responseData = response->getResponseData();
		std::string readBuffer(responseData->begin(), responseData->end());

		if (readBuffer == "1") gd::FLAlertLayer::create(nullptr, "Deleted.", "<cc>Level is permanently deleted.</c>", "Ok", nullptr, 240.f, false, 0)->show();
		else if (readBuffer == "-1")  gd::FLAlertLayer::create(nullptr, "Deletion failure.", "You have no permissions to delete levels.", "Ok", nullptr, 260.f, false, 0)->show();
		else if (readBuffer == "-2") gd::FLAlertLayer::create(nullptr, "Deletion failure.", "Level does not exist on servers.", "Ok", nullptr, 260.f, false, 0)->show();
		else gd::FLAlertLayer::create(nullptr, "Something went wrong...", "You have no internet connection or servers are down.", "Ok", nullptr, 300.f, false, 0)->show();
		if (gjlayer) gjlayer->downloadLevel();
	}

	void onAddToHoFRequest(CCHttpClient* client, CCHttpResponse* response) {
		if (!response) {
			gd::FLAlertLayer::create(nullptr, "No response", "Server don't responding.", "Ok", nullptr, 300.f, false, 0)->show();
			return;
		}
		if (!response->isSucceed()) {
			gd::FLAlertLayer::create(nullptr, "Request Error", response->getErrorBuffer(), "Ok", nullptr, 300.f, false, 0)->show();
			return;
		}
		std::vector<char>* responseData = response->getResponseData();
		std::string readBuffer(responseData->begin(), responseData->end());

		if (readBuffer == "1") gd::FLAlertLayer::create(nullptr, "Done!", "Level is succesfully <cg>added</c> \nto <cy>Hall of Fame</c>!", "Ok", nullptr, 270.f, false, 0)->show();
		else if (readBuffer == "-1")  gd::FLAlertLayer::create(nullptr, "Adding failure.", "You have no permissions to add levels \nto <cy>Hall of Fame</c>.", "Ok", nullptr, 290.f, false, 0)->show();
		else if (readBuffer == "-2") gd::FLAlertLayer::create(nullptr, "Adding failure.", "Level does not exist on servers.", "Ok", nullptr, 260.f, false, 0)->show();
		else gd::FLAlertLayer::create(nullptr, "Something went wrong...", "You have no internet connection or servers are down.", "Ok", nullptr, 300.f, false, 0)->show();
		if (gjlayer) gjlayer->downloadLevel();
	}

	void onRemoveFromHoFRequest(CCHttpClient* client, CCHttpResponse* response) {
		if (!response) {
			gd::FLAlertLayer::create(nullptr, "No response", "Server don't responding.", "Ok", nullptr, 300.f, false, 0)->show();
			return;
		}
		if (!response->isSucceed()) {
			gd::FLAlertLayer::create(nullptr, "Request Error", response->getErrorBuffer(), "Ok", nullptr, 300.f, false, 0)->show();
			return;
		}
		std::vector<char>* responseData = response->getResponseData();
		std::string readBuffer(responseData->begin(), responseData->end());

		if (readBuffer == "1") gd::FLAlertLayer::create(nullptr, "Done!", "Level is succesfully <cr>removed</c> \nfrom <cy>Hall of Fame</c>!", "Ok", nullptr, 270.f, false, 0)->show();
		else if (readBuffer == "-1")  gd::FLAlertLayer::create(nullptr, "Removing failure.", "You have no permissions to remove levels \nfrom <cy>Hall of Fame</c>.", "Ok", nullptr, 300.f, false, 0)->show();
		else if (readBuffer == "-2") gd::FLAlertLayer::create(nullptr, "Removing failure.", "Level does not exist on servers.", "Ok", nullptr, 260.f, false, 0)->show();
		else gd::FLAlertLayer::create(nullptr, "Something went wrong...", "You have no internet connection or servers are down.", "Ok", nullptr, 300.f, false, 0)->show();
		if (gjlayer) gjlayer->downloadLevel();
	}
};

class UnrateAlertProtocol : public gd::FLAlertLayerProtocol, CCObject {
protected:
	void FLAlert_Clicked(gd::FLAlertLayer* layer, bool btn2) override
	{
		if (btn2)
		{
			auto gameManager = gd::GameManager::sharedState();
			std::string udid = gameManager->getPlayerUDID();
			int levelid = level->m_levelID;

			std::string postfield = "deviceId=" + udid + "&levelId=" + std::to_string(levelid) + "&stars=0&diff=0&featured=0";

			auto checkReq = new CCHttpRequest();
			checkReq->setUrl("http://85.209.2.73:25568/AdminPanel/RateLevel");
			checkReq->setRequestType(CCHttpRequest::HttpRequestType::kHttpPost);
			checkReq->setRequestData(postfield.c_str(), postfield.size());
			checkReq->setResponseCallback(this, callfuncND_selector(MyRequests::onUnrateRequest));
			CCHttpClient::getInstance()->send(checkReq);
			checkReq->release();

			if (rateLayer) {
				rateLayer->removeMeAndCleanup();
				rateLayer = nullptr;
			}

			//std::cout << "UNRATE PROTOCOL" << std::endl;
			//std::cout << readBuffer << std::endl;
			//std::cout << "STRING - " << postfield << std::endl;
			//std::cout << levelid << std::endl << std::endl;
		}
	}
};

class DeleteAlertProtocol : public gd::FLAlertLayerProtocol, CCObject {
protected:

	void FLAlert_Clicked(gd::FLAlertLayer* layer, bool btn2) override
	{
		if (btn2)
		{
			auto gameManager = gd::GameManager::sharedState();
			std::string udid = gameManager->getPlayerUDID();
			int levelid = level->m_levelID;

			std::string postfield = "deviceId=" + udid + "&levelId=" + std::to_string(levelid);

			auto checkReq = new CCHttpRequest();
			checkReq->setUrl("http://85.209.2.73:25568/AdminPanel/DeleteLevel");
			checkReq->setRequestType(CCHttpRequest::HttpRequestType::kHttpPost);
			checkReq->setRequestData(postfield.c_str(), postfield.size());
			checkReq->setResponseCallback(this, callfuncND_selector(MyRequests::onDeleteRequest));
			CCHttpClient::getInstance()->send(checkReq);
			checkReq->release();

			if (rateLayer) {
				rateLayer->removeMeAndCleanup();
				rateLayer = nullptr;
			}

			//std::cout << "DELETE PROTOCOL" << std::endl;
			//std::cout << readBuffer << std::endl;
			//std::cout << levelid << std::endl << std::endl;
		}
	}
};

class BlockAlertProtocol : public gd::FLAlertLayerProtocol, CCObject {
protected:

	void FLAlert_Clicked(gd::FLAlertLayer* layer, bool btn2) override
	{
		if (btn2)
		{
			auto gameManager = gd::GameManager::sharedState();
			std::string udid = gameManager->getPlayerUDID();
			int levelid = level->m_levelID;

			std::string postfield = "deviceId=" + udid + "&levelId=" + std::to_string(levelid);

			auto checkReq = new CCHttpRequest();
			checkReq->setUrl("http://85.209.2.73:25568/AdminPanel/SetBanLevel");
			checkReq->setRequestType(CCHttpRequest::HttpRequestType::kHttpPost);
			checkReq->setRequestData(postfield.c_str(), postfield.size());
			checkReq->setResponseCallback(this, callfuncND_selector(MyRequests::onBlockRequest));
			CCHttpClient::getInstance()->send(checkReq);
			checkReq->release();

			if (rateLayer) {
				rateLayer->removeMeAndCleanup();
				rateLayer = nullptr;
			}

			//std::cout << "BLOCK PROTOCOL" << std::endl;
			//std::cout << readBuffer << std::endl;
			//std::cout << levelid << std::endl << std::endl;
		}
	}
};

class DiffRateAlertProtocol : public gd::FLAlertLayerProtocol, CCObject {
protected:

	void FLAlert_Clicked(gd::FLAlertLayer* layer, bool btn2) override
	{
		if (btn2)
		{
			std::string readBufferStore;
			auto gameManager = gd::GameManager::sharedState();
			std::string udid = gameManager->getPlayerUDID();
			int levelid = level->m_levelID;

			std::string postfield = "deviceId=" + udid + "&levelId=" + std::to_string(levelid) + "&stars=0" + "&diff=" + std::to_string(globalSelectedDiff) + "&featured=0";
			
			auto checkReq = new CCHttpRequest();
			checkReq->setUrl("http://85.209.2.73:25568/AdminPanel/RateLevel");
			checkReq->setRequestType(CCHttpRequest::HttpRequestType::kHttpPost);
			checkReq->setRequestData(postfield.c_str(), postfield.size());
			checkReq->setResponseCallback(this, callfuncND_selector(MyRequests::onDiffrateRequest));
			CCHttpClient::getInstance()->send(checkReq);
			checkReq->release();


			//std::cout << "DIFF RATE PROTOCOL" << std::endl;
			//std::cout << levelid << std::endl;
			//std::cout << globalSelectedDiff << " - diff" << std::endl;
			//std::cout << "STRING - " << postfield << std::endl << std::endl;

			if (rateLayer) {
				rateLayer->removeMeAndCleanup();
				rateLayer = nullptr;
			}

			//gd::GameLevelManager::sharedState()->levelUpdate(level);
		}
	}
};

class StarRateAlertProtocol : public gd::FLAlertLayerProtocol, CCObject {
protected:

	void FLAlert_Clicked(gd::FLAlertLayer* layer, bool btn2) override
	{
		if (btn2)
		{
			std::string readBufferStore;
			auto gameManager = gd::GameManager::sharedState();
			std::string udid = gameManager->getPlayerUDID();
			int levelid = level->m_levelID;

			std::string postfield = "deviceId=" + udid + "&levelId=" + std::to_string(levelid) + "&stars=" + std::to_string(globalSelectedStars) + "&diff=" + std::to_string(globalSelectedDiff) + "&featured=0";

			auto checkReq = new CCHttpRequest();
			checkReq->setUrl("http://85.209.2.73:25568/AdminPanel/RateLevel");
			checkReq->setRequestType(CCHttpRequest::HttpRequestType::kHttpPost);
			checkReq->setRequestData(postfield.c_str(), postfield.size());
			checkReq->setResponseCallback(this, callfuncND_selector(MyRequests::onStarRequest));
			CCHttpClient::getInstance()->send(checkReq);
			checkReq->release();

			//std::cout << "STAR RATE PROTOCOL" << std::endl;
			//std::cout << readBuffer << std::endl;
			//std::cout << levelid << std::endl;
			//std::cout << globalSelectedStars << " - stars" << std::endl;
			//std::cout << globalSelectedDiff << " - diff" << std::endl;

			//std::cout << "STRING - " << postfield << std::endl << std::endl;

			if (rateLayer) {
				rateLayer->removeMeAndCleanup();
				rateLayer = nullptr;
			}

			//gd::GameLevelManager::sharedState()->levelUpdate(level);
		}
	}
};

class FeaturedAlertProtocol : public gd::FLAlertLayerProtocol, CCObject {
protected:

	void FLAlert_Clicked(gd::FLAlertLayer* layer, bool btn2) override
	{
		if (btn2)
		{
			std::string readBufferStore;

			auto gameManager = gd::GameManager::sharedState();
			std::string udid = gameManager->getPlayerUDID();
			int levelid = level->m_levelID;

			std::string postfield = "deviceId=" + udid + "&levelId=" + std::to_string(levelid) + "&stars=" + std::to_string(globalSelectedStars) + "&diff=" + std::to_string(globalSelectedDiff) + "&featured=1";

			auto checkReq = new CCHttpRequest();
			checkReq->setUrl("http://85.209.2.73:25568/AdminPanel/RateLevel");
			checkReq->setRequestType(CCHttpRequest::HttpRequestType::kHttpPost);
			checkReq->setRequestData(postfield.c_str(), postfield.size());
			checkReq->setResponseCallback(this, callfuncND_selector(MyRequests::onFeatureRequest));
			CCHttpClient::getInstance()->send(checkReq);
			checkReq->release();

			//std::cout << "FEATURE RATE PROTOCOL" << std::endl;
			//std::cout << readBuffer << std::endl;
			//std::cout << levelid << std::endl;
			//std::cout << globalSelectedStars << " - stars" << std::endl;
			//std::cout << globalSelectedDiff << " - diff" << std::endl;

			//std::cout << "STRING - " << postfield << std::endl << std::endl;

			if (rateLayer) {
				rateLayer->removeMeAndCleanup();
				rateLayer = nullptr;
			}

			//gd::GameLevelManager::sharedState()->levelUpdate(level);
		}
	}
};

class AddToHofAlertProtocol : public gd::FLAlertLayerProtocol, CCObject {
protected:

	void FLAlert_Clicked(gd::FLAlertLayer* layer, bool btn2) override
	{
		if (btn2)
		{
			auto gameManager = gd::GameManager::sharedState();
			std::string udid = gameManager->getPlayerUDID();
			int levelid = level->m_levelID;

			std::string postfield = "deviceId=" + udid + "&levelID=" + std::to_string(levelid) + "&value=1";

			auto checkReq = new CCHttpRequest();
			checkReq->setUrl("http://85.209.2.73:25568/AdminPanel/SetHallOfFame");
			checkReq->setRequestType(CCHttpRequest::HttpRequestType::kHttpPost);
			checkReq->setRequestData(postfield.c_str(), postfield.size());
			checkReq->setResponseCallback(this, callfuncND_selector(MyRequests::onAddToHoFRequest));
			CCHttpClient::getInstance()->send(checkReq);
			checkReq->release();

			//std::cout << "ADD TO HOF PROTOCOL" << std::endl;
			//std::cout << "STRING - " << postfield << std::endl;
			//std::cout << levelid << std::endl << std::endl;

			if (rateLayer) {
				rateLayer->removeMeAndCleanup();
				rateLayer = nullptr;
			}

			//lvlmngr->levelUpdate(level);
			//gd::GameLevelManager::sharedState()->levelUpdate(level);
		}
	}
};

class RemoveFromHofAlertProtocol : public gd::FLAlertLayerProtocol, CCObject{
protected:

	void FLAlert_Clicked(gd::FLAlertLayer* layer, bool btn2) override
	{
		if (btn2)
		{
			auto gameManager = gd::GameManager::sharedState();
			std::string udid = gameManager->getPlayerUDID();
			int levelid = level->m_levelID;

			std::string postfield = "deviceId=" + udid + "&levelID=" + std::to_string(levelid) + "&value=0";

			auto checkReq = new CCHttpRequest();
			checkReq->setUrl("http://85.209.2.73:25568/AdminPanel/SetHallOfFame");
			checkReq->setRequestType(CCHttpRequest::HttpRequestType::kHttpPost);
			checkReq->setRequestData(postfield.c_str(), postfield.size());
			checkReq->setResponseCallback(this, callfuncND_selector(MyRequests::onRemoveFromHoFRequest));
			CCHttpClient::getInstance()->send(checkReq);
			checkReq->release();

			//std::cout << "REMOVE FROM HOF PROTOCOL" << std::endl;
			//std::cout << "STRING - " << postfield << std::endl;
			//std::cout << levelid << std::endl << std::endl;

			if (rateLayer) {
				rateLayer->removeMeAndCleanup();
				rateLayer = nullptr;
			}

			//lvlmngr->levelUpdate(level);
			//gd::GameLevelManager::sharedState()->levelUpdate(level);
		}
	}
};

UnrateAlertProtocol unrateProtocol;
DeleteAlertProtocol deleteProtocol;
BlockAlertProtocol blockProtocol;
DiffRateAlertProtocol diffrateProtocol;
StarRateAlertProtocol starProtocol;
FeaturedAlertProtocol featureProtocol;
AddToHofAlertProtocol addToHofProtocol;
RemoveFromHofAlertProtocol removeFromHofProtocol;

class MyAwesomeRateLayer : public CCLayer 
{
private:
	bool isFeatured = false;
	int selectedStar = 0;
	int selectedDiff = 0;

	void featuredButtonCallback(CCObject*)
	{
		isFeatured = !isFeatured;
	}

	void deleteButtonCallback(CCObject*)
	{
		const auto flalert = gd::FLAlertLayer::create(&deleteProtocol, "Permanent Deletion", "Are you sure you want to <cc>delete</c> this level from <cy>server</c>?", "No", "Yes", 320.f, false, 0);
		flalert->show();
	}

	void blockButtonCallback(CCObject*)
	{
		const auto flalert = gd::FLAlertLayer::create(&blockProtocol, "Level Block", "Do you want to <cr>block</c>/<cg>unblock</c> this level on <cy>server</c>?\nYou can unblock this level later.", "No", "Yes", 300.f, false, 0);
		flalert->show();
	}

	void cancelButtonCallback(CCObject*)
	{
		this->removeMeAndCleanup();
		rateLayer = nullptr;
	}

	void submitButtonCallback(CCObject* btn)
	{
		globalIsFeatured = isFeatured;
		globalSelectedStars = selectedStar;
		globalSelectedDiff = selectedDiff;
		if (selectedStar != 0)
		{
			if (!isFeatured) {
				const auto flalert = gd::FLAlertLayer::create(&starProtocol, "Star Rate", "Are you sure you want to <cy>star rate</c> this level?", "No", "Yes", 340.f, false, 0);
				flalert->show();
			}
			else {
				const auto flalert = gd::FLAlertLayer::create(&featureProtocol, "Feature", "Are you sure you want to <cp>feature</c> this level?", "No", "Yes", 340.f, false, 0);
				flalert->show();
			}

		}
	}
	void difrateButtonCallback(CCObject*)
	{
		globalSelectedDiff = selectedDiff;
		if (selectedDiff != 0) {
			const auto flalert = gd::FLAlertLayer::create(&diffrateProtocol, "Difficulty Rate", "Do you want to give chosen <cy>difficulty</c> to this level?\nThis action will not give any creator points, nor stars.", "No", "Yes", 360.f, false, 0);
			flalert->show();
		}
	}

public:

	virtual void keyBackClicked()
	{
		this->removeMeAndCleanup();
		rateLayer = nullptr;
	}
	//void backButton();
	//void keyBackClicked(void) override {
	//	rateLayer->removeMeAndCleanup();
	//	rateLayer = nullptr;
	//}

	bool rateButtonToggled = false;
	auto rateButtonToggler(CCSprite* toggleOn, CCSprite* toggleOff) {
		return (rateButtonToggled) ? toggleOn : toggleOff;
	}

	bool featuredButtonToggled = false;
	auto featureButtonToggler(CCSprite* toggleOn, CCSprite* toggleOff) {
		return (featuredButtonToggled) ? toggleOn : toggleOff;
	}

	void selectStarRate(CCObject* btn)
	{
		int tag;
		tag = static_cast<gd::CCMenuItemToggler*>(btn)->getTag();
		if (selectedStar != 0)
		{
			auto lastBtn = this->getChildByTag(457)->getChildByTag(100 + selectedStar);
			reinterpret_cast<gd::CCMenuItemToggler*>(lastBtn)->toggle(false);
		}
		selectedStar = tag - 100;

		auto submitSprite = this->getChildByTag(458)->getChildByTag(1)->getChildByTag(1);
		reinterpret_cast<gd::ButtonSprite*>(submitSprite)->setColor({ 255, 255, 255 });
		submitSprite = this->getChildByTag(458)->getChildByTag(2)->getChildByTag(1);
		reinterpret_cast<gd::ButtonSprite*>(submitSprite)->setColor({ 255, 255, 255 });

		auto starSprite = this->getChildByTag(460);
		starSprite->setVisible(true);

		auto starlbl = this->getChildByTag(461);
		starlbl->setVisible(true);
		static_cast<CCLabelBMFont*>(starlbl)->setString(CCString::createWithFormat("%d", selectedStar)->getCString());

		auto faceSprite = reinterpret_cast<CCSprite*>(this->getChildByTag(459));
		switch (selectedStar)
		{
		case 1:
			faceSprite->initWithSpriteFrameName("difficulty_auto_btn_001.png");
			selectedDiff = 7;
			break;
		case 2:
			faceSprite->initWithSpriteFrameName("difficulty_01_btn_001.png");
			selectedDiff = 1;
			break;
		case 3:
			faceSprite->initWithSpriteFrameName("difficulty_02_btn_001.png");
			selectedDiff = 2;
			break;
		case 4:
		case 5:
			faceSprite->initWithSpriteFrameName("difficulty_03_btn_001.png");
			selectedDiff = 3;
			break;
		case 6:
		case 7:
			faceSprite->initWithSpriteFrameName("difficulty_04_btn_001.png");
			selectedDiff = 4;
			break;
		case 8:
		case 9:
			faceSprite->initWithSpriteFrameName("difficulty_05_btn_001.png");
			selectedDiff = 5;
			break;
		case 10:
			faceSprite->initWithSpriteFrameName("difficulty_06_btn_001.png");
			selectedDiff = 6;
			break;
		default:
			faceSprite->createWithSpriteFrameName("difficulty_00_btn_001.png");
			selectedDiff = 0;
			break;
		}
	}

	bool init()
	{
		//static CCLayerColor::initWithColor(ccc4(0, 0, 0, 105));
		/*if (!(cclcol->initWithColor(ccc4(0, 0, 0, 105)))) return false;*/

		//gd::FLAlertLayer::init(nullptr, nullptr, nullptr, nullptr, nullptr, 10, false, 10);
		if (!CCLayer::init())
			return false;
		CCLayerColor* cclcol = CCLayerColor::create(ccc4(0, 0, 0, 0));
		cclcol->setZOrder(1);
		cclcol->setScale(10.f);
		this->addChild(cclcol);
		auto actionCol = CCFadeTo::create(0.1f, 75);
		cclcol->runAction(actionCol);

		auto touchDispatcher = CCDirector::sharedDirector()->m_pTouchDispatcher;
		touchDispatcher->incrementForcePrio();
		touchDispatcher->incrementForcePrio();
		registerWithTouchDispatcher();
		setTouchEnabled(true);
		setKeypadEnabled(true);
		setMouseEnabled(true);

		auto bgSprite = CCSprite::create("GJ_button_03.png"); //Background button. Ik this is so bad, but i can't do anything better than that
		bgSprite->setScale(100.f);
		bgSprite->setOpacity(0);
		auto bgButton = gd::CCMenuItemSpriteExtra::create(bgSprite, nullptr, this, nullptr);
		auto bgMenu = CCMenu::create();
		bgMenu->addChild(bgButton);
		bgMenu->setZOrder(0);
		bgMenu->setPosition((CCDirector::sharedDirector()->getScreenRight()) - 25, (CCDirector::sharedDirector()->getScreenTop()) - 25);
		this->addChild(bgMenu);

		cocos2d::extension::CCScale9Sprite* bg = cocos2d::extension::CCScale9Sprite::create("GJ_square01.png");
		auto director = CCDirector::sharedDirector();
		bg->setContentSize({ 380, 180 });
		bg->setPosition((director->getScreenRight()) / 2, (director->getScreenTop()) / 2);
		bg->setZOrder(2);
		this->addChild(bg);
		auto appearAction = CCEaseElasticOut::create(CCScaleTo::create(.5f, 1.f), 0.6f);

		auto mainLabel = CCLabelBMFont::create("Mod: Rate level", "bigFont.fnt");
		mainLabel->setPosition((director->getScreenRight()) / 2, (director->getScreenTop()) / 2 + 70);
		mainLabel->setZOrder(3);
		this->addChild(mainLabel);

		//rate stars buttons
		auto rateMenu = CCMenu::create();
		rateMenu->setPosition((director->getScreenRight()) / 2 - 135, (director->getScreenTop()) / 2 + 26);
		rateMenu->setZOrder(3);

		for (int i = 1; i <= 10; i++)
		{
			std::string str = std::to_string(i);
			const char* ch = str.c_str();
			auto spriteStarOff = gd::ButtonSprite::create(ch, 21, 0, 0.5f, true, "bigFont.fnt", "GJ_button_04.png", 30.0);
			auto spriteStarOn = gd::ButtonSprite::create(ch, 21, 0, 0.5f, true, "bigFont.fnt", "GJ_button_02.png", 30.0);
			auto rateButton = gd::CCMenuItemToggler::create(rateButtonToggler(spriteStarOn, spriteStarOff), rateButtonToggler(spriteStarOff, spriteStarOn), this, menu_selector(MyAwesomeRateLayer::selectStarRate));
			rateButton->setTag(i + 100);
			rateButton->setZOrder(5);
			if (i < 6) rateButton->setPosition(55 * (i - 1), 0);
			else rateButton->setPosition(55 * (i - 6), -46);
			rateMenu->addChild(rateButton);
		}
		rateMenu->setTag(457);

		//face sprite and feature button
		auto faceSprite = CCSprite::createWithSpriteFrameName("difficulty_00_btn_001.png");
		faceSprite->setPosition({ (director->getScreenRight()) / 2 + 140, (director->getScreenTop()) / 2 });
		faceSprite->setScale(1.2f);
		faceSprite->setZOrder(4);
		faceSprite->setTag(459);
		this->addChild(faceSprite);

		auto starSprite = CCSprite::createWithSpriteFrameName("star_small01_001.png");
		starSprite->setPosition({ (director->getScreenRight()) / 2 + 150, (director->getScreenTop()) / 2 - 35 });
		starSprite->setVisible(false);
		starSprite->setZOrder(4);
		starSprite->setTag(460);
		this->addChild(starSprite);

		auto starlbl = CCLabelBMFont::create("0", "bigFont.fnt");
		starlbl->setAnchorPoint({ 1.f, 0.5f });
		starlbl->setPosition({ (director->getScreenRight()) / 2 + 142, (director->getScreenTop()) / 2 - 35 });
		starlbl->setZOrder(4);
		starlbl->setScale(0.45f);
		starlbl->setVisible(false);
		starlbl->setTag(461);
		this->addChild(starlbl);

		auto spriteFeatureOff = CCSprite::createWithSpriteFrameName("GJ_featuredCoin_001.png");
		spriteFeatureOff->setScale(1.f);
		auto spriteFeatureOn = CCSprite::createWithSpriteFrameName("GJ_featuredCoin_001.png");
		spriteFeatureOn->setScale(1.2f);
		auto featureButton = gd::CCMenuItemToggler::create(featureButtonToggler(spriteFeatureOn, spriteFeatureOff), featureButtonToggler(spriteFeatureOff, spriteFeatureOn), this, menu_selector(MyAwesomeRateLayer::featuredButtonCallback));
		auto featureMenu = CCMenu::create();
		featureMenu->setZOrder(3);
		featureMenu->setPosition({ (director->getScreenRight()) / 2 + 140, (director->getScreenTop()) / 2 });
		featureMenu->addChild(featureButton);

		//block and delete buttons
		auto additionalMenu = CCMenu::create();
		additionalMenu->setZOrder(3);
		if (gd::GameManager::sharedState()->getIntGameVariable("0457") > 1)
		{
			auto deleteLevelSprite = CCSprite::createWithSpriteFrameName("edit_delBtn_001.png");
			deleteLevelSprite->setScale(1.5f);
			auto deleteLevelButton = gd::CCMenuItemSpriteExtra::create(deleteLevelSprite, nullptr, this, menu_selector(MyAwesomeRateLayer::deleteButtonCallback));

			auto blockLevelSprite = CCSprite::createWithSpriteFrameName("GJ_lock_001.png");
			blockLevelSprite->setScale(1.5f);
			auto blockLevelButton = gd::CCMenuItemSpriteExtra::create(blockLevelSprite, nullptr, this, menu_selector(MyAwesomeRateLayer::blockButtonCallback));
			blockLevelButton->setPosition(0, -50);

			additionalMenu->setPosition((director->getScreenRight() / 2 + bg->getContentSize().width / 2 + blockLevelButton->getContentSize().width/2+5), (director->getScreenTop()) / 2 + 60);

			additionalMenu->addChild(deleteLevelButton);
			additionalMenu->addChild(blockLevelButton);
		}

		auto additionalMenu2 = CCMenu::create();
		additionalMenu2->setZOrder(3);
		// 46x46
		auto unrateSprite = CCSprite::createWithSpriteFrameName("GJ_rateDiffBtn_001.png");
		if (!unrateSprite->initWithFile("ghs-extension/GJ_unrateBtnMod_001.png"))
		{
			unrateSprite->initWithSpriteFrameName("GJ_rateDiffBtn_001.png");
			unrateSprite->setColor({ 255, 255, 0 });
		}
		auto unrateButton = gd::CCMenuItemSpriteExtra::create(unrateSprite, nullptr, this, menu_selector(LevelInfoLayer::Callback::unrateButton));
		unrateButton->setPosition({ 0, 51.f });

		auto addToHofSprite = CCSprite::createWithSpriteFrameName("GJ_likeBtn_001.png");
		if (!addToHofSprite->initWithFile("ghs-extension/GJ_addToHofBtn_001.png"))
		{
			addToHofSprite->initWithSpriteFrameName("GJ_likeBtn_001.png");
			addToHofSprite->setColor({ 255, 255, 0 });
		}
		auto addToHofButton = gd::CCMenuItemSpriteExtra::create(addToHofSprite, nullptr, this, menu_selector(LevelInfoLayer::Callback::addToHofButton));
		addToHofButton->setPosition({ 0, 0 });

		auto removeFromHofSprite = CCSprite::createWithSpriteFrameName("GJ_dislikeBtn_001.png");
		if (!removeFromHofSprite->initWithFile("ghs-extension/GJ_removeFromHofBtn_001.png"))
		{
			removeFromHofSprite->initWithSpriteFrameName("GJ_dislikeBtn_001.png");
			removeFromHofSprite->setColor({ 255, 255, 0 });
		}
		auto removeFromHofButton = gd::CCMenuItemSpriteExtra::create(removeFromHofSprite, nullptr, this, menu_selector(LevelInfoLayer::Callback::removeFromHofButton));
		removeFromHofButton->setPosition({ 0, -51.f });
		additionalMenu2->addChild(unrateButton);
		if (gd::GameManager::sharedState()->getIntGameVariable("0457") > 2) {
			additionalMenu2->addChild(addToHofButton);
			additionalMenu2->addChild(removeFromHofButton);
		}
		additionalMenu2->setPosition({ director->getScreenRight() / 2 - bg->getContentSize().width / 2 - unrateButton->getContentSize().width / 2 - 5, director->getScreenTop() / 2 });

		//submit and cancel buttons
		auto bottomMenu = CCMenu::create();
		bottomMenu->setPosition({ (director->getScreenRight()) / 2, (director->getScreenTop()) / 2 - 65 });
		bottomMenu->setZOrder(3);

		auto submitSprite = gd::ButtonSprite::create("Submit", 90, 0, 2.5f, true, "goldFont.fnt", "GJ_button_01.png", 30.0);
		submitSprite->setColor({ 166, 166, 166 });
		submitSprite->setTag(1);
		auto submitButton = gd::CCMenuItemSpriteExtra::create(submitSprite, nullptr, this, menu_selector(MyAwesomeRateLayer::submitButtonCallback));
		submitButton->setPosition(120, 0);
		submitButton->setTag(1);

		auto cancelSprite = gd::ButtonSprite::create("Cancel", 90, 0, 2.5f, true, "goldFont.fnt", "GJ_button_01.png", 30.0);
		auto cancelButton = gd::CCMenuItemSpriteExtra::create(cancelSprite, nullptr, this, menu_selector(MyAwesomeRateLayer::cancelButtonCallback));
		cancelButton->setPosition(-120, 0);

		auto difrateSprite = gd::ButtonSprite::create("Dif. Rate", 90, 0, 2.5f, true, "goldFont.fnt", "GJ_button_01.png", 30.0);
		difrateSprite->setColor({ 166, 166, 166 });
		difrateSprite->setTag(1);
		auto difrateButton = gd::CCMenuItemSpriteExtra::create(difrateSprite, nullptr, this, menu_selector(MyAwesomeRateLayer::difrateButtonCallback));
		difrateButton->setPosition(0, 0);
		difrateButton->setTag(2);

		bottomMenu->addChild(submitButton);
		bottomMenu->addChild(cancelButton);
		bottomMenu->addChild(difrateButton);
		bottomMenu->setTag(458);
		//bottomMenu->setTouchPriority(10);
		//adding all menus
		//this->addChild(sblbl);

		this->addChild(bottomMenu);
		this->addChild(featureMenu);
		this->addChild(rateMenu);
		this->addChild(additionalMenu);
		this->addChild(additionalMenu2);

		/*addingLayer->setZOrder(457);*/

		//this->getLayer()->addChild(addingLayer);
		this->setScale(0.1f);
		this->runAction(appearAction);
		return true;
	}

	static MyAwesomeRateLayer* create()
	{
		MyAwesomeRateLayer* obj = new MyAwesomeRateLayer;
		if (obj && obj->init())
		{
			obj->autorelease();
			return obj;
		}
		CC_SAFE_DELETE(obj);
		return nullptr;
	}

	void rateCallback(CCObject* btn)
	{
		auto director = CCDirector::sharedDirector();
		auto myRateLayer = MyAwesomeRateLayer::create();
		myRateLayer->setZOrder(100000);
		auto myLayer = static_cast<CCLayer*>(static_cast<CCNode*>(btn)->getUserObject());

		myLayer->addChild(myRateLayer);
		rateLayer = myRateLayer;
	}
};

void LevelInfoLayer::Callback::unrateButton(CCObject*)
{
	const auto flalert = gd::FLAlertLayer::create(&unrateProtocol, "Unrate level?", "Do you want to <cr>unrate</c> this level?", "No", "Yes", 300.f, false, 0);
	flalert->show();
}

void LevelInfoLayer::Callback::addToHofButton(CCObject*)
{
	const auto flalert = gd::FLAlertLayer::create(&addToHofProtocol, "Add to HoF?", "Do you want to <cg>add</c> this level \nto <cy>Hall of Fame</c>?", "No", "Yes", 290.f, false, 0);
	flalert->show();
}

void LevelInfoLayer::Callback::removeFromHofButton(CCObject*)
{
	const auto flalert = gd::FLAlertLayer::create(&removeFromHofProtocol, "Remove from HoF?", "Do you want to <cr>remove</c> this \nfrom <cy>Hall of Fame</c>?", "No", "Yes", 300.f, false, 0);
	flalert->show();
}

bool __fastcall LevelInfoLayer::LevelInfoLayer_init_hook(gd::LevelInfoLayer* self, void* edx, gd::GJGameLevel* gjlevel)
{
	level = gjlevel;
	gjlayer = self;
	//gjlevel->m_requiredCoins = 50; //lol what
	bool result = LevelInfoLayer::LevelInfoLayer_init(self, gjlevel);
	auto scrnright = CCDirector::sharedDirector()->getScreenRight();

	lvlmngr = gd::GameLevelManager::sharedState();
	savedlvls = lvlmngr->getSavedLevels();

	auto savedlvlscount = savedlvls->count();


	auto starSprite = CCSprite::createWithSpriteFrameName("GJ_starBtn_001.png");
	if (!starSprite->initWithFile("ghs-extension/GJ_starBtnMod_001.png"))
	{
		starSprite->initWithSpriteFrameName("GJ_starBtn_001.png");
		starSprite->setColor({ 255, 255, 0 });
	}
	auto rateButton = gd::CCMenuItemSpriteExtra::create(starSprite, nullptr, self, menu_selector(MyAwesomeRateLayer::rateCallback));

	auto menu2 = CCMenu::create();
	menu2->setPosition({ 30, 135 });



	if (gd::GameManager::sharedState()->getIntGameVariable("0457") > 0)
		menu2->addChild(rateButton);
	rateButton->setUserObject(self);

	if (gd::GameManager::sharedState()->getUserID() == gjlevel->getUserID())
		menu2->setPosition(30, 85);

	self->addChild(menu2);
	return result;
}
