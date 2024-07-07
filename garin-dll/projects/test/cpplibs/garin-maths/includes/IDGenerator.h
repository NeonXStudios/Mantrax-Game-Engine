#pragma once
#include <iostream>
#include <random>
#include <iomanip>

class IDGenerator
{
public:
    static int generate_id()
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(1000000, 2147483);

        return dis(gen);
    }

    static int generate_id_component()
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(1, 1023);

        return dis(gen);
    }
};