#include "RetuiApp.hpp"

namespace retui {

MatchResult RetuiApp::Evaluate(const std::string& regex, const std::string& text) {
  auto expand_res = variable_manager_.Expand(regex);
  if (!expand_res.success) {
    last_expanded_regex_ = "";
    return {false, false, expand_res.error_message, {}};
  }
  last_expanded_regex_ = expand_res.result;
  regex_matcher_.Compile(last_expanded_regex_);
  regex_matcher_.Execute(text);
  return regex_matcher_.GetMatchResult();
}

std::vector<MatchResult> RetuiApp::EvaluateMultiple(const std::string& regex, const std::vector<std::string>& texts) {
  auto expand_res = variable_manager_.Expand(regex);
  std::vector<MatchResult> results;
  results.reserve(texts.size());
  if (!expand_res.success) {
    last_expanded_regex_ = "";
    for (size_t i = 0; i < texts.size(); ++i) {
      results.push_back({false, false, expand_res.error_message, {}});
    }
    return results;
  }
  last_expanded_regex_ = expand_res.result;
  regex_matcher_.Compile(last_expanded_regex_);
  for (const auto& text : texts) {
    regex_matcher_.Execute(text);
    results.push_back(regex_matcher_.GetMatchResult());
  }
  return results;
}

} // namespace retui
