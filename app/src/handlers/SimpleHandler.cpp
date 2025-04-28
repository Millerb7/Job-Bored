#include "SimpleHandler.h"
#include <windows.h>
#include <unordered_map>
#include "wrapper/cef_helpers.h"
#include <QDebug>

std::unordered_map<int, HWND> tab_windows;

SimpleHandler::SimpleHandler(std::function<void(CefRefPtr<CefBrowser>)> onBrowserCreated)
    : onBrowserCreated_(onBrowserCreated) {}

void SimpleHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
    CEF_REQUIRE_UI_THREAD();

    browser_ = browser;
    if (onBrowserCreated_) {
        onBrowserCreated_(browser_);
    }

        // Setup message router
        CefMessageRouterConfig config;
        message_router_ = CefMessageRouterBrowserSide::Create(config);
        message_router_->AddHandler(this, false);
    
        HWND hwnd = browser->GetHost()->GetWindowHandle();
        SetWindowTextW(hwnd, L"CEF Embedded");
    
        RECT rect;
        GetClientRect(GetParent(hwnd), &rect);
        MoveWindow(hwnd, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, TRUE);
        browser->GetHost()->WasResized();
    
        qDebug() << "[SimpleHandler] Browser HWND resized after creation.";
    
        int tabId = browser->GetIdentifier();
        tab_windows[tabId] = hwnd;
}

bool SimpleHandler::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                             CefRefPtr<CefFrame> frame,
                                             CefProcessId source_process,
                                             CefRefPtr<CefProcessMessage> message) {
    std::string name = message->GetName();
    HWND hwnd = browser->GetHost()->GetWindowHandle();

    qDebug() << "[SimpleHandler] Received message:" << QString::fromStdString(name);

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

        qDebug() << "[SimpleHandler] Created new tab with ID:" << tabId << "URL:" << QString::fromStdString(tab_url);
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

        qDebug() << "[SimpleHandler] Navigated to tab ID:" << tabId;
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

        qDebug() << "[SimpleHandler] Focused tab ID:" << tabId;
        return true;

    } else if (name == "close") {
        PostMessage(hwnd, WM_CLOSE, 0, 0);
        qDebug() << "[SimpleHandler] Close requested.";
        return true;

    } else if (name == "minimize") {
        ShowWindow(hwnd, SW_MINIMIZE);
        qDebug() << "[SimpleHandler] Minimize requested.";
        return true;
    }

    return false;
}

void SimpleHandler::OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title) {
    CEF_REQUIRE_UI_THREAD();
    std::string js = "updateTabTitle(" + std::to_string(browser->GetIdentifier()) + ", '" + title.ToString() + "');";
    browser->GetMainFrame()->ExecuteJavaScript(js, browser->GetMainFrame()->GetURL(), 0);

    qDebug() << "[SimpleHandler] Title changed for Browser ID:" << browser->GetIdentifier()
             << "to:" << QString::fromStdString(title.ToString());
}
