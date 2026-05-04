#include "RetuiApp.hpp"

namespace retui {

MatchResult RetuiApp::Evaluate(const std::string& regex, const std::string& text) {
  regex_matcher_.Compile(regex);
  if (!text.empty()) {
    regex_matcher_.Execute(text);
  }
  return regex_matcher_.GetMatchResult();
}

std::vector<MatchResult> RetuiApp::EvaluateMultiple(const std::string& regex, const std::vector<std::string>& texts) {
  regex_matcher_.Compile(regex);
  std::vector<MatchResult> results;
  results.reserve(texts.size());
  for (const auto& text : texts) {
    if (!text.empty()) {
      regex_matcher_.Execute(text);
    }
    results.push_back(regex_matcher_.GetMatchResult());
  }
  return results;
}

} // namespace retui
