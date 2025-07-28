#include "EditorPanel.h"
# include <fstream>
#include <sstream>

void EditorPanel::loadScript(const std::string& path) // Load script from a file
{
	std::ifstream file(path);// Open the script file
	std::stringstream buffer;// Create a string stream to hold the file content
	buffer << file.rdbuf();// Read the entire file into the string stream
	scriptCode = buffer.str();// Store the content in the scriptCode member variable
}

std::string EditorPanel::GetScript() const // Get the current script code
{
	return scriptCode;// Return the script code stored in the member variable
}