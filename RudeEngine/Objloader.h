#pragma once
#include <glm\glm.hpp>

bool loadOBJ(
	const char * path,
	std::vector<glm::vec3> &out_vertices,
	std::vector<glm::vec2> &out_uvs,
	std::vector<glm::vec3> &out_normals,
	glm::vec2 &xCons,
	glm::vec2 &yCons,
	glm::vec2 &zCons
	);