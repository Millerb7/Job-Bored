#pragma once
#include "include/cef_client.h"

class SimpleHandler : public CefClient,
                      public CefLifeSpanHandler {
public:
    SimpleHandler();

    CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override;
    void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;
    void LoadURL(const std::string& url);

private:
    CefRefPtr<CefBrowser> browser_;
    IMPLEMENT_REFCOUNTING(SimpleHandler);
};
