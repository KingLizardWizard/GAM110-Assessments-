#include "stdafx.h"
#include "GameObject.h"
#include "LootObject.h"

GameObject::GameObject()
{
	type = egotBase;
	name = "NONAME";
}


GameObject::~GameObject()
{
	// free up all of the game objects
	for (GameObject* objectPtr : children)
	{
		delete objectPtr;
	}
}

void GameObject::Render(Gdiplus::Graphics& canvas, const CRect& clientRect)
{
	// Save the current transformation of the scene
	Gdiplus::Matrix transform;
	canvas.GetTransform(&transform);

	canvas.TranslateTransform((float)location.X, (float)location.Y);

	///// ALL OF THE RENDERING CODE GOES HERE
	GameFrameworkInstance.DrawRectangle(canvas, AABBi(Vector2i(-5, -5), Vector2i(5, 5)), true, Gdiplus::Color::Bisque);

	// Restore the transformation of the scene
	canvas.SetTransform(&transform);
}

void GameObject::WriteToCSV(std::ofstream& os)
{
	// save out the common game object information
	os << type << "," 
	   << location.X << "," 
	   << location.Y << "," 
	   << name << ","
	   << children.size() 
	   << std::endl;

	// save all of the children
	for (GameObject* objectPtr : children)
	{
		objectPtr->WriteToCSV(os);
	}
}

void GameObject::ReadFromCSV(std::ifstream& is)
{
	char dummyVar;

	// read in the delimeters and the location
	is >> dummyVar
		>> location.X
		>> dummyVar
		>> location.Y
		>> dummyVar;

	// read in the name. getline also consumes the comma
	std::getline(is, name, ',');

	// read in all of the children
	children = ReadObjectsFromCSV(is);
}

GameObjects GameObject::ReadObjectsFromCSV(std::ifstream& is)
{
	GameObjects objects;

	// read in the number of children
	size_t numChildren;
	is >> numChildren;
	
	// iterate up to that number
	for (size_t currentObject = 0; currentObject < numChildren; ++currentObject)
	{
		// read the type of the game object
		int dummyType;
		is >> dummyType;
		GameObjectType type = static_cast<GameObjectType>(dummyType);

		// Create the correct type of game object
		GameObject* objectPtr = nullptr;
		switch (type)
		{
		case egotBase:
			objectPtr = new GameObject();
			break;

		case egotLoot:
			objectPtr = new LootObject();
			break;
		}

		// tell the game object to load it's own damn data
		objectPtr->ReadFromCSV(is);

		// add the object to my list
		objects.push_back(objectPtr);
	}

	return objects;
}

void GameObject::WriteToBinary(std::ofstream& os)
{
	os.write(reinterpret_cast<const char*>(&type), sizeof(type));
	os.write(reinterpret_cast<const char*>(&location), sizeof(location));

	// write out the name
	size_t nameLength = name.length();
	os.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
	os.write(name.c_str(), nameLength);

	// write out the number of children
	size_t numChildren = children.size();
	os.write(reinterpret_cast<const char*>(&numChildren), sizeof(numChildren));

	// save all of the children
	for (GameObject* objectPtr : children)
	{
		objectPtr->WriteToBinary(os);
	}
}

void GameObject::ReadFromBinary(std::ifstream& is)
{
	is.read(reinterpret_cast<char*>(&location), sizeof(location));

	// read in the length of the name
	size_t nameLength = 0;
	is.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));

	// allocate temporary memory for the name
	// we need 1 extra character for the end of string marker
	char* tempBuffer = new char[nameLength + 1];

	// read the name in to the temporary location
	is.read(tempBuffer, nameLength);

	// add in end of string marker
	tempBuffer[nameLength] = '\0';

	// Set the name
	name = tempBuffer;

	// clean up the temporary buffer
	delete tempBuffer;

	// Load the children
	children = ReadObjectsFromBinary(is);
}

GameObjects GameObject::ReadObjectsFromBinary(std::ifstream& is)
{
	GameObjects objects;

	// read in how many game objects there are
	size_t numObjects = 0;
	is.read(reinterpret_cast<char*>(&numObjects), sizeof(numObjects));

	// iterate up to that number
	for (size_t currentObject = 0; currentObject < numObjects; ++currentObject)
	{
		// read the type of the game object
		GameObjectType type = egotNumGameObjectTypes;
		is.read(reinterpret_cast<char*>(&type), sizeof(type));

		// Create the correct type of game object
		GameObject* objectPtr = nullptr;
		switch (type)
		{
		case egotBase:
			objectPtr = new GameObject();
			break;

		case egotLoot:
			objectPtr = new LootObject();
			break;
		}

		// tell the game object to load it's own damn data
		objectPtr->ReadFromBinary(is);

		// add the object to my list
		objects.push_back(objectPtr);
	}

	return objects;
}

