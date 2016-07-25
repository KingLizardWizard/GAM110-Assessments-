#pragma once

#include <string>
#include <map>
#include <sstream>

class AppConfig
{
public:
	static AppConfig& Instance();

	/**
	 * Retrieves the value of an item stored in the configuration file (Output/Data/Config.xml).
	 * 
	 * You can indicate the type of the value being retrieved as well, eg:
	 *   AppConfigInstance.GetConfig<int>("key name");   // Retrieve key name as an integer
	 *   AppConfigInstance.GetConfig<float>("key name"); // Retrieve key name as a float
	 *
	 * @param key The key to lookup in the configuration file.
	 *
	 * @return The value for the requested key.
	 */
	template <typename T>
	T GetConfig(const std::string& key)
	{
		// Use a string stream to perform a lexical cast on the data
		std::istringstream stream(configEntries[key]);
		T value;
		stream >> value;

		return value;
	}

	/**
	 * Gets the full path to a resource file that is located in the Output/Data folder.
	 * 
	 * eg. AppConfigInstance.GetResourcePath("Images/TestImage.png"); 
	 * This retrieves the file TestImage.png from the Images folder in the Output/Data folder
	 *
	 * @param relativePath The path to the file (relative to the Output/Data folder).
	 *
	 * @return The full path to the resource.
	 */
	std::string GetResourcePath(const std::string& relativePath) const;

protected:
	AppConfig();
	~AppConfig();

	const CString& BaseDataPath() const { return baseDataPath; }

	CString baseDataPath;
	std::map<std::string, std::string> configEntries;
};

/**
 * Retrieves the Application Config Instance for the game.
 *
 * @return The application configuration instance
 */
#define AppConfigInstance (AppConfig::Instance())
