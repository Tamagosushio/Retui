#include "TuiController.hpp"

int main() {
  std::shared_ptr<retui::TuiController> app = std::make_shared<retui::TuiController>();
  auto screen = ftxui::ScreenInteractive::Fullscreen();
  screen.Loop(app);
}