#pragma once
#include <string>
#include "glm/glm.hpp"

namespace BE {
	enum class UniformType {
		FLOAT = 0x1406,
		INT = 0x1404,
		VECTOR_2 = 0x8B50,
		VECTOR_3 = 0x8B51,
		VECTOR_4 = 0x8B52,
		MATRIX_2 = 0x8B5A,
		MATRIX_3 = 0x8B5B,
		MATRIX_4 = 0x8B5C,
		TEXTURE_2D = 0x8B5E,
	};

	// TODO:
	// Handle Arrays in uniforms
	// Handle deletion of data in destructors


	class Shader;
	class Material;

	// ------------ Base Uniform
	class Uniform {
	public:

		Uniform(const UniformType& uniformType, const char* uniformName, const unsigned int& arraySize = 1);
		~Uniform() {};

		virtual void SetValue(const void* value, const unsigned int& size = 1) = 0;
		virtual void* GetValue(const unsigned int& index = 0) const = 0;

		inline UniformType GetType() const { return type; }
		inline const char* GetName() const { return name.c_str(); }

		static Uniform* CreateUniform(const UniformType& uniformType, const char* uniformName, const unsigned int& arraySize = 1);

	protected:

		UniformType type;
		std::string name;
		unsigned int arraySize;
	};

	// ------------ Int Uniform
	class UniformInt : public Uniform {
	public:

		UniformInt(const UniformType& uniformType, const char* uniformName, const unsigned int& arraySize = 1);
		~UniformInt() { /* TODO */ };

		virtual void SetValue(const void* value, const unsigned int& size = 1) override;
		virtual void* GetValue(const unsigned int& index = 0) const override;

	private:

		int* data;
	};

	// ------------ Float Uniform
	class UniformFloat : public Uniform {
	public:

		UniformFloat(const UniformType& uniformType, const char* uniformName, const unsigned int& arraySize = 1);
		~UniformFloat() { /* TODO */ };

		virtual void SetValue(const void* value, const unsigned int& size = 1) override;
		virtual void* GetValue(const unsigned int& index = 0) const override;

	private:

		float* data;
	};
}

