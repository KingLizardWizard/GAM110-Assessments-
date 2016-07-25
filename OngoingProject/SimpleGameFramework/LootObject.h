#pragma once
#include "GameObject.h"
class LootObject :
	public GameObject
{
public:
	LootObject();
	virtual ~LootObject();

	virtual void WriteToCSV(std::ofstream& os);

	virtual void WriteToBinary(std::ofstream& os);
	virtual void ReadFromBinary(std::ifstream& is);
};

