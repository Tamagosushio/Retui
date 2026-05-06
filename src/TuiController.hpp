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
#include "AppState.hpp"
#include "VariablesContainer.hpp"

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
  bool IsInputFocused() const;
  std::string GetText() const;
  void SetText(const std::string& text);
  void SetMatchResult(const MatchResult& result);
  void SetTitle(const std::string& title);
  void SetCanDelete(bool can_delete);
private:
  std::string test_string_;
  MatchResult match_result_;
  Component input_box_;
  Component delete_button_;
  std::string title_ = " Test String ";
  bool can_delete_ = true;
};

/// @brief 複数のTestStringBoxを管理し、動的に追加・削除を行うコンポーネント
class TestStringsContainer : public ComponentBase {
public:
  explicit TestStringsContainer(std::function<void(TestStringBox*)> on_box_change);
  Element OnRender() override;
  bool OnEvent(Event event) override;
  bool Focusable() const override;
  const std::vector<std::shared_ptr<TestStringBox>>& GetBoxes() const;
  void InitWithTexts(const std::vector<std::string>& texts);
private:
  void AddBox();
  void AddNewOnConditioner();
  void RemoveBox(TestStringBox* target);
  void UpdateBoxesState();
  std::vector<std::shared_ptr<TestStringBox>> boxes_;
  Component test_strings_container_;
  std::function<void(TestStringBox*)> on_box_change_;
};

class RegexContainer : public ComponentBase {
public:
  explicit RegexContainer(std::function<void(std::string)> on_regex_change);
  Element OnRender() override;
  bool OnEvent(Event event) override;
  bool Focusable() const override;
  bool IsInputFocused() const;
  std::string GetText() const;
  void SetText(const std::string& text);
  void SetError(const std::string& error);
  void SetExpandedRegex(const std::string& expanded);
private:
  std::string input_regex_string_;
  Component input_regex_;
  std::string regex_compile_result_ = "Compile Result: None";
  std::string expanded_regex_ = "";
};

class TuiController : public ComponentBase {
public:
  explicit TuiController(RetuiApp* app, AppState* app_state);
  bool OnEvent(Event event) override;
  bool Focusable() const override;
  std::string GetMainRegexText() const;
  std::vector<std::string> GetAllTestStrings() const;
  std::vector<std::pair<std::string, std::string>> GetAllVariables() const;

private:
  void OnRegexChange(std::string regex);
  void OnTestStringChange(TestStringBox* box);
  void EvaluateBox(TestStringBox* box);
  void OnVariablesChange();
  std::string GetFocusedText() const;
  void ExecuteCopy();
  void ShowMessage(const std::string& text, Color color = Color::White);
  void ExecuteReset();
  RetuiApp* app_;
  AppState* app_state_;
  std::shared_ptr<TestStringsContainer> test_strings_container_;
  std::shared_ptr<RegexContainer> regex_container_;
  std::shared_ptr<VariablesContainer> variables_container_;
  Component copy_button_;
  Component reset_button_;
  bool show_reset_modal_ = false;
  Component reset_modal_component_;
  std::string status_message_ = "";
  Color status_message_color_ = Color::White;
};

} // namespace retui