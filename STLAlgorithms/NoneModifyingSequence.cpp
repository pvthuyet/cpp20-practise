#include "NoneModifyingSequence.h"
#include <iostream>
#include <charconv>
#include <exception>

void NoneModifyingSequence::start() const
{
    try {
        printList();
        while (true) {
            std::cout << '\n';
            while (true) {
                std::string cat;
                std::cout << "Choose algorithm: ";
                std::cin >> cat;
                int catNumb{};
                if (auto [p, ec] = std::from_chars(cat.data(), cat.data() + cat.length(), catNumb);
                    std::errc{} != ec) {
                    throw std::runtime_error("Algorithm number must be a integer");
                }

                switch (catNumb) {
                case 0:
                    break;

                default:
                    break;
                }
            }
        }
    }
    catch (std::exception const& ex) {
        std::cout << ex.what() << std::endl;
    }
}

void NoneModifyingSequence::printList() const
{
    std::cout << "Welcome to 'Non-modifying sequence operations'\n";
    std::cout
        << "\t0 - all_of (C++11)\n"
        << "\t1 - any_of (C++11)\n"
        << "\t2 - none_of (C++11)\n";
}
