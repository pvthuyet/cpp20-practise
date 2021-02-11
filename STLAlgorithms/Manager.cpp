#include "Manager.h"
#include <iostream>
#include <charconv>
#include "NoneModifyingSequence.h"

Manager::Manager()
{
    using enum Catalogs;
    mCatalogs[NON_MODIFYING_SEQUENCE_OPERATIONS]            = "Non-modifying sequence operations";
    mCatalogs[MODIFYING_SEQUENCE_OPERATIONS]                = "Modifying sequence operations";
    mCatalogs[PARTITIONING_OPERATIONS]                      = "Partitioning operations";
    mCatalogs[SORTING_OPERATIONS]                           = "Sorting operations";
    mCatalogs[BINARY_SEARCH_OPERATIONS_ON_SORTED_RANGES]    = "Binary search operations (on sorted ranges)";
    mCatalogs[OTHER_OPERATIONS_ON_SORTED_RANGES]            = "Other operations on sorted ranges";
    mCatalogs[SET_OPERATIONS_ON_SORTED_RANGES]              = "Set operations (on sorted ranges)";
    mCatalogs[HEAP_OPERATIONS]                              = "Heap operations";
    mCatalogs[MINIMUM_MAXIMUM_OPERATIONS]                   = "Minimum/maximum operations";
    mCatalogs[COMPARISON_OPERATIONS]                        = "Comparison operations";
    mCatalogs[PERMUTATION_OPERATIONS]                       = "Permutation operations";
    mCatalogs[NUMERIC_OPERATIONS]                           = "Numeric operations";
    mCatalogs[OPERATIONS_ON_UNINITIALIZED_MEMORY]           = "Operations on uninitialized memory";
}

void Manager::start() const
{
    printCatalogs();
    std::cout << '\n';
    while (true) {
        std::string cat;
        std::cout << "Choose Catalog number: ";
        std::cin >> cat;
        int catNumb{};
        if (auto [p, ec] = std::from_chars(cat.data(), cat.data() + cat.length(), catNumb);
            std::errc{} != ec) {
            throw std::runtime_error("Catalog number must be a integer");
        }

        switch (catNumb) {
            using enum Catalogs;
        case NON_MODIFYING_SEQUENCE_OPERATIONS:
            NoneModifyingSequence{}.start();
            break;

        default:
            break;
        }
    }
}

void Manager::printCatalogs() const
{
    std::cout << "Alorithm Catalogs\n";
    std::cout
        << "\t0 - Non-modifying sequence operations\n"
        << "\t1 - Modifying sequence operations\n"
        << "\t2 - Partitioning operations\n"
        << "\t3 - Sorting operations\n"
        << "\t4 - Binary search operations (on sorted ranges)\n"
        << "\t5 - Other operations on sorted ranges\n"
        << "\t6 - Set operations (on sorted ranges)\n"
        << "\t7 - Heap operations\n"
        << "\t8 - Minimum/maximum operations\n"
        << "\t9 - Comparison operations\n"
        << "\t10 - Permutation operations\n"
        << "\t11 - Numeric operations\n"
        << "\t12 - Operations on uninitialized memory\n";
}