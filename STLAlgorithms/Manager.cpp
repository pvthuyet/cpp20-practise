#include "pch.h"
#include "Manager.h"
#include "Catalog.h"
#include "fmt/format.h"
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include "NoneModifyingSequence.h"

using json = nlohmann::json;

void from_json(const nlohmann::json& j, Catalog& cat)
{
	j.at("id").get_to(cat.id);
	j.at("catalog").get_to(cat.catalog);
	j.at("operations").get_to(cat.operations);
}

bool Manager::load()
{
	try {
		std::ifstream ifs("algorithms.json");
		auto jf = json::parse(ifs);
		auto jsAl = jf["algorithms"];
		for (auto& item : jsAl) {
			mCatalogs.push_back(item.get<Catalog>());
		}
	}
	catch (const std::exception& ex)
	{
		fmt::print(ex.what());
	}
	return false;
}

const std::vector<Catalog>& Manager::getCatalogs() const
{
	return mCatalogs;
}

const std::vector<std::string>& Manager::getOperations(int index) const
{
	for (const auto& el : mCatalogs) {
		if (el.id == index) {
			return el.operations;
		}
	}
	static std::vector<std::string> dummy{};
	return dummy;
}

void Manager::name(int catalogIndex)
{
	switch (catalogIndex)
	{
	case NON_MODIFYING_SEQUENCE_OPERATIONS:
		NoneModifyingSequence::name();
		break;

	default:
		break;
	}
}

void Manager::show(int catalogIndex, int operationIndex) const
{
	switch (catalogIndex)
	{
	case NON_MODIFYING_SEQUENCE_OPERATIONS:
		NoneModifyingSequence{}.show(operationIndex);
		break;

	default:
		break;
	}
}