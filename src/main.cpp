#include <iostream>
#include "cpu.hpp"

int main()
{
    std::cout << "LC-3 Emulator" << std::endl;

    // Initialize CPU and memory
    lc3::Memory memory;

    // Load a program into memory (not implemented here)
    // ...

    lc3::CPU cpu(std::make_shared<lc3::Memory>(memory));
    cpu.setup();

    // Run the CPU
    try
    {
        cpu.run();
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}