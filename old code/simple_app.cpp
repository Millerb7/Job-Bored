#include "simple_app.h"
#include "simple_handler.h" // So we can use SimpleHandler
#include "include/cef_app.h"
#include "include/wrapper/cef_helpers.h"  // this one defines CEF_REQUIRE_UI_THREAD

SimpleApp::SimpleApp() {}

CefRefPtr<CefBrowserProcessHandler> SimpleApp::GetBrowserProcessHandler() {
    return this;
}

void SimpleApp::OnContextInitialized() {
    CEF_REQUIRE_UI_THREAD();

    CefWindowInfo window_info;

    window_info.runtime_style = CEF_RUNTIME_STYLE_ALLOY;
    
    #if defined(OS_WIN)
    window_info.style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
    window_info.ex_style = WS_EX_APPWINDOW;
    window_info.parent_window = nullptr;
    
    window_info.bounds.x = 100;
    window_info.bounds.y = 100;
    window_info.bounds.width = 1024;
    window_info.bounds.height = 768;

    #endif
    

    CefBrowserSettings browser_settings;
    CefRefPtr<SimpleHandler> handler(new SimpleHandler());

    std::string start_url = "file:///C:/Users/wnd/Documents/github/Job-Bored/app/resources/home.html";

    CefBrowserHost::CreateBrowser(window_info, handler, start_url, browser_settings, nullptr, nullptr);
}
