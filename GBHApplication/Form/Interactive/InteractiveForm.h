#pragma once
#include "../../Application.h"
#include "../../../GBHRC/CheatApi/GBHRC/gbhrc.h"
#include "../../configuration/drawconf.h"
#include "../../Render/Scene/CanvasScene.h"

namespace Application
{

	namespace UI {
		struct InteractiveElementDesc;
		class InteractiveElement;
	}

	class InteractiveForm final : public Render::CanvasScene
	{
	public:
		struct DragStruct
		{
			Render::Position dragged_offset;
			UI::InteractiveElement* element;
		};
	private:
		std::vector<UI::InteractiveElement*> interactive_elements;
		void render_components(Render::DrawEvent* event) override;
		DragStruct* dragged = nullptr;
	public:
		void foreach(std::function<void(UI::InteractiveElement* element)>callback);
		void drag_move(UI::InteractiveElement* element);
		void free_drag_move();

		//void initialize_components() override;

		InteractiveForm* add_element(UI::InteractiveElement* element);
	public:
		InteractiveForm(Render::Engine* pEngine);
		~InteractiveForm();
	public:
		Interaction::EventStatus on_lbmouse_up() ;
		Interaction::EventStatus on_lbmouse_down() ;
		Interaction::EventStatus on_mouse_move(int mx,int my) ;
		Interaction::EventStatus on_mouse_scroll(short direction) ;
	};
}
