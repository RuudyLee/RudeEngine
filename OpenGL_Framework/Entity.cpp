#include "Entity.h"


Entity::Entity() {
	_Mesh = nullptr;
	_Texture = nullptr;
}

Entity::Entity(std::shared_ptr<Mesh> _mesh, std::shared_ptr<Texture> _texture)
: _Mesh(_mesh)
, _Texture(_texture)
{}

Entity::~Entity() {
}

void Entity::Init(std::shared_ptr<Mesh> _mesh, std::shared_ptr<Texture> _texture) {
	_Mesh = _mesh;
	_Texture = _texture;
}

void Entity::Draw() {
	_Texture->Bind();
	glBindVertexArray(_Mesh->GetVAO());
	glDrawArrays(GL_TRIANGLES, 0, _Mesh->GetNumVertices());
	_Mesh->Unbind();
	_Texture->Unbind();
}
