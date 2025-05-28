#include "memory.hpp"
#include <stdexcept>
#include <algorithm>
#include <iostream>

namespace lc3
{
    Memory::Memory(uint16_t *initial, size_t initial_size)
    {

        if (initial_size > get_size())
            memory = new uint16_t[initial_size];

        std::copy(initial, initial + initial_size, memory);
    }

    Memory::Memory(const char *filePath)
    {
        // Load memory from a binary file
        if (!read_image(filePath))
        {
            printf("failed to load image: %s\n", filePath);
            exit(1);
        }
    }

    Memory::~Memory()
    {
        delete[] memory;
    }

    uint16_t Memory::read(uint16_t address) const
    {
        if (address < get_size())
        {
            return memory[address];
        }

        throw std::out_of_range("Memory read out of bounds");
    }

    void Memory::write(uint16_t address, uint16_t value)
    {
        if (address < get_size())
        {
            memory[address] = value;
            return;
        }
        throw std::out_of_range("Memory write out of bounds");
    }

    uint16_t Memory::get_size() const noexcept
    {
        return UINT16_MAX;
    }

    int Memory::read_image(const char *image_path)
    {
        FILE *file = fopen(image_path, "rb");
        if (!file)
        {
            return 0;
        };
        read_image_file(file);
        fclose(file);
        return 1;
    }

    void Memory::read_image_file(FILE *file)
    {
        /* the origin tells us where in memory to place the image */
        uint16_t origin;
        fread(&origin, sizeof(origin), 1, file);

        /* we know the maximum file size so we only need one fread */
        uint16_t max_read = get_size() - origin;
        uint16_t *p = memory + origin;
        size_t read = fread(p, sizeof(uint16_t), max_read, file);

        std::cout << "memory : " << memory << std::endl;
    }
}