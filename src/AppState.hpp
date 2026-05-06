#pragma once

#include <string>
#include <vector>
#include <unordered_map>

namespace retui {

class AppState {
public:
  bool Load(const std::string& filepath);
  bool Save() const;
  void Reset();
  std::string GetMainRegex() const;
  void SetMainRegex(const std::string& main_regex);
  std::vector<std::string> GetTestStrings() const;
  void SetTestStrings(const std::vector<std::string>& test_strings);
  std::vector<std::pair<std::string, std::string>> GetVariables() const;
  void SetVariables(const std::vector<std::pair<std::string, std::string>>& variables);

private:
  std::string filepath_;
  std::string main_regex_;
  std::vector<std::string> test_strings_;
  std::vector<std::pair<std::string, std::string>> variables_;
};

} // namespace retui
