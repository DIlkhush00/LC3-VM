#include "io.hpp"

#include <unistd.h>
#include <signal.h>
#include <iostream>
#include <stdlib.h>
#include <sys/select.h>

namespace lc3 {
    struct termios IO::original_tio;

    void IO::disable_input_buffering() {
        tcgetattr(STDIN_FILENO, &original_tio);
        struct termios new_tio = original_tio;
        new_tio.c_lflag &= ~ICANON & ~ECHO;
        tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
    }

    void IO::restore_input_buffering() {
        tcsetattr(STDIN_FILENO, TCSANOW, &original_tio);
    }

    bool IO::check_key() {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);

        struct timeval timeout = {0, 0};
        return select(1, &readfds, NULL, NULL, &timeout) != 0;
    }

    void IO::handle_interrupt(int signal) {
        restore_input_buffering();
        std::cout << "\nInterrupted!\n";
        exit(-2);
    }
}
