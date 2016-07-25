#include "stdafx.h"
#include "AppConfig.h"

AppConfig::AppConfig()
{
	// Retrieve the executable's file path
	TCHAR executableFilePath[_MAX_PATH];
	VERIFY(::GetModuleFileName(AfxGetApp()->m_hInstance, executableFilePath, _MAX_PATH));

	// Convert to a CString
	CString filePath(executableFilePath);

	int lastPathSeparator;

	// Strip everything after the last path separator
	lastPathSeparator = filePath.ReverseFind(_T('\\'));
	if (lastPathSeparator > 0) {
		filePath = filePath.Left(lastPathSeparator);

		// Run the strip again to get our base path
		lastPathSeparator = filePath.ReverseFind(_T('\\'));
		if (lastPathSeparator > 0) {
			filePath = filePath.Left(lastPathSeparator);

			filePath += _T("\\");
		}
	}

	// Store our base data path
	baseDataPath = filePath;

	// Load the config file
	tinyxml2::XMLDocument xmlConfig;
	xmlConfig.LoadFile(baseDataPath + "Data\\Config.xml");

	// Parse the configuration into our map
	tinyxml2::XMLElement* configElement = xmlConfig.FirstChildElement("Config");
	tinyxml2::XMLNode* setting = configElement->FirstChild();
	do 
	{
		std::string id = setting->ToElement()->Attribute("id");
		std::string value = setting->ToElement()->Attribute("value");

		configEntries[id] = value;
	} while (setting = setting->NextSibling());
}

AppConfig::~AppConfig()
{
}

AppConfig& AppConfig::Instance()
{
	static AppConfig instance;

	return instance;
}

std::string AppConfig::GetResourcePath(const std::string& relativePath) const
{
	std::string resourcePath = CW2A(BaseDataPath());
	resourcePath += std::string("Data\\") + relativePath;

	return resourcePath;
}
