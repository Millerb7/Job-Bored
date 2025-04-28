
#pragma once

#include "cef_client.h"
#include "cef_life_span_handler.h"
#include "cef_display_handler.h"
#include "wrapper/cef_message_router.h"
#include <functional>

class SimpleHandler : public CefClient,
                      public CefLifeSpanHandler,
                      public CefDisplayHandler,
                      public CefMessageRouterBrowserSide::Handler {
public:
    SimpleHandler(std::function<void(CefRefPtr<CefBrowser>)> onBrowserCreated = nullptr);
    void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;
    bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                   CefRefPtr<CefFrame> frame,
                                   CefProcessId source_process,
                                   CefRefPtr<CefProcessMessage> message) override;
    void OnTitleChange(CefRefPtr<CefBrowser> browser,
                       const CefString& title) override;

    CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override { return this; }
    CefRefPtr<CefDisplayHandler> GetDisplayHandler() override { return this; }

private:
    CefRefPtr<CefBrowser> browser_;
    CefRefPtr<CefMessageRouterBrowserSide> message_router_;
    std::function<void(CefRefPtr<CefBrowser>)> onBrowserCreated_;
    IMPLEMENT_REFCOUNTING(SimpleHandler);
};
