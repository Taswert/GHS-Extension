#include "pch.h"

class AdminCheck : CCObject
{
public:
    void adminCheckCallback(CCObject*);
    void adminInitCheck();
    void onAdminCallbackReq(CCHttpClient* client, CCHttpResponse* response);
};