#pragma once

#include <string>
#include <vector>
#include <functional>
#include <memory>

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

#include "RetuiApp.hpp"

namespace retui {

using namespace ftxui;

/// @brief テスト文字列とマッチ結果、削除ボタンを管理するコンポーネント
class TestStringBox : public ComponentBase {
public:
  explicit TestStringBox(std::function<void(TestStringBox*)> on_change, std::function<void(TestStringBox*)> on_delete);
  Element OnRender() override;
  bool OnEvent(Event event) override;
  bool Focusable() const override;
  bool IsEmpty() const;
  std::string GetText() const;
  void SetMatchResult(const MatchResult& result);
  void SetShowCaptureDetails(bool show);
private:
  std::string test_string_;
  MatchResult match_result_;
  Component input_box_;
  Component delete_button_;
  bool show_capture_details_ = false;
};

/// @brief 複数のTestStringBoxを管理し、動的に追加・削除を行うコンポーネント
class TestStringsContainer : public ComponentBase {
public:
  explicit TestStringsContainer(std::function<void(TestStringBox*)> on_box_change);
  Element OnRender() override;
  bool OnEvent(Event event) override;
  bool Focusable() const override;
  const std::vector<std::shared_ptr<TestStringBox>>& GetBoxes() const;
  void SetShowCaptureDetails(bool show);
private:
  void AddBox();
  void AddNewOnConditioner();
  void RemoveBox(TestStringBox* target);
  std::vector<std::shared_ptr<TestStringBox>> boxes_;
  Component test_strings_container_;
  std::function<void(TestStringBox*)> on_box_change_;
  bool show_capture_details_ = false;
};

class RegexContainer : public ComponentBase {
public:
  explicit RegexContainer(std::function<void(std::string)> on_regex_change, std::function<void()> on_toggle);
  Element OnRender() override;
  bool OnEvent(Event event) override;
  bool Focusable() const override;
  void SetError(const std::string& error);
private:
  std::string input_regex_string_;
  Component input_regex_;
  std::string regex_compile_result_ = "Compile Result: None";
  Component switch_captcha_button_;
};

class TuiController : public ComponentBase {
public:
  explicit TuiController(RetuiApp* app);
  Element OnRender() override;
  bool OnEvent(Event event) override;
  bool Focusable() const override;
private:
  void OnRegexChange(std::string regex);
  void OnTestStringChange(TestStringBox* box);
  void EvaluateBox(TestStringBox* box);
  void OnToggleDisplay();
  RetuiApp* app_;
  std::shared_ptr<TestStringsContainer> test_strings_container_;
  std::shared_ptr<RegexContainer> regex_container_;
  bool show_capture_details_ = false;
};

} // namespace retui