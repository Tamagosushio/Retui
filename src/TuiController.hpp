#pragma once

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
  explicit TestStringBox(std::function<void()> on_change, std::function<void(TestStringBox*)> on_delete);
  Element OnRender() override;
  bool OnEvent(Event event) override;
  bool Focusable() const override;
  bool IsEmpty() const;
private:
  std::string test_string_;
  std::string match_result_ = "Result";
  Component input_box_;
  Component delete_button_;
};

/// @brief 複数のTestStringBoxを管理し、動的に追加・削除を行うコンポーネント
class TestStringsContainer : public ComponentBase {
public:
  TestStringsContainer();
  Element OnRender() override;
  bool OnEvent(Event event) override;
  bool Focusable() const override;
private:
  void AddBox();
  void AddNewOnConditioner();
  void RemoveBox(TestStringBox* target);
  std::vector<std::shared_ptr<TestStringBox>> boxes_;
  Component test_strings_container_;
};

class RegexContainer : public ComponentBase {
public:
  RegexContainer();
  Element OnRender() override;
  bool OnEvent(Event event) override;
  bool Focusable() const override;
private:
  std::string input_regex_string_;
  Component input_regex_;
  std::string regex_compile_result_ = "Compile Result";
  Component switch_captcha_button_;
};

class TuiController : public ComponentBase {
public:
  TuiController();
  Element OnRender() override;
  bool OnEvent(Event event) override;
  bool Focusable() const override;
private:
  std::shared_ptr<TestStringsContainer> test_strings_container_;
  std::shared_ptr<RegexContainer> regex_container_;
};

} // namespace retui