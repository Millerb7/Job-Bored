#pragma once

#include "include/cef_client.h"
#include "include/wrapper/cef_helpers.h"

class SimpleHandler : public CefClient,
                      public CefLifeSpanHandler {
public:
    SimpleHandler() {}

    CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override {
        return this;
    }

    void OnAfterCreated(CefRefPtr<CefBrowser> browser) override {
        CEF_REQUIRE_UI_THREAD();
        browser_ = browser;
    }

    void LoadURL(const std::string& url) {
        if (browser_) {
            browser_->GetMainFrame()->LoadURL(url);
        }
    }

private:
    CefRefPtr<CefBrowser> browser_;
    IMPLEMENT_REFCOUNTING(SimpleHandler);
};

class SimpleApp : public CefApp,
                  public CefBrowserProcessHandler {
public:
    CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override {
        return this;
    }

    void OnContextInitialized() override {
        CEF_REQUIRE_UI_THREAD();

        CefWindowInfo window_info;
        window_info.SetAsPopup(nullptr, "JobBored Browser");

        CefBrowserSettings browser_settings;
        CefRefPtr<SimpleHandler> handler(new SimpleHandler());

        // Load local HTML as your custom homepage
        std::string path = "file://" + std::string(CefString(CefCommandLine::GetGlobalCommandLine()->GetProgram())) + "/resources/home.html";

        CefBrowserHost::CreateBrowser(
            window_info, handler,
            path,
            browser_settings, nullptr, nullptr
        );        
    }

private:
    IMPLEMENT_REFCOUNTING(SimpleApp);
};
