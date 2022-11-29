//
// Created by Vlad on 29.11.2022.
//
#pragma once
#include <vector>
#include "../math/ImVec3.h"
#include <string>

namespace engine
{

	class Object
	{
	 public:
		explicit Object(const std::string& path);
		std::vector<ImVec3> m_vVertexes;
		std::vector<std::vector<size_t>> m_vFaces;

	};

} // engine