#pragma once

#include <vector>
#include "Catalog.h"

class Manager
{
public:
	bool load();
	const std::vector<Catalog>& getCatalogs() const;

private:
	std::vector<Catalog> mCatalogs;
};

