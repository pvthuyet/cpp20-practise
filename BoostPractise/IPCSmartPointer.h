#pragma once
class IPCSmartPointer
{
public:
	int intrusivePointer();
	int scopedPointer();
	int sharedPointer();
	int sharedWeakPointer();
	int uniquePointer();
};

