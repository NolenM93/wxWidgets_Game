#include "App.h"

void App::run() {
	MainFrame frame;// Create the main frame for the application
	frame.init();// Initialize the main frame, load scripts, and setup arena
	frame.loop();// Main loop for running the bot battles and rendering the arena
}