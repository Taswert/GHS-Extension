#include "pch.h"

class AdminCheck : CCObject
{
public:
    void adminCheckCallback(CCObject*);
    static void adminInitCheck(CCObject* self);
    void onAdminCallbackReq(CCHttpClient* client, CCHttpResponse* response);
    void onAdminInitReq(CCHttpClient* client, CCHttpResponse* response);
};