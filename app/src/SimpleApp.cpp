#include "SimpleApp.h"

void SimpleApp::OnBeforeCommandLineProcessing(const CefString& process_type,
                                              CefRefPtr<CefCommandLine> command_line) {
    // Most stable fallback is OpenGL:
    command_line->AppendSwitchWithValue("use-angle", "gl");

    // Optional: suppress GPU sandbox or enable remote debugging
    // command_line->AppendSwitch("disable-gpu-sandbox");
    // command_line->AppendSwitchWithValue("remote-debugging-port", "9222");
}
