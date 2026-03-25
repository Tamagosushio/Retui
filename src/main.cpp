#include "TuiController.hpp"
#include "RegexMatcher.hpp"

int main() {
  retui::RegexMatcher regex_matcher;
  regex_matcher.Compile("^https?://([^/]+)(?:/([^?]*))?(?:\\?(.*))?$");
  regex_matcher.Execute("https://www.google.com");
  regex_matcher.DebugPrint();
  regex_matcher.Execute("https://www.google.com/search");
  regex_matcher.DebugPrint();
  regex_matcher.Execute("https://www.google.com/search?q=test");
  regex_matcher.DebugPrint();
}