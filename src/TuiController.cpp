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
  delete_button_ = Maybe(Button(
    "   DEL   ",
    [this, on_delete]() { on_delete(this); },
    ButtonOption::Animated(Color::Red)
  ), &can_delete_);
  Component internal_container = Container::Horizontal({
    input_box_,
    delete_button_,
  });
  Add(internal_container);
}

Element TestStringBox::OnRender() {
  Element match_status;
  if (!match_result_.is_valid_regex) match_status = text("Invalid Regex") | color(Color::Red);
  else if (match_result_.is_match) match_status = text("Matched!") | color(Color::Green) | bold;
  else match_status = text("No Match") | color(Color::Yellow);
  Elements group_elements;
  for (size_t i = 0; i < match_result_.captured_groups.size(); ++i) {
    group_elements.push_back(text(std::to_string(i) + ": " + match_result_.captured_groups[i]));
  }
  Color border_color = Focused() ? Color(Color::Cyan1) : Color(Color::White);
  BorderStyle border_style = Focused() ? HEAVY : LIGHT;
  Decorator title_style = Focused() ? bold : nothing;
  return window(text(title_) | title_style, hbox({
    vbox({
      filler(),
      input_box_->Render() | border,
      filler(),
    }) | flex,
    separator(),
    text(" "),
    vbox({
      match_status,
      vbox(std::move(group_elements)) | flex,
    }) | size(WIDTH, GREATER_THAN, 20),
    text(" "),
    separator(),
    delete_button_->Render() | center,
  }) | color(Color::White), border_style) | color(border_color);
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

void TestStringBox::SetTitle(const std::string& title) {
  title_ = title;
}

void TestStringBox::SetCanDelete(bool can_delete) {
  can_delete_ = can_delete;
}

TestStringsContainer::TestStringsContainer(std::function<void(TestStringBox*)> on_box_change)
  : on_box_change_(on_box_change) {
  test_strings_container_ = Container::Vertical({});
  Add(test_strings_container_);
  AddBox();
}

Element TestStringsContainer::OnRender() {
  return window(text(" Test Strings "),
    test_strings_container_->Render() | vscroll_indicator | yframe | yflex
  );
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

void TestStringsContainer::AddBox() {
  std::shared_ptr<TestStringBox> box = std::make_shared<TestStringBox>(
    [this](TestStringBox* target) { AddNewOnConditioner(); on_box_change_(target); },
    [this](TestStringBox* target) { RemoveBox(target); }
  );
  boxes_.push_back(box);
  test_strings_container_->Add(box);
  UpdateBoxesState();
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
  } else {
    UpdateBoxesState();
  }
}

void TestStringsContainer::UpdateBoxesState() {
  size_t total = boxes_.size();
  for (size_t i = 0; i < total; ++i) {
    boxes_[i]->SetTitle(" Test String " + std::to_string(i + 1) + "/" + std::to_string(total) + " ");
    bool can_delete = !(i == total - 1 && boxes_[i]->IsEmpty());
    boxes_[i]->SetCanDelete(can_delete);
  }
}

RegexContainer::RegexContainer(std::function<void(std::string)> on_regex_change) {
  InputOption option;
  option.on_change = [this, on_regex_change]() { on_regex_change(input_regex_string_); };
  input_regex_ = Input(&input_regex_string_, "Input Regex", option);
  Component regex_container = Container::Vertical({ input_regex_ });
  Add(regex_container);
}

Element RegexContainer::OnRender() {
  Element compile_status;
  if (regex_compile_result_ == "Compile Result: Valid Regex") {
    compile_status = text(regex_compile_result_) | color(Color::Green) | bold;
  } else if (regex_compile_result_ != "Compile Result: None") {
    compile_status = paragraph(regex_compile_result_) | color(Color::Red) | bold;
  } else {
    compile_status = text(regex_compile_result_);
  }
  Color border_color = Focused() ? Color(Color::Cyan1) : Color(Color::White);
  BorderStyle border_style = Focused() ? HEAVY : LIGHT;
  Decorator title_style = Focused() ? bold : nothing;
  return window(text(" Main Regex ") | title_style, vbox({
    text(" Expression: ") | bold,
    input_regex_->Render() | border,
    separatorEmpty(),
    compile_status,
    filler(),
  }) | color(Color::White), border_style) | color(border_color);
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
    [this](std::string regex) { OnRegexChange(regex); }
  );
  Component controller_container = Container::Horizontal({
    regex_container_,
    test_strings_container_,
  });
  Add(controller_container);
}

Element TuiController::OnRender() {
  return hbox({
    regex_container_->Render() | size(WIDTH, EQUAL, 50),
    separator(),
    test_strings_container_->Render() | flex,
  });
}

bool TuiController::OnEvent(Event event) {
  if (event == Event::AltH) event = Event::ArrowLeft;
  else if (event == Event::AltJ) event = Event::ArrowDown;
  else if (event == Event::AltK) event = Event::ArrowUp;
  else if (event == Event::AltL) event = Event::ArrowRight;
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

} // namespace retui