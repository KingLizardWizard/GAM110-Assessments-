#include "stdafx.h"
#include "LootObject.h"


LootObject::LootObject()
{
	type = egotLoot;
	name = "NONAME-LOOT";
}


LootObject::~LootObject()
{
}

void LootObject::WriteToCSV(std::ofstream& os)
{
	// save the base data first
	GameObject::WriteToCSV(os);

	// save our own data
}

void LootObject::WriteToBinary(std::ofstream& os)
{
	// save the base data first
	GameObject::WriteToBinary(os);

	// save our own data here
}

void LootObject::ReadFromBinary(std::ifstream& is)
{
	// load the base data first
	GameObject::ReadFromBinary(is);

	// load our own data here
}
