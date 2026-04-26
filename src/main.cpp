#include "TuiController.hpp"
#include "RetuiApp.hpp"

#include <ftxui/component/screen_interactive.hpp>

int main() {
  retui::RetuiApp app;
  auto controller = std::make_shared<retui::TuiController>(&app);
  auto screen = ftxui::ScreenInteractive::Fullscreen();
  screen.Loop(controller);
  return 0;
}