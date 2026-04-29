#pragma once

#include <string>

namespace retui {

class ClipboardManager {
public:
  static bool CopyToClipboard(const std::string& text);
};

} // namespace retui
