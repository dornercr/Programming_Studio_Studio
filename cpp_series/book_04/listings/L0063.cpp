#include "check.hpp"
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <spawn.h>
#include <string_view>
#include <system_error>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char** argv) {
    if (argc == 2 && std::string_view{argv[1]} == "--child") {
        const char* mode = std::getenv("HARBOR_MODE");
        return mode && std::strcmp(mode, "fixture") == 0 ? 7 : 8;
    }
    CHECK(argc == 1);
    char child_flag[] = "--child";
    char environment_value[] = "HARBOR_MODE=fixture";
    char* child_arguments[]{argv[0], child_flag, nullptr};
    char* child_environment[]{environment_value, nullptr};
    pid_t child{};
    const int error = ::posix_spawn(&child, argv[0], nullptr, nullptr, child_arguments, child_environment);
    if (error != 0) throw std::system_error(error, std::generic_category(), "posix_spawn");
    int status{};
    pid_t waited;
    do { waited = ::waitpid(child, &status, 0); } while (waited < 0 && errno == EINTR);
    if (waited < 0) throw std::system_error(errno, std::generic_category(), "waitpid");
    CHECK(waited == child && WIFEXITED(status));
    CHECK(WEXITSTATUS(status) == 7);
    std::cout << "child_exit=" << WEXITSTATUS(status) << "\nPASS\n";
}
