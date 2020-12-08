module;

#include <coroutine>
#include <iostream>
#include <thread>
#include <atomic>
#include <future>

import CpprefGenerator;

export module fibo.coro;

#define LOGINFO	(std::cout << "[" << std::this_thread::get_id() << "] ")

// create global string generator

export void testCpprefGenerator()
{
	std::atomic_int gGenerateIndex = 0;
	auto gStrGenerator = cppref::generator_string(33);
	auto gStrGeneratorExper = cppref::generator_string_exper(33);
	auto curIt = gStrGeneratorExper.begin();

	auto lb = [&](int loops) {
		for(int i = 0; i < loops; ++i) {
			auto s1 = gStrGenerator();
			LOGINFO << "Hand write generate\t" << gGenerateIndex << " - " << s1 << std::endl << std::flush;

			auto s2 = *curIt;
			LOGINFO << "Experimental generate\t" << gGenerateIndex << " - " << s2 << std::endl << std::flush;
			++curIt;

			gGenerateIndex++;
			std::this_thread::sleep_for(std::chrono::milliseconds(3000));
		}
	};

	auto fut1 = std::async(std::launch::async, lb, 2);
	auto fut2 = std::async(std::launch::async, lb, 3);
	fut1.wait();
	fut2.wait();
}
