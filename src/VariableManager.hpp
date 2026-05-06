#pragma once

#include <string>
#include <vector>
#include <set>

namespace retui {

struct ExpandResult {
  bool success;
  std::string result;
  std::string error_message;
};

class VariableManager {
public:
  void SetVariable(const std::string& name, const std::string& value);
  void RemoveVariable(const std::string& name);
  void ClearVariables();
  const std::vector<std::pair<std::string, std::string>>& GetVariables() const;
  void SetVariables(const std::vector<std::pair<std::string, std::string>>& vars);
  ExpandResult Expand(const std::string& input) const;

private:
  std::vector<std::pair<std::string, std::string>> variables_;
  std::string ExpandInternal(const std::string& input, std::set<std::string>& visited, bool& has_error, std::string& error_message) const;
};

} // namespace retui
