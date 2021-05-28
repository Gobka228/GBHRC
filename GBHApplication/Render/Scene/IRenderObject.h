#pragma once
#include "../d3d/Vertex.h"
#include "../Events/EngineEvents.h"

namespace Application
{
	namespace Render
	{
		class Engine;

		class IRenderObject
		{
		public:
			virtual ~IRenderObject() = default;
			virtual void draw(Render::DrawEvent*event)         PURE;
			virtual void set_pos(float x, float y)             PURE;
			virtual void set_color(float r, float g, float b)  PURE;

			virtual void move_by(float x, float y)             PURE;
		};
	}
}
