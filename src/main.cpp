#include "TuiController.hpp"
#include "RetuiApp.hpp"

#include <ftxui/component/screen_interactive.hpp>

int main(int argc, char* argv[]) {
  std::string filepath = "retui.json";
  if (argc > 1) {
    filepath = argv[1];
  }
  retui::AppState app_state;
  app_state.Load(filepath);
  retui::RetuiApp app;
  auto controller = std::make_shared<retui::TuiController>(&app, &app_state);
  auto screen = ftxui::ScreenInteractive::Fullscreen();
  screen.Loop(controller);
  app_state.SetMainRegex(controller->GetMainRegexText());
  app_state.SetTestStrings(controller->GetAllTestStrings());
  app_state.Save();
  return 0;
}