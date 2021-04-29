#pragma once
#include "../IElement/IElement.h"

namespace Application
{
	namespace UI
	{
		
		class Panel final : public UI::IElement
		{
		private:
			Render::Color color;
			Render::Resolution resolution;
		private:
			void __draw(ID3D11DeviceContext* pContext) override;
		public:
			UINT size() override
			{
				return 5;
			}
			Panel(Render::Position position,Render::Resolution resolution, Render::Color color);
			
			bool point_belongs(POINT point) override;
			void init(Application::InteractiveForm* pForm) override;
			void set_pos(float x, float y) override;
			void set_color(float r, float g, float b) override;
			UI::IElement* set_rect(float width, float height);
		};
	}

}