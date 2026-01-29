#include "interface.h"

IGui::IGui(Gui_manager* manager) {
	this->manager = manager;
}

IGui::~IGui() {
	manager = nullptr;
}
