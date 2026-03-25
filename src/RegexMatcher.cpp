#include <iostream>
#include <memory>

#include "RegexMatcher.hpp"

namespace retui {

void RegexMatcher::Compile(const std::string& regex) {
  re2::RE2::Options options;
  options.set_log_errors(false);
  re2_ = std::make_unique<re2::RE2>(regex, options);
  is_valid_ = re2_->ok();
  error_message_ = is_valid_ ? "" : re2_->error();
}

void RegexMatcher::Execute(const std::string& text) {
  captured_groups_.clear();
  if (!re2_ || !is_valid_) {
    is_match_ = false;
    return;
  }
  int num_groups = re2_->NumberOfCapturingGroups();
  if (num_groups == 0) {
    is_match_ = re2::RE2::PartialMatch(text, *re2_);
    return;
  }
  std::vector<re2::StringPiece> matches(num_groups + 1);
  re2::StringPiece input(text);
  is_match_ = re2_->Match(input, 0, input.size(), re2::RE2::UNANCHORED, matches.data(), matches.size());
  if (IsMatch()) {
    captured_groups_.reserve(num_groups);
    for (int i = 1; i <= num_groups; i++) {
      captured_groups_.emplace_back(matches[i].data(), matches[i].size());
    }
  }
}

std::vector<std::string> RegexMatcher::GetCapturedGroups() const {
  return captured_groups_;
}

bool RegexMatcher::IsValid() const {
  return is_valid_;
}

bool RegexMatcher::IsMatch() const {
  return is_match_;
}

std::string RegexMatcher::GetErrorMessage() const {
  return error_message_;
}

void RegexMatcher::DebugPrint() const {
  std::cout << (IsValid() ? "Valid" : "Invalid") << std::endl;
  if (IsValid()) {
    std::cout << (IsMatch() ? "Match" : "Unmatch") << std::endl;
    for (int i = 0; const std::string& captured_group : GetCapturedGroups()) {
      std::cout << i++ << "-index:\t" << captured_group << std::endl;
    }
  }
  std::cout << (GetErrorMessage() == "" ? "No Error" : GetErrorMessage()) << std::endl;
}

} // namespace retui