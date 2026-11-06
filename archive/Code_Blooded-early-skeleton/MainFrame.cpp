#include "MainFrame.h"

void MainFrame::init() // Initialize the main frame, load scripts, and setup arena
{
	editor.loadScript("scripts/default.bot");// Load the default bot script from a file
	arena.setup(editor.GetScript());// Setup the arena with the loaded bot script
}

void MainFrame::loop()// Main loop for running the bot battles and rendering the arena
{
	for (int tick = 0; tick < 1000; ++tick) // Run the bot battles for 1000 ticks
	{
		arena.update();// Update the arena state based on the bot actions
		arena.render();// Render the current state of the arena
	}
}