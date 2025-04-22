#pragma once
#include "include/cef_app.h"

class SimpleApp : public CefApp {
public:
    SimpleApp() = default;

    void OnBeforeCommandLineProcessing(
        const CefString& process_type,
        CefRefPtr<CefCommandLine> command_line) override;

private:
    IMPLEMENT_REFCOUNTING(SimpleApp);
};