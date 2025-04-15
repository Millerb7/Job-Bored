#pragma once
#include "include/cef_app.h"
#include "include/cef_browser_process_handler.h"

class SimpleApp : public CefApp,
                  public CefBrowserProcessHandler {
public:
    SimpleApp();

    CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override;
    void OnContextInitialized() override;

private:
    IMPLEMENT_REFCOUNTING(SimpleApp);
};
