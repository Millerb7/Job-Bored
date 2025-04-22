#pragma once
#include "include/cef_app.h"

class SimpleApp : public CefApp {
public:
    void OnBeforeCommandLineProcessing(const CefString& process_type,
                                       CefRefPtr<CefCommandLine> command_line) override {
        command_line->AppendSwitchWithValue("use-angle", "gl"); // or "d3d11", "vulkan"
    }

private:
    IMPLEMENT_REFCOUNTING(SimpleApp);
};
