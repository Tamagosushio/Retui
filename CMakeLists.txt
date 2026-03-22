cmake_minimum_required(VERSION 3.14)
project(Retui CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

# FTXUI
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG        v6.1.9
)
FetchContent_MakeAvailable(ftxui)

# abseil
set(ABSL_PROPAGATE_CXX_STD ON CACHE BOOL "" FORCE)
set(ABSL_ENABLE_INSTALL ON CACHE BOOL "" FORCE)
FetchContent_Declare(absl
  GIT_REPOSITORY https://github.com/abseil/abseil-cpp.git
  GIT_TAG        20260107.1
)
FetchContent_MakeAvailable(absl)

# rE2
set(RE2_BUILD_TESTING OFF CACHE BOOL "" FORCE)
FetchContent_Declare(re2
  GIT_REPOSITORY https://github.com/google/re2.git
  GIT_TAG        2025-11-05
)
FetchContent_MakeAvailable(re2)

# nlohmann/json
FetchContent_Declare(json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG        v3.12.0
)
FetchContent_MakeAvailable(json)

# 実行可能ファイル
add_executable(retui src/main.cpp)

# ライブラリのリンク
target_link_libraries(retui
  PRIVATE
    ftxui::screen
    ftxui::dom
    ftxui::component
    re2::re2
    nlohmann_json::nlohmann_json
)
