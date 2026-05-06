#include "HelpModal.hpp"
#include <ftxui/component/event.hpp>

namespace retui {

using namespace ftxui;

HelpModal::HelpModal(std::function<void()> on_close) {
  close_button_ = Button("Close", on_close, ButtonOption::Animated(Color::Blue));
  Add(close_button_);
}

Element HelpModal::OnRender() {
  auto controls = vbox({
    text("  [Controls]  ") | bold | center | color(Color::GreenLight),
    text(" Alt+(H/J/K/L)   : Left/Down/Up/Right"),
    text(" Tab / Shift+Tab : Change focus"),
    text(" Enter / Space   : Interact with button/input"),
    text(" Alt+C           : Copy expanded regex to clipboard"),
  });
  auto re2_syntax = vbox({
    text("  [RE2 Syntax Quick Reference]  ") | bold | center | color(Color::GreenLight),
    hbox({ text(" \\d : "), text("Digit (0-9)") }) | xflex,
    hbox({ text(" \\D : "), text("Not a digit") }) | xflex,
    hbox({ text(" \\w : "), text("Word char (a-zA-Z0-9_)") }) | xflex,
    hbox({ text(" \\W : "), text("Not a word char") }) | xflex,
    hbox({ text(" \\s : "), text("Whitespace") }) | xflex,
    hbox({ text(" \\S : "), text("Not whitespace") }) | xflex,
    hbox({ text(" \\b : "), text("Word boundary") }) | xflex,
    hbox({ text(" \\B : "), text("Not a word boundary") }) | xflex,
  });
  auto content = vbox({
    controls | xflex,
    separator(),
    re2_syntax | xflex,
  }) | border;
  return window(text(" Help & Shortcuts ") | bold | color(Color::Yellow),
    vbox({
      content,
      hbox({
        filler(),
        close_button_->Render(),
        filler()
      })
    })
  ) | clear_under | center;
}

bool HelpModal::OnEvent(Event event) {
  return ComponentBase::OnEvent(event);
}

bool HelpModal::Focusable() const {
  return ComponentBase::Focusable();
}

} // namespace retui
