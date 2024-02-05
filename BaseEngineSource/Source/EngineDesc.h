#pragma once

namespace BE {
	enum class GraphicsAPI
	{
		OpenGL
	};

	enum class GLVersion {
		GL2_1,
		GL3_0,
		GL3_1,
		GL3_2,
		GL3_3,
		GL4_0,
		GL4_1,
		GL4_2,
		GL4_3
	};

	class EngineDesc
	{
	public:

		const char* name = "Base Engine Project";
		int width = 1280;
		int height = 720;
		GraphicsAPI graphicsAPI = GraphicsAPI::OpenGL;

		// OpenGL
		GLVersion OpenGL_version = GLVersion::GL3_3;

		int OpenGL_GetVersionMajor() const;
		int OpenGL_GetVersionMinor() const;
		const char* OpenGL_GetGLSLShaderVersion() const;
	};
}

