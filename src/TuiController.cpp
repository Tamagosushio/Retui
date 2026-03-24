#include "TuiController.hpp"

namespace retui {

using namespace ftxui;


/// @brief 入力時と削除時のコールバックを設定
/// @param on_change テキスト変更時の処理
/// @param on_delete 削除ボタン押下時の処理
TestStringBox::TestStringBox(std::function<void()> on_change, std::function<void(TestStringBox*)> on_delete) {
  InputOption option;
  option.on_change = on_change;
  input_box_ = Input(&test_string_, "Input Test String", option);
  delete_button_ = Button(
    "Del",
    [this, on_delete]() { on_delete(this); },
    ButtonOption::Animated(Color::Red)
  );
  Component internal_container = Container::Horizontal({
    input_box_,
    delete_button_,
  });
  Add(internal_container);
}

Element TestStringBox::OnRender() {
  return hbox({
    input_box_->Render(),
    separator(),
    vbox({
      text(match_result_),
      delete_button_->Render(),
    }),
  }) | border;
}

bool TestStringBox::OnEvent(Event event) {
  return ComponentBase::OnEvent(event);
}

bool TestStringBox::Focusable() const {
  return ComponentBase::Focusable();
}

bool TestStringBox::IsEmpty() const {
  return test_string_.empty();
}


TestStringsContainer::TestStringsContainer() {
  test_strings_container_ = Container::Vertical({});
  Add(test_strings_container_);
  AddBox();
}

Element TestStringsContainer::OnRender() {
  return vbox({
    text("Container List"),
    separator(),
    test_strings_container_->Render(),
  }) | border;
}

bool TestStringsContainer::OnEvent(Event event) {
  return ComponentBase::OnEvent(event);
}

bool TestStringsContainer::Focusable() const {
  return ComponentBase::Focusable();
}

void TestStringsContainer::AddBox() {
  std::shared_ptr<TestStringBox> box = std::make_shared<TestStringBox>(
    [this]() { AddNewOnConditioner(); },
    [this](TestStringBox* target) { RemoveBox(target); }
  );
  boxes_.push_back(box);
  test_strings_container_->Add(box);
}

void TestStringsContainer::AddNewOnConditioner() {
  if (!boxes_.empty() && !boxes_.back()->IsEmpty()) {
    AddBox();
  }
}

void TestStringsContainer::RemoveBox(TestStringBox* target) {
  auto iter = std::find_if(boxes_.begin(), boxes_.end(),
    [target](const std::shared_ptr<TestStringBox>& box) {
      return box.get() == target;
    }
  );
  if (iter != boxes_.end()) {
    (*iter)->Detach();
    boxes_.erase(iter);
  }
  if (boxes_.empty()) {
    AddBox();
  }
}


RegexContainer::RegexContainer() {
  input_regex_ = Input(&input_regex_string_, "Input Regex");
  switch_captcha_button_ = Button(
    "Switch Captcha",
    [](){},
    ButtonOption::Animated(Color::Blue)
  );
  Component regex_container = Container::Vertical({
    input_regex_,
    switch_captcha_button_,
  });
  Add(regex_container);
}

Element RegexContainer::OnRender() {
  return vbox({
    input_regex_->Render(),
    text(regex_compile_result_),
    switch_captcha_button_->Render(),
  });
}

bool RegexContainer::OnEvent(Event event) {
  return ComponentBase::OnEvent(event);
}

bool RegexContainer::Focusable() const {
  return ComponentBase::Focusable();
}


TuiController::TuiController() {
  test_strings_container_ = std::make_shared<TestStringsContainer>();
  regex_container_ = std::make_shared<RegexContainer>();
  Component controller_container = Container::Horizontal({
    test_strings_container_,
    regex_container_,
  });
  Add(controller_container);
}

Element TuiController::OnRender() {
  return hbox({
    regex_container_->Render(),
    separator(),
    test_strings_container_->Render(),
  });
}

bool TuiController::OnEvent(Event event) {
  return ComponentBase::OnEvent(event);
}

bool TuiController::Focusable() const {
  return ComponentBase::Focusable();
}


} // namespace retui