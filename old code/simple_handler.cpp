#include "simple_handler.h"
#include <windows.h>
#include <unordered_map>
#include "include/wrapper/cef_helpers.h"

std::unordered_map<int, HWND> tab_windows;

SimpleHandler::SimpleHandler() {}

void SimpleHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
    CEF_REQUIRE_UI_THREAD();
    browser_ = browser;

    CefMessageRouterConfig config;
    message_router_ = CefMessageRouterBrowserSide::Create(config);
    message_router_->AddHandler(this, false);

    HWND hwnd = browser->GetHost()->GetWindowHandle();
    SetWindowTextW(hwnd, L"JobBored");

    int tabId = browser->GetIdentifier();
    tab_windows[tabId] = hwnd;
}

bool SimpleHandler::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                             CefRefPtr<CefFrame> frame,
                                             CefProcessId source_process,
                                             CefRefPtr<CefProcessMessage> message) {
    std::string name = message->GetName();
    HWND hwnd = browser->GetHost()->GetWindowHandle();

    if (name.rfind("create-tab:", 0) == 0) {
        auto rest = name.substr(11);
        auto sep = rest.find("::");
        int tabId = std::stoi(rest.substr(0, sep));
        std::string tab_url = rest.substr(sep + 2);

        RECT rect;
        GetClientRect(hwnd, &rect);
        int tab_top_offset = 100;

        HWND tab_hwnd = CreateWindowExW(
            0, L"STATIC", NULL,
            WS_CHILD | WS_VISIBLE,
            0, tab_top_offset,
            rect.right, rect.bottom - tab_top_offset,
            hwnd, NULL, GetModuleHandle(nullptr), NULL
        );

        CefWindowInfo tab_info;
        tab_info.SetAsChild(tab_hwnd, { 0, 0, rect.right, rect.bottom - tab_top_offset });

        CefBrowserSettings tab_settings;
        CefRefPtr<SimpleHandler> new_handler(new SimpleHandler());

        CefBrowserHost::CreateBrowser(tab_info, new_handler, tab_url, tab_settings, nullptr, nullptr);
        return true;
    } else if (name.rfind("navigate-tab:", 0) == 0) {
        auto rest = name.substr(13);
        auto sep = rest.find("::");
        int tabId = std::stoi(rest.substr(0, sep));
        std::string url = rest.substr(sep + 2);

        for (auto& entry : tab_windows) {
            ShowWindow(entry.second, SW_HIDE);
        }

        auto it = tab_windows.find(tabId);
        if (it != tab_windows.end()) {
            ShowWindow(it->second, SW_SHOW);
        }
        return true;
    } else if (name.rfind("focus-tab:", 0) == 0) {
        int tabId = std::stoi(name.substr(10));

        for (auto& entry : tab_windows) {
            ShowWindow(entry.second, SW_HIDE);
        }

        auto it = tab_windows.find(tabId);
        if (it != tab_windows.end()) {
            ShowWindow(it->second, SW_SHOW);
        }
        return true;
    } else if (name == "close") {
        PostMessage(hwnd, WM_CLOSE, 0, 0);
        return true;
    } else if (name == "minimize") {
        ShowWindow(hwnd, SW_MINIMIZE);
        return true;
    }
    return false;
}

void SimpleHandler::OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title) {
    CEF_REQUIRE_UI_THREAD();
    std::string js = "updateTabTitle(" + std::to_string(browser->GetIdentifier()) + ", '" + title.ToString() + "');";
    browser->GetMainFrame()->ExecuteJavaScript(js, browser->GetMainFrame()->GetURL(), 0);
}
