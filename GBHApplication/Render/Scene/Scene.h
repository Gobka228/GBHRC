#pragma once

#include "../d3d/Vertex.h"
#include <vector>
#include <functional>
#include "IRenderObject.h"
#include "../../Application.h"

namespace Application
{
	namespace Render
	{
		class Scene
		{
		private:
			GVertex::VertexBuffer* pVBuffer;
			std::vector<Render::IRenderObject*> pElements;
		protected:
			Render::Resolution resolution;
		public:
			// public variables
			void(__stdcall* render_callback)(Scene* self) = nullptr;
			bool hidden = false;
		public:
			// public void's

			void update(ID3D11DeviceContext*pContext);
			void render(ID3D11DeviceContext* pContext,ID3D11Device* pDevice);

			void foreach(std::function<void(IRenderObject*)> const& nigger);
			UINT total_size();
		protected:
			void alloc_vbuffer(Render::Engine* pEngine);
			void add_render_object(IRenderObject* object);

			IRenderObject* element_at(UINT index);
		public:
			// public setters
			
			void set_resolution(Resolution resolution);
		public:
			// public getters
			
			GVertex::Vertex* get_ptr() const;
		public:
			~Scene();
		};
	}
}
