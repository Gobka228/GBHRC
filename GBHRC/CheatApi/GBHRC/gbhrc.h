#pragma once
#include <vector>

#include "config/Config.h"
#include "../BrokeProtocol/BrokeProtocol.h"
//#include "configuration/drawconf.h"
#include "../../../GBHApplication/configuration/drawconf.h"
//#include "configuration/drawconf.h"
//#include "Form/Canvas/elements/rectangle/rectangle.h"

namespace Application {
	namespace Canvas {
		class Rectangle;
	}
}

namespace Application {
	namespace Render {
		struct D3D11DrawEvent;
		class Engine;
		class CanvasScene;
	}
}

namespace GBHRC
{

	struct EspBox
	{
		Application::Canvas::Rectangle* box;
		Application::Canvas::Rectangle* max_health_box;
		Application::Canvas::Rectangle* health_box;
	};

	struct EspPlayer
	{
		BrokeProtocol::Players::ShPlayer* player;
		Vector3 top_point;
		Vector3 bottom_point;
		float map_distance;
		float display_distance;
	};
	
	class Context
	{
	private:
		HMODULE DllInst;
		UnityEngine::Transform* aim_target = nullptr;
		
		Context();

		void render_callback(Application::Render::DrawEvent*event);
		void draw_player(Application::Render::DrawEvent* event,UINT element_index,Application::Render::Resolution camera_resolution,EspPlayer*player);
		bool is_aim_target(EspPlayer* old_player, EspPlayer* new_player) const;

		std::vector<EspBox*> esp_boxes;
		Application::Render::CanvasScene* esp_scene;
		std::vector<wchar_t*> friend_list;
	public:
		void set_esp(bool status);
		bool is_friend(wchar_t* nickname);

		void add_friend(wchar_t* nickname);
		void remove_friend(wchar_t* nickname);
		
		Config* config;

		
		bool aim_active = false;

		void implement(HMODULE DllInst);
		static Context* instance();

		void make_esp_boxes();
		void set_esp_scene(Application::Render::CanvasScene*form);

		static void static_draw_callback(Application::Render::DrawEvent*event);
		void life_cycle();
	};

}

