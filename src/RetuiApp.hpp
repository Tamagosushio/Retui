#pragma once

#include <string>
#include <vector>

#include "RegexMatcher.hpp"
#include "AppState.hpp"

namespace retui {

class RetuiApp {
public:
  void SetMainRegex(const std::string& regex);
  void SetTestText(const std::string& text);
  MatchResult GetMatchResult() const;
  bool CopyToClipboard(const std::string& text) const;
  void LoadState(const std::string& filepath);
  void SaveState(const std::string& main_regex, const std::vector<std::string>& test_strings);
  void ResetState();
  const AppState& GetAppState() const { return app_state_; }

private:
  std::string main_regex_;
  std::string test_text_;
  RegexMatcher regex_matcher_;
  AppState app_state_;
};

} // namespace retui
