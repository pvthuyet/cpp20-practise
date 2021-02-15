#pragma once
#include <string>
#include <vector>

enum CatalogIndex : int
{
	NON_MODIFYING_SEQUENCE_OPERATIONS = 0,
	MODIFYING_SEQUENCE_OPERATIONS,
	END
};

class Catalog
{
public:
	int id;
	std::string catalog;
	std::vector<std::string> operations;
};

