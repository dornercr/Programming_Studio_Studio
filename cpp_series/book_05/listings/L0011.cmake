add_library(core src/core.cpp)
target_include_directories(core PUBLIC include PRIVATE src)
target_compile_definitions(core PRIVATE CORE_BUILD=1)

add_executable(app app/main.cpp)
target_link_libraries(app PRIVATE core)
