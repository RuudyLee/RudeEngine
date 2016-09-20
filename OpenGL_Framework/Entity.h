#pragma once
#include "Texture.h"
#include "Mesh.h"
#include <memory>

class Entity
{
public:
	Entity();
	Entity(std::shared_ptr<Mesh> _mesh, std::shared_ptr<Texture> _texture);
	~Entity();

	void Init(std::shared_ptr<Mesh> _mesh, std::shared_ptr<Texture> _texture);
	void Draw();

private:
	std::shared_ptr<Mesh> _Mesh = nullptr;
	std::shared_ptr<Texture> _Texture = nullptr;
};

