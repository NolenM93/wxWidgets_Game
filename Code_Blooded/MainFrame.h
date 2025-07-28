#pragma once
#include "EditorPanel.h"
#include "ArenaPanel.h"

class MainFrame
{private:
	
	EditorPanel editor;// Editor for bot scripts
	ArenaPaneel arena;// Arena for bot battles

public:
	
	void init();// Initialize the main frame, load scripts, and setup arena
	void loop();// Main loop for running the bot battles and rendering the arena
};

