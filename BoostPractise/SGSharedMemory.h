#pragma once
class SGSharedMemory
{
public:
	int start(int argc, char* argv[]);

private:
	int a_simple_example(int argc, char* argv[]);
	int Native_windows_shared_memory(int argc, char* argv[]);
};