#include "AppState.hpp"

#include <fstream>
#include <filesystem>

#include <nlohmann/json.hpp>

namespace retui {

using json = nlohmann::json;

bool AppState::Load(const std::string& filepath) {
  filepath_ = filepath;
  std::ifstream file(filepath_);
  if (!file.is_open()) {
    Save();
    return true;
  }
  try {
    json j;
    file >> j;
    if (j.contains("main_regex") && j["main_regex"].is_string()) {
      main_regex_ = j["main_regex"];
    }
    if (j.contains("test_strings") && j["test_strings"].is_array()) {
      test_strings_.clear();
      for (const auto& item : j["test_strings"]) {
        if (item.is_string()) {
          test_strings_.push_back(item);
        }
      }
    }
    return true;
  } catch (const json::exception&) {
    return false;
  }
}

bool AppState::Save() const {
  json j;
  j["main_regex"] = main_regex_;
  j["test_strings"] = test_strings_;
  std::ofstream file(filepath_);
  if (!file.is_open()) {
    return false;
  }
  file << j.dump(2);
  return file.good();
}

void AppState::Reset() {
  main_regex_.clear();
  test_strings_.clear();
  if (!filepath_.empty() && std::filesystem::exists(filepath_)) {
    std::filesystem::remove(filepath_);
  }
}

std::string AppState::GetMainRegex() const {
  return main_regex_;
}

void AppState::SetMainRegex(const std::string& main_regex) {
  main_regex_ = main_regex;
}

std::vector<std::string> AppState::GetTestStrings() const {
  return test_strings_;
}

void AppState::SetTestStrings(const std::vector<std::string>& test_strings) {
  test_strings_ = test_strings;
}

} // namespace retui
