// STLAlgorithms.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

int main()
{
    std::cout << "Catalogs\n";
    std::cout
        << "\t1 - Non-modifying sequence operations\n"
        << "\t2 - Modifying sequence operations\n"
        << "\t3 - Partitioning operations\n"
        << "\t4 - Sorting operations\n"
        << "\t5 - Binary search operations (on sorted ranges)\n"
        << "\t6 - Other operations on sorted ranges\n"
        << "\t7 - Set operations (on sorted ranges)\n"
        << "\t8 - Heap operations\n"
        << "\t9 - Minimum/maximum operations\n"
        << "\t10 - Comparison operations\n"
        << "\t11 - Permutation operations\n"
        << "\t12 - Numeric operations\n"
        << "\t13 - Operations on uninitialized memory\n";
    std::cout << "\nChoose catalog: ";
    std::string cat;
    std::cin >> cat;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
