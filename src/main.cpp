#include "TuiController.hpp"
#include "RetuiApp.hpp"

#include <ftxui/component/screen_interactive.hpp>

int main(int argc, char* argv[]) {
  std::string filepath = "retui.json";
  if (argc > 1) {
    filepath = argv[1];
  }
  retui::RetuiApp app;
  app.LoadState(filepath);
  auto controller = std::make_shared<retui::TuiController>(&app);
  auto screen = ftxui::ScreenInteractive::Fullscreen();
  screen.Loop(controller);
  app.SaveState(controller->GetMainRegexText(), controller->GetAllTestStrings());
  return 0;
}