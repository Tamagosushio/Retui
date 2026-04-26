#include "TuiController.hpp"

namespace retui {

using namespace ftxui;


/// @brief テスト文字列入力フィールドと削除ボタンを持つコンポーネントを構築する
/// @param on_change テキスト変更時に呼び出される処理
/// @param on_delete 削除ボタン押下時に呼び出される処理
TestStringBox::TestStringBox(std::function<void(TestStringBox*)> on_change, std::function<void(TestStringBox*)> on_delete) {
  InputOption option;
  option.on_change = [this, on_change]() { on_change(this); };
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
  std::string match_text;
  if (!match_result_.is_valid_regex) match_text = "Invalid Regex";
  else if (match_result_.is_match) match_text = "Matched!";
  else match_text = "No Match";
  Elements group_elements;
  if (show_capture_details_) {
    for (size_t i = 0; i < match_result_.captured_groups.size(); ++i) {
      group_elements.push_back(text(std::to_string(i) + ": " + match_result_.captured_groups[i]));
    }
  } else {
    if (match_result_.is_match) {
      group_elements.push_back(text("Captured: " + std::to_string(match_result_.captured_groups.size())));
    }
  }
  return hbox({
    input_box_->Render() | flex,
    separator(),
    vbox({
      text(match_text),
      vbox(std::move(group_elements)),
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

std::string TestStringBox::GetText() const {
  return test_string_;
}

void TestStringBox::SetMatchResult(const MatchResult& result) {
  match_result_ = result;
}

void TestStringBox::SetShowCaptureDetails(bool show) {
  show_capture_details_ = show;
}

TestStringsContainer::TestStringsContainer(std::function<void(TestStringBox*)> on_box_change)
  : on_box_change_(on_box_change) {
  test_strings_container_ = Container::Vertical({});
  Add(test_strings_container_);
  AddBox();
}

Element TestStringsContainer::OnRender() {
  return vbox({
    text("Test Strings"),
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

const std::vector<std::shared_ptr<TestStringBox>>& TestStringsContainer::GetBoxes() const {
  return boxes_;
}

void TestStringsContainer::SetShowCaptureDetails(bool show) {
  show_capture_details_ = show;
  for (auto& box : boxes_) {
    box->SetShowCaptureDetails(show);
  }
}

void TestStringsContainer::AddBox() {
  std::shared_ptr<TestStringBox> box = std::make_shared<TestStringBox>(
    [this](TestStringBox* target) { AddNewOnConditioner(); on_box_change_(target); },
    [this](TestStringBox* target) { RemoveBox(target); }
  );
  box->SetShowCaptureDetails(show_capture_details_);
  boxes_.push_back(box);
  test_strings_container_->Add(box);
  on_box_change_(box.get());
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

RegexContainer::RegexContainer(std::function<void(std::string)> on_regex_change, std::function<void()> on_toggle) {
  InputOption option;
  option.on_change = [this, on_regex_change]() { on_regex_change(input_regex_string_); };
  input_regex_ = Input(&input_regex_string_, "Input Regex", option);
  switch_captcha_button_ = Button(
    "Switch Display",
    on_toggle,
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
  }) | border;
}

bool RegexContainer::OnEvent(Event event) {
  return ComponentBase::OnEvent(event);
}

bool RegexContainer::Focusable() const {
  return ComponentBase::Focusable();
}

void RegexContainer::SetError(const std::string& error) {
  if (error.empty()) regex_compile_result_ = "Compile Result: Valid Regex";
  else regex_compile_result_ = "Compile Result: " + error;
}

TuiController::TuiController(RetuiApp* app) : app_(app) {
  test_strings_container_ = std::make_shared<TestStringsContainer>(
    [this](TestStringBox* box) { OnTestStringChange(box); }
  );
  regex_container_ = std::make_shared<RegexContainer>(
    [this](std::string regex) { OnRegexChange(regex); },
    [this]() { OnToggleDisplay(); }
  );
  Component controller_container = Container::Horizontal({
    regex_container_,
    test_strings_container_,
  });
  Add(controller_container);
}

Element TuiController::OnRender() {
  return hbox({
    regex_container_->Render() | flex,
    separator(),
    test_strings_container_->Render() | flex,
  });
}

bool TuiController::OnEvent(Event event) {
  return ComponentBase::OnEvent(event);
}

bool TuiController::Focusable() const {
  return ComponentBase::Focusable();
}

void TuiController::OnRegexChange(std::string regex) {
  app_->SetMainRegex(regex);
  auto result = app_->GetMatchResult();
  regex_container_->SetError(result.error_message);
  for (const auto& box : test_strings_container_->GetBoxes()) {
    EvaluateBox(box.get());
  }
}

void TuiController::OnTestStringChange(TestStringBox* box) {
  EvaluateBox(box);
}

void TuiController::EvaluateBox(TestStringBox* box) {
  app_->SetTestText(box->GetText());
  box->SetMatchResult(app_->GetMatchResult());
}

void TuiController::OnToggleDisplay() {
  show_capture_details_ = !show_capture_details_;
  test_strings_container_->SetShowCaptureDetails(show_capture_details_);
}

} // namespace retui