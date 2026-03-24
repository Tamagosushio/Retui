#include <string>
#include <vector>
#include <functional>

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

namespace retui {

using namespace ftxui;

/// @brief テスト文字列とマッチ結果、削除ボタンを管理するコンポーネント
class TestStringBox : public ComponentBase {
public:
  /// @brief 入力時と削除時のコールバックを設定
  /// @param on_change テキスト変更時の処理
  /// @param on_delete 削除ボタン押下時の処理
  explicit TestStringBox(std::function<void()> on_change, std::function<void(TestStringBox*)> on_delete) {
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
  Element OnRender() override {
    return hbox({
      input_box_->Render(),
      separator(),
      vbox({
        text(match_result_),
        delete_button_->Render(),
      }),
    }) | border;
  }
  bool OnEvent(Event event) override {
    return ComponentBase::OnEvent(event);
  }
  bool Focusable() const override {
    return ComponentBase::Focusable();
  }
  bool IsEmpty() const {
    return test_string_.empty();
  }
private:
  std::string test_string_;
  std::string match_result_ = "Result";
  Component input_box_;
  Component delete_button_;
};

/// @brief 複数のTestStringBoxを管理し、動的に追加・削除を行うコンポーネント
class TestStringsContainer : public ComponentBase {
public:
  TestStringsContainer() {
    test_strings_container_ = Container::Vertical({});
    Add(test_strings_container_);
    AddBox();
  }
  Element OnRender() override {
    return vbox({
      text("Container List"),
      separator(),
      test_strings_container_->Render(),
    }) | border;
  }
  bool OnEvent(Event event) override {
    return ComponentBase::OnEvent(event);
  }
  bool Focusable() const override {
    return ComponentBase::Focusable();
  }
private:
  void AddBox() {
    std::shared_ptr<TestStringBox> box = std::make_shared<TestStringBox>(
      [this]() { AddNewOnConditioner(); },
      [this](TestStringBox* target) { RemoveBox(target); }
    );
    boxes_.push_back(box);
    test_strings_container_->Add(box);
  }
  void AddNewOnConditioner() {
    if (!boxes_.empty() && !boxes_.back()->IsEmpty()) {
      AddBox();
    }
  }
  void RemoveBox(TestStringBox* target) {
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
  std::vector<std::shared_ptr<TestStringBox>> boxes_;
  Component test_strings_container_;
};

class RegexContainer : public ComponentBase {
public:
  RegexContainer() {
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
  Element OnRender() override {
    return vbox({
      input_regex_->Render(),
      text(regex_compile_result_),
      switch_captcha_button_->Render(),
    });
  }
  bool OnEvent(Event event) override {
    return ComponentBase::OnEvent(event);
  }
  bool Focusable() const override {
    return ComponentBase::Focusable();
  }
private:
  std::string input_regex_string_;
  Component input_regex_;
  std::string regex_compile_result_ = "Compile Result";
  Component switch_captcha_button_;
};

} // namespace retui

int main() {
  std::shared_ptr<retui::RegexContainer> app = std::make_shared<retui::RegexContainer>();
  auto screen = ftxui::ScreenInteractive::Fullscreen();
  screen.Loop(app);
}