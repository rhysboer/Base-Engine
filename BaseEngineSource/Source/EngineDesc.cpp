#include "EngineDesc.h"

namespace BE {
	int EngineDesc::OpenGL_GetVersionMajor() const
	{
		const int major[] = {2, 3, 3, 3, 3, 4, 4, 4, 4};
		return major[(int)OpenGL_version];
	}

	int EngineDesc::OpenGL_GetVersionMinor() const
	{
		const int minor[] = { 1, 0, 1, 2, 3, 0, 1, 2, 3 };
		return minor[(int)OpenGL_version];
	}

	const char* EngineDesc::OpenGL_GetGLSLShaderVersion() const
	{
		const char* glsl[] = { "#version 120", "#version 130", "#version 140", "#version 150", "#version 330", "#version 400", "#version 410", "#version 420", "#version 430" };
		return glsl[(int)OpenGL_version];
	}
}