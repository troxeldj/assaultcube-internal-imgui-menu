#pragma once
#pragma comment(lib, "OpenGL32.lib")
#include <Windows.h>
#include <stdio.h>
#include <gl\GL.h>
#include "mem.h"
#include "util.hpp"
#include "imgui195/imgui.h"

namespace GL
{
	void SetupOrtho();
	void RestoreGL();

	void DrawFilledRect(float x, float y, float width, float height, ImVec4 color);
	void DrawOutline(float x, float y, float x2, float y2, float lineWidth, ImVec4 color);
	void DrawLine(float startx, float starty, float endx, float endy, float lineWidth, ImVec4 color);

	float getScreenx();
	float getScreenY();

	class Font
	{
	public:
		bool bBuilt = false;
		unsigned int base;
		HDC hdc = nullptr;
		int height;
		int width;

		void Build(int height);
		void Print(float x, float y, ImVec4 color, const char* format, ...);

		Vector3 centerText(float x, float y, float width, float height, float textWidth, float textHeight);
		float centerText(float x, float width, float textWidth);
	};
}