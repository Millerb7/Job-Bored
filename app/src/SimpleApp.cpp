#include "SimpleApp.h"

void SimpleApp::OnBeforeCommandLineProcessing(const CefString& process_type,
                                              CefRefPtr<CefCommandLine> command_line) {
    // Most stable fallback is OpenGL:
    command_line->AppendSwitchWithValue("use-angle", "gl");
}
