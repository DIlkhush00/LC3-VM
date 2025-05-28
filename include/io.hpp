#pragma once
#include <termios.h>

namespace lc3 {
    class IO {
    public:
        static void disable_input_buffering();
        static void restore_input_buffering();
        static bool check_key();
        static void handle_interrupt(int signal);
    private:
        static struct termios original_tio;
    };
}
