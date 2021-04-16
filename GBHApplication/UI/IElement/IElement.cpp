#include "IElement.h"

Application::Render::Position Application::UI::IElement::get_position() const
{
	return this->position;
}

bool Application::UI::IElement::point_belongs(POINT point)
{
	return false;
}

void Application::UI::IElement::set_index(UINT index)
{
	this->index = index;
}

void Application::UI::IElement::move_by(float x, float y)
{
	const auto size = this->size();
	GVertex::Vertex* ptr = this->get_ptr();
	for (UINT i = 0; i < size; i++)
	{
		const auto pos = ptr[i].pos;
		ptr[i].pos = DirectX::XMFLOAT3(pos.x + x, pos.y + y, 1.f);
	}
}

GVertex::Vertex* Application::UI::IElement::get_ptr() const
{
	return this->pScene->get_ptr() + this->index;
}