#ifndef _INTERFACE_H_
#define _INTERFACE_H_

class Gui_manager;

class IGui {
	public:
		IGui(Gui_manager* manager);
		virtual void logic() = 0;
		virtual void render() = 0;
		virtual ~IGui();
	protected:
		Gui_manager* manager;

};

#endif

