#include "ClipboardManager.hpp"

#ifdef _WIN32
#include <windows.h>
#else
#include <stdio.h>
#endif

namespace retui {

bool ClipboardManager::CopyToClipboard(const std::string& text) {
#ifdef _WIN32
  if (!OpenClipboard(nullptr)) {
    return false;
  }
  EmptyClipboard();
  HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, text.size() + 1);
  if (hMem) {
    memcpy(GlobalLock(hMem), text.c_str(), text.size() + 1);
    GlobalUnlock(hMem);
    SetClipboardData(CF_TEXT, hMem);
  } else {
    CloseClipboard();
    return false;
  }
  CloseClipboard();
  return true;
#elif defined(__linux__)
  FILE* pipe = popen("wl-copy 2>/dev/null || xclip -selection clipboard 2>/dev/null || xsel --clipboard --input 2>/dev/null", "w");
  if (pipe) {
    size_t written = fwrite(text.c_str(), 1, text.size(), pipe);
    int result = pclose(pipe);
    return (result == 0 && written == text.size());
  }
  return false;
#else
  return false;
#endif
}

} // namespace retui
