add_library(mathlib src/add.cpp)
target_include_directories(mathlib PUBLIC include)
target_compile_features(mathlib PUBLIC cxx_std_20)

add_executable(calculator app/main.cpp)
target_link_libraries(calculator PRIVATE mathlib)
