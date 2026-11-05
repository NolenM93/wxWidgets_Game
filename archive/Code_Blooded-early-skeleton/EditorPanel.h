#pragma once
#include <string>

class EditorPanel
{private:
	std::string scriptCode;// Holds the script code for the bot
public:
	void loadScript(const std::string& path);// Load script from a file
	std::string GetScript() const;// Get the current script code
};

