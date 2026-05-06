#pragma once

#include <string>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>

namespace retui {

inline ftxui::Element FocusedWindow(const std::string& title, ftxui::Element content, bool is_focused) {
  using namespace ftxui;
  Color border_color = is_focused ? Color(Color::Cyan1) : Color(Color::White);
  Decorator title_style = is_focused ? bold : nothing;
  return window(text(title) | title_style, std::move(content)) | color(border_color);
}

} // namespace retui
