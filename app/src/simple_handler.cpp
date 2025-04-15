#include "simple_handler.h"
#include "include/wrapper/cef_helpers.h"

SimpleHandler::SimpleHandler() {}

CefRefPtr<CefLifeSpanHandler> SimpleHandler::GetLifeSpanHandler() {
    return this;
}

void SimpleHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
    CEF_REQUIRE_UI_THREAD();
    browser_ = browser;
}

void SimpleHandler::LoadURL(const std::string& url) {
    if (browser_)
        browser_->GetMainFrame()->LoadURL(url);
}
