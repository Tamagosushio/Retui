#pragma once

#include <string>
#include <vector>

#include "RegexMatcher.hpp"

namespace retui {

class RetuiApp {
public:
  MatchResult Evaluate(const std::string& regex, const std::string& text);
  std::vector<MatchResult> EvaluateMultiple(const std::string& regex, const std::vector<std::string>& texts);

private:
  RegexMatcher regex_matcher_;
};

} // namespace retui
