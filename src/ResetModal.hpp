#pragma once

#include <functional>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/elements.hpp>

namespace retui {

using namespace ftxui;

class ResetModal : public ComponentBase {
public:
  explicit ResetModal(std::function<void()> on_confirm, std::function<void()> on_cancel);
  Element OnRender() override;
  bool OnEvent(Event event) override;
  bool Focusable() const override;

private:
  Component modal_component_;
};

} // namespace retui
