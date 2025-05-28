#include <iostream>
#include <memory>
#include <fstream>
#include <vector>

#include "cpu.hpp"


void print(std::vector<uint16_t> vec);

std::vector<uint16_t> loadBinary(std::string filename)
{
	std::ifstream input(filename, std::ios::binary);

	std::vector<char> bytes(
		std::istreambuf_iterator<char>(input),
		(std::istreambuf_iterator<char>())
		);
	input.close();

	std::vector<uint16_t> result;
    result.reserve(bytes.size() / 2);

	for (size_t i = 0; i < bytes.size() - 1; i += 2)
		result.push_back((bytes[i + 1] << 8) | bytes[i]);

    
    // print(result);

	return result;
}

void print(std::vector<uint16_t> vec) 
{
    for(auto& x: vec)
        std::cout << x << std::endl;
}

int main(int argc, char** argv)
{
    if (argc < 2) 
    {
        std::cerr << "Usage: " << argv[0] << " <path_to_binary_file>\n";
		return 1;
	}

    // load the file
    // for (int j = 1; j < argc; ++j)
    // {
    //     if (!read_image(argv[j]))
    //     {
    //         printf("failed to load image: %s\n", argv[j]);
    //         exit(1);
    //     }
    // }
	// std::vector<uint16_t> program;
	// try 
    // {
	// 	program = loadBinary(argv[1]);
	// } 
    // catch(const std::ifstream::failure&) 
    // {
    //     std::cerr << "Error: Could not open or read the file '" << argv[1] << "'." << std::endl;
	// 	return 1;
	// }

    // load the program into memory
    // auto memory = std::make_shared<lc3::Memory>(program.data(), program.size());
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