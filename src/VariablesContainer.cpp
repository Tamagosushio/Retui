#include "VariablesContainer.hpp"
#include "UIUtils.hpp"
#include <ftxui/component/component_options.hpp>

namespace retui {

using namespace ftxui;

VariableBox::VariableBox(std::function<void()> on_change, std::function<void(VariableBox*)> on_delete) {
  InputOption option;
  option.on_change = on_change;
  input_name_ = Input(&name_, "Name", option);
  input_value_ = Input(&value_, "Regex Fragment", option);
  delete_button_ = Maybe(Button("DEL", [this, on_delete] { on_delete(this); }, ButtonOption::Animated(Color::Red)), &can_delete_);
  Component internal_container = Container::Horizontal({
    input_name_,
    input_value_,
    delete_button_
  });
  Add(internal_container);
}

Element VariableBox::OnRender() {
  return FocusedWindow(title_, hbox({
    input_name_->Render() | size(WIDTH, EQUAL, 15),
    separator(),
    input_value_->Render() | flex,
    separator(),
    delete_button_->Render() | center
  }), Focused());
}

bool VariableBox::IsEmpty() const {
  return name_.empty() && value_.empty();
}

VariablesContainer::VariablesContainer(std::function<void()> on_change)
  : on_change_(on_change) {
  container_ = Container::Vertical({});
  Add(container_);
  AddBox();
}

Element VariablesContainer::OnRender() {
  return FocusedWindow(" Variables ",
    container_->Render() | vscroll_indicator | frame | flex,
    Focused()
  );
}

std::vector<std::pair<std::string, std::string>> VariablesContainer::GetVariables() const {
  std::vector<std::pair<std::string, std::string>> vars;
  for (const auto& box : boxes_) {
    if (!box->IsEmpty() && !box->GetName().empty()) {
      vars.push_back({box->GetName(), box->GetValue()});
    }
  }
  return vars;
}

void VariablesContainer::SetVariables(const std::vector<std::pair<std::string, std::string>>& variables) {
  for (auto& box : boxes_) {
    box->Detach();
  }
  boxes_.clear();
  container_->DetachAllChildren();
  for (const auto& pair : variables) {
    AddBox(pair.first, pair.second);
  }
  AddBox();
  UpdateBoxesState();
}

void VariablesContainer::AddBox(const std::string& name, const std::string& value) {
  auto box = std::make_shared<VariableBox>(
    [this] {
      AddNewOnCondition();
      if (on_change_) on_change_();
    },
    [this](VariableBox* target) { RemoveBox(target); }
  );
  box->SetName(name);
  box->SetValue(value);
  boxes_.push_back(box);
  container_->Add(box);
  UpdateBoxesState();
}

void VariablesContainer::AddNewOnCondition() {
  if (!boxes_.empty() && !boxes_.back()->IsEmpty()) {
    AddBox();
  }
}

void VariablesContainer::RemoveBox(VariableBox* target) {
  auto it = std::find_if(
    boxes_.begin(), boxes_.end(),
    [target](const std::shared_ptr<VariableBox>& box) { return box.get() == target; }
  );
  if (it != boxes_.end()) {
    (*it)->Detach();
    boxes_.erase(it);
    if (on_change_) on_change_();
  }
  if (boxes_.empty()) {
    AddBox();
  } else {
    UpdateBoxesState();
  }
}

void VariablesContainer::UpdateBoxesState() {
  size_t total = boxes_.size();
  for (size_t i = 0; i < total; ++i) {
    boxes_[i]->SetTitle(" Variable " + std::to_string(i + 1) + "/" + std::to_string(total) + " ");
    bool can_delete = !(i == total - 1 && boxes_[i]->IsEmpty());
    boxes_[i]->SetCanDelete(can_delete);
  }
}

} // namespace retui
