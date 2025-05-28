#include <iostream>
#include <memory>
#include <fstream>
#include <vector>

#include "cpu.hpp"

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <path_to_binary_file>\n";
        return 1;
    }

    // load the program into memory
    auto memory = std::make_shared<lc3::Memory>(argv[1]);

    // Create the CPU instance
    lc3::CPU cpu(memory);

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