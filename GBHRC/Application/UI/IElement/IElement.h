#pragma once
#include "../../Application.h"


namespace Application
{
	namespace UI {
		
		class IElement
		{
		public:
			virtual ~IElement() = default;
			typedef IElement UIElementEventArgs;
			typedef void(*EventCallback)(UIElementEventArgs* args);
		protected:
			Application::Render::Scene* pScene = nullptr;
			UINT index = 0;
			Render::Position position{ 0,0,0,0 };
			static void default_event_callback(UIElementEventArgs* args) {};
		public:
			bool hovered = false;

			Render::Position get_position() const;

			EventCallback onMouseEnter = default_event_callback;
			EventCallback onMouseLeave = default_event_callback;
			EventCallback onMouseMove = default_event_callback;
			EventCallback onMouseClick = default_event_callback;

			virtual void __draw(ID3D11DeviceContext* pContext) = 0;
			virtual UI::IElement* set_pos(float x, float y) = 0;
			virtual UI::IElement* set_color(float r, float g, float b) = 0;
			virtual void init(Render::Scene* pScene) = 0;

			virtual UINT size() = 0;
			void set_index(UINT index);
			void move_by(float x, float y);
			GVertex::Vertex* get_ptr() const;
		};
	}
}
