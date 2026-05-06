#pragma once

#include <functional>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/elements.hpp>

namespace retui {

using namespace ftxui;

class HelpModal : public ComponentBase {
public:
  explicit HelpModal(std::function<void()> on_close);
  Element OnRender() override;
  bool OnEvent(Event event) override;
  bool Focusable() const override;

private:
  Component close_button_;
};

} // namespace retui
