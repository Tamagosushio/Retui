#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/elements.hpp>

namespace retui {

class VariableBox : public ftxui::ComponentBase {
public:
  explicit VariableBox(std::function<void()> on_change, std::function<void(VariableBox*)> on_delete);
  ftxui::Element OnRender() override;
  bool IsEmpty() const;
  std::string GetName() const { return name_; }
  std::string GetValue() const { return value_; }
  void SetName(const std::string& name) { name_ = name; }
  void SetValue(const std::string& value) { value_ = value; }

private:
  std::string name_;
  std::string value_;
  ftxui::Component input_name_;
  ftxui::Component input_value_;
  ftxui::Component delete_button_;
};

class VariablesContainer : public ftxui::ComponentBase {
public:
  explicit VariablesContainer(std::function<void()> on_change);
  ftxui::Element OnRender() override;
  std::vector<std::pair<std::string, std::string>> GetVariables() const;
  void SetVariables(const std::vector<std::pair<std::string, std::string>>& variables);

private:
  void AddBox(const std::string& name = "", const std::string& value = "");
  void AddNewOnCondition();
  void RemoveBox(VariableBox* target);
  std::vector<std::shared_ptr<VariableBox>> boxes_;
  ftxui::Component container_;
  std::function<void()> on_change_;
};

} // namespace retui
