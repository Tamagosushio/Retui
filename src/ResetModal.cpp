#include "ResetModal.hpp"

#include <ftxui/component/event.hpp>

namespace retui {

using namespace ftxui;

ResetModal::ResetModal(std::function<void()> on_confirm, std::function<void()> on_cancel) {
  modal_component_ = Container::Vertical({
    Button("Yes, Reset", on_confirm, ButtonOption::Animated(Color::Red)),
    Button("Cancel", on_cancel, ButtonOption::Animated(Color::Blue)),
  });
  Add(modal_component_);
}

Element ResetModal::OnRender() {
  return window(text(" Confirm Reset ") | bold | color(Color::RedLight),
    vbox({
      text("Are you sure you want to reset all state?") | center,
      text("This will delete retui.json.") | center,
      separator(),
      hbox({
        filler(),
        modal_component_->Render(),
        filler()
      })
    })
  ) | clear_under | center;
}

bool ResetModal::OnEvent(Event event) {
  return ComponentBase::OnEvent(event);
}

bool ResetModal::Focusable() const {
  return ComponentBase::Focusable();
}

} // namespace retui
