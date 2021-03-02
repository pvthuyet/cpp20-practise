#pragma once
class SynchronizationMechanisms
{
public:
	int start(int argc, char* argv[]);

private:
	int Anonymous_mutex_example(int argc, char* argv[]);
	int Named_mutex_example(int argc, char* argv[]);
	int Anonymous_condition_example(int argc, char* argv[]);
	int Anonymous_semaphore_example(int argc, char* argv[]);
	int Using_a_message_queue(int argc, char* argv[]);
};

