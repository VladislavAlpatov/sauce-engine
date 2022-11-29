//
// Created by Vlad on 29.11.2022.
//
#pragma once
#include "object.h"
#include <Windows.h>
#include <memory>


std::vector<std::string> split(const std::string& str, char dev)
{
	std::vector<std::string> out;
	size_t dwTmpSize = 0;
	for (size_t i = 0; i < str.size() + 1; ++i)
	{
		if (str[i] != dev and str[i] != NULL)
		{
			dwTmpSize++;
			continue;
		}

		if (!dwTmpSize) continue;


		auto line = std::string(dwTmpSize, NULL);

		memcpy_s(line.data(), line.size(), &str[i - dwTmpSize], dwTmpSize);
		out.push_back(line);
		dwTmpSize = 0;
	}
	return out;
}

namespace engine
{
	Object::Object(const std::string& path)
	{
		auto hFile = CreateFileA(path.c_str(), GENERIC_READ | GENERIC_WRITE, NULL, NULL, OPEN_ALWAYS, NULL, NULL);
		const auto dwFileSize = GetFileSize(hFile, NULL);
		auto data = std::unique_ptr<char>(new char[dwFileSize+1]);
		ReadFile(hFile, data.get(), dwFileSize, NULL, NULL);
		data.get()[dwFileSize] = '\x0';

		CloseHandle(hFile);
		for (const auto& line : split(data.get(), '\n'))
		{
			auto line_data = split(line, ' ');

			if (line_data[0] == "v")
				m_vVertexes.emplace_back(std::stof(line_data[1]), std::stof(line_data[2]), std::stof(line_data[3]));

			else if (line_data[0] == "f")
			{
				std::vector<size_t> face;
				for (size_t i = 1; i < line_data.size(); ++i)
					face.push_back(std::stoul(line_data[i])-1);
				m_vFaces.push_back(face);
			}
		}
	}
} // engine