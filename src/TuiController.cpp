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

bool TestStringBox::IsInputFocused() const {
  return input_box_->Focused();
}

bool TestStringBox::IsEmpty() const {
  return test_string_.empty();
}

std::string TestStringBox::GetText() const {
  return test_string_;
}

void TestStringBox::SetText(const std::string& text) {
  test_string_ = text;
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

void TestStringsContainer::InitWithTexts(const std::vector<std::string>& texts) {
  for (auto& box : boxes_) {
    box->Detach();
  }
  boxes_.clear();
  if (texts.empty()) {
    AddBox();
    return;
  }
  for (const auto& text : texts) {
    std::shared_ptr<TestStringBox> box = std::make_shared<TestStringBox>(
      [this](TestStringBox* target) { AddNewOnConditioner(); on_box_change_(target); },
      [this](TestStringBox* target) { RemoveBox(target); }
    );
    box->SetText(text);
    boxes_.push_back(box);
    test_strings_container_->Add(box);
  }
  UpdateBoxesState();
  if (!boxes_.empty() && !boxes_.back()->IsEmpty()) {
    AddBox();
  }
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

bool RegexContainer::IsInputFocused() const {
  return input_regex_->Focused();
}

std::string RegexContainer::GetText() const {
  return input_regex_string_;
}

void RegexContainer::SetText(const std::string& text) {
  input_regex_string_ = text;
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
  copy_button_ = Button(
    " Copy to Clipboard (Alt+C) ",
    [this]() { ExecuteCopy(); },
    ButtonOption::Animated(Color::Cyan)
  );
  reset_button_ = Button(
    " Reset ",
    [this]() { show_reset_modal_ = true; },
    ButtonOption::Animated(Color::RedLight)
  );
  const auto& app_state = app_->GetAppState();
  regex_container_->SetText(app_state.GetMainRegex());
  OnRegexChange(app_state.GetMainRegex());
  test_strings_container_->InitWithTexts(app_state.GetTestStrings());
  for (const auto& box : test_strings_container_->GetBoxes()) {
    EvaluateBox(box.get());
  }
  auto container = Container::Vertical({
    Container::Horizontal({
      regex_container_,
      test_strings_container_,
    }),
    Container::Horizontal({
      copy_button_,
      reset_button_
    })
  });
  auto main_layout = Renderer(container, [this] {
    return vbox({
      hbox({
        regex_container_->Render() | size(WIDTH, EQUAL, 50),
        separator(),
        test_strings_container_->Render() | flex,
      }) | flex,
      separator(),
      hbox({
        text(status_message_) | color(status_message_color_),
        filler(),
        copy_button_->Render(),
        text(" "),
        reset_button_->Render(),
      }),
    });
  });
  auto modal_component = Container::Vertical({
    Button("Yes, Reset", [this] {
      ExecuteReset();
      show_reset_modal_ = false;
    }, ButtonOption::Animated(Color::Red)),
    Button("Cancel", [this] { show_reset_modal_ = false; }, ButtonOption::Animated(Color::Blue)),
  });
  reset_modal_component_ = Renderer(modal_component, [modal_component] {
    return window(text(" Confirm Reset ") | bold | color(Color::RedLight),
      vbox({
        text("Are you sure you want to reset all state?"),
        text("This will delete retui.json."),
        separator(),
        hbox({
          filler(),
          modal_component->Render(),
          filler()
        })
      })
    ) | clear_under | center;
  });
  main_layout |= Modal(reset_modal_component_, &show_reset_modal_);
  Add(main_layout);
}

bool TuiController::OnEvent(Event event) {
  if (event == Event::AltH) event = Event::ArrowLeft;
  else if (event == Event::AltJ) event = Event::ArrowDown;
  else if (event == Event::AltK) event = Event::ArrowUp;
  else if (event == Event::AltL) event = Event::ArrowRight;
  else if (event == Event::AltC) {
    ExecuteCopy();
    return true;
  }
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

std::string TuiController::GetFocusedText() const {
  if (regex_container_->IsInputFocused()) {
    return regex_container_->GetText();
  }
  for (const auto& box : test_strings_container_->GetBoxes()) {
    if (box->IsInputFocused()) {
      return box->GetText();
    }
  }
  return "";
}

void TuiController::ExecuteCopy() {
  std::string text_to_copy = GetFocusedText();
  if (text_to_copy.empty()) {
    ShowMessage(" No text selected to copy ", Color::Yellow);
    return;
  }
  bool success = app_->CopyToClipboard(text_to_copy);
  if (success) {
    ShowMessage(" Copied to clipboard: " + text_to_copy + " ", Color::Green);
  } else {
    ShowMessage(" Failed to copy ", Color::Red);
  }
}

void TuiController::ShowMessage(const std::string& text, Color color) {
  status_message_ = text;
  status_message_color_ = color;
}

std::string TuiController::GetMainRegexText() const {
  return regex_container_->GetText();
}

std::vector<std::string> TuiController::GetAllTestStrings() const {
  std::vector<std::string> texts;
  for (const auto& box : test_strings_container_->GetBoxes()) {
    if (!box->IsEmpty()) {
      texts.push_back(box->GetText());
    }
  }
  return texts;
}

void TuiController::ExecuteReset() {
  app_->ResetState();
  regex_container_->SetText("");
  regex_container_->SetError("");
  test_strings_container_->InitWithTexts({});
  ShowMessage(" State reset successfully ", Color::Green);
}

} // namespace retui