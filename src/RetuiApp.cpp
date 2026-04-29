#include "RetuiApp.hpp"
#include "ClipboardManager.hpp"

namespace retui {

void RetuiApp::SetMainRegex(const std::string& regex) {
  main_regex_ = regex;
  regex_matcher_.Compile(main_regex_);
  if (!test_text_.empty()) {
    regex_matcher_.Execute(test_text_);
  }
}

void RetuiApp::SetTestText(const std::string& text) {
  test_text_ = text;
  regex_matcher_.Execute(test_text_);
}

MatchResult RetuiApp::GetMatchResult() const {
  return regex_matcher_.GetMatchResult();
}

bool RetuiApp::CopyToClipboard(const std::string& text) const {
  return ClipboardManager::CopyToClipboard(text);
}

} // namespace retui
