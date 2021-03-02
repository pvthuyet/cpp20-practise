#pragma once
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/interprocess_condition.hpp>
#include <boost/interprocess/sync/interprocess_semaphore.hpp>

struct shared_memory_log
{
	enum { NumItems = 100 };
	enum { LineSize = 100 };

	shared_memory_log()
		: current_line(0)
		, end_a(false)
		, end_b(false)
	{}

	//Mutex to protect access to the queue
	boost::interprocess::interprocess_mutex mutex;

	//Items to fill
	char   items[NumItems][LineSize];
	int    current_line;
	bool   end_a;
	bool   end_b;
};

struct trace_queue
{
	enum {LineSize = 100};

	//Mutex to protect access to the queue
	boost::interprocess::interprocess_mutex mutex;

	//Condition to wait when the queue is empty
	boost::interprocess::interprocess_condition cond_empty;

	//Condition to wait when the queue is full
	boost::interprocess::interprocess_condition cond_full;

	// Items to fill
	char items[LineSize];

	// Is there any message
	bool message_in;
};

struct shared_memory_buffer
{
	enum { NumItems = 10 };
	shared_memory_buffer()
		: mutex(1), nempty(NumItems), nstored(0)
	{}

	//Semaphores to protect and synchronize access
	boost::interprocess::interprocess_semaphore mutex, nempty, nstored;

	//Items to fill
	int items[NumItems] = {};
};