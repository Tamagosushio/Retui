#include "VariableManager.hpp"
#include <algorithm>

namespace retui {

void VariableManager::SetVariable(const std::string& name, const std::string& value) {
  for (auto& pair : variables_) {
    if (pair.first == name) {
      pair.second = value;
      return;
    }
  }
  variables_.push_back({name, value});
}

void VariableManager::RemoveVariable(const std::string& name) {
  variables_.erase(
    std::remove_if(variables_.begin(), variables_.end(),
      [&name](const std::pair<std::string, std::string>& pair) { return pair.first == name; }
    ), variables_.end()
  );
}

void VariableManager::ClearVariables() {
  variables_.clear();
}

const std::vector<std::pair<std::string, std::string>>& VariableManager::GetVariables() const {
  return variables_;
}

void VariableManager::SetVariables(const std::vector<std::pair<std::string, std::string>>& vars) {
  variables_ = vars;
}

ExpandResult VariableManager::Expand(const std::string& input) const {
  std::set<std::string> visited;
  bool has_error = false;
  std::string error_message;
  std::string result = ExpandInternal(input, visited, has_error, error_message);
  if (has_error) {
    return {false, "", error_message};
  }
  return {true, result, ""};
}

std::string VariableManager::ExpandInternal(const std::string& input, std::set<std::string>& visited, bool& has_error, std::string& error_message) const {
  if (has_error) return "";
  std::string result;
  result.reserve(input.size());
  size_t i = 0;
  while (i < input.size()) {
    if (input[i] == '\\' && i + 2 < input.size() && input[i+1] == '{' && input[i+2] == '{') {
      result += "{{";
      i += 3;
    } else if (input[i] == '\\' && i + 2 < input.size() && input[i+1] == '}' && input[i+2] == '}') {
      result += "}}";
      i += 3;
    } else if (input[i] == '{' && i + 1 < input.size() && input[i+1] == '{') {
      size_t end_pos = input.find("}}", i + 2);
      if (end_pos != std::string::npos) {
        std::string var_name = input.substr(i + 2, end_pos - (i + 2));
        if (visited.contains(var_name)) {
          has_error = true;
          error_message = "Circular dependency detected: " + var_name;
          return "";
        }
        std::string var_value;
        bool found = false;
        for (const auto& pair : variables_) {
          if (pair.first == var_name) {
            var_value = pair.second;
            found = true;
            break;
          }
        }
        if (found) {
          visited.insert(var_name);
          result += ExpandInternal(var_value, visited, has_error, error_message);
          visited.erase(var_name);
        } else {
          result += "{{" + var_name + "}}";
        }
        i = end_pos + 2;
      } else {
        result += input[i];
        i++;
      }
    } else {
      result += input[i];
      i++;
    }
  }
  return result;
}

} // namespace retui
