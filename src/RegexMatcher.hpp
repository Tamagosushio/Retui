#pragma once

#include <string>
#include <vector>

#include <re2/re2.h>

namespace retui {

class RegexMatcher {
public:
  void Compile(const std::string& regex);
  void Execute(const std::string& text);
  std::vector<std::string> GetCapturedGroups() const;
  bool IsValid() const;
  bool IsMatch() const;
  std::string GetErrorMessage() const;
  void DebugPrint() const;
private:
  std::unique_ptr<re2::RE2> re2_;
  std::vector<std::string> captured_groups_;
  std::string error_message_;
  bool is_valid_ = false;
  bool is_match_ = false;
};

} // namespace retui