#pragma once

#include <string>
#include <vector>

#include "RegexMatcher.hpp"
#include "VariableManager.hpp"

namespace retui {

class RetuiApp {
public:
  MatchResult Evaluate(const std::string& regex, const std::string& text);
  std::vector<MatchResult> EvaluateMultiple(const std::string& regex, const std::vector<std::string>& texts);
  VariableManager& GetVariableManager() { return variable_manager_; }
  const VariableManager& GetVariableManager() const { return variable_manager_; }
  std::string GetLastExpandedRegex() const { return last_expanded_regex_; }

private:
  RegexMatcher regex_matcher_;
  VariableManager variable_manager_;
  std::string last_expanded_regex_;
};

} // namespace retui
