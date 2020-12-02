module;

#include <coroutine>
#include <iostream>
#include <thread>

import CpprefGenerator;

export module fibo.coro;

#define LOGINFO	(std::cout << "[" << std::this_thread::get_id() << "] ")

// create global string generator

static int gGenerateIndex = 0;

export void testCpprefGenerator()
{
	auto gStrGenerator = cppref::generator_string(33);
	auto gStrGeneratorExper = cppref::generator_string_exper(33);
	auto curIt = gStrGeneratorExper.begin();

	while (true) {
		auto s1 = gStrGenerator();
		LOGINFO << "Hand write generate\t" << gGenerateIndex << " - " << s1 << std::endl << std::flush;

		auto s2 = *curIt;
		LOGINFO << "Experimental generate\t" << gGenerateIndex << " - " << s2 << std::endl << std::flush;
		++curIt;

		gGenerateIndex++;
		std::cout << "\tContinue y/n: ";
		char c;
		std::cin >> c;
		if (c != 'y') break;
	}
}
