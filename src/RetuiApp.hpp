#pragma once

#include <string>
#include <vector>

#include "RegexMatcher.hpp"

namespace retui {

class RetuiApp {
public:
  void SetMainRegex(const std::string& regex);
  void SetTestText(const std::string& text);
  MatchResult GetMatchResult() const;

private:
  std::string main_regex_;
  std::string test_text_;
  RegexMatcher regex_matcher_;
};

} // namespace retui
