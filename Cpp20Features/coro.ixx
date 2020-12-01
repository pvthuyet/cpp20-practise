module;

#include <coroutine>
#include <iostream>
#include <thread>

import CpprefGenerator;

export module fibo.coro;

#define LOGINFO	(std::cout << "[" << std::this_thread::get_id() << "] ")

// create global string generator
auto gStrGenerator = cppref::generator_string(33);
auto gStrGeneratorExper = cppref::generator_string_exper(33);
auto curIt = gStrGeneratorExper.begin();

static int gGenerateIndex = 0;

export void testCpprefGenerator()
{
	auto str1 = gStrGenerator();
	LOGINFO << "Hand write generate\t" << gGenerateIndex << " - " << str1 << std::endl;

	auto str2 = *curIt;
	LOGINFO << "experimental generate\t" << gGenerateIndex << " - " << str2 << std::endl;
	++curIt;
	gGenerateIndex++;
}
