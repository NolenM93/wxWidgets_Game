#include "MainFrame.h"

void MainFrame::init() {
	editor.loadScript("scripts/default.bot");
	arena.setup(editor.getScript());
}

void MainFrame::loop() {
	for (int tick = 0; tick < 1000; ++tick) {
		arena.update();
		arena.render();
	}
}