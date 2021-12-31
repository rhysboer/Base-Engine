#include "Uniform.h"
#include <assert.h>

namespace BE {

	// -----------------------------------------------------------
#pragma region Base Uniform

	Uniform::Uniform(const UniformType& uniformType, const char* uniformName, const unsigned int& arraySize)
		: type(uniformType), name(uniformName), arraySize(arraySize) {
	}

	Uniform* Uniform::CreateUniform(const UniformType& uniformType, const char* uniformName, const unsigned int& arraySize) {
		switch(uniformType) {
			case BE::UniformType::FLOAT: return new UniformFloat(uniformType, uniformName, 1);
			case BE::UniformType::INT: return new UniformInt(uniformType, uniformName, 1);
			case BE::UniformType::VECTOR_2: return new UniformFloat(uniformType, uniformName, 2);
			case BE::UniformType::VECTOR_3: return new UniformFloat(uniformType, uniformName, 3);
			case BE::UniformType::VECTOR_4: return new UniformFloat(uniformType, uniformName, 4);
			case BE::UniformType::MATRIX_3: return new UniformFloat(uniformType, uniformName, 9);
			case BE::UniformType::MATRIX_4: return new UniformFloat(uniformType, uniformName, 16);
			case BE::UniformType::TEXTURE_2D: return new UniformInt(uniformType, uniformName, 1);
		}

		assert(false && "Uniform: Unsupported uniform type!");
		return nullptr;
	}

#pragma endregion
	// -----------------------------------------------------------

	// -----------------------------------------------------------
#pragma region Int Uniform
	UniformInt::UniformInt(const UniformType& uniformType, const char* uniformName, const unsigned int& arraySize) : Uniform(uniformType, uniformName, arraySize) {
		data = new int[arraySize];
		for(int i = 0; i < arraySize; i++)
			data[i] = 0; // Default value
	}

	void UniformInt::SetValue(const void* value, const unsigned int& size) {
		assert(size <= arraySize);

		for(int i = 0; i < size; i++)
			data[i] = ((int*)value)[i];
	}

	void* UniformInt::GetValue(const unsigned int& index) const {
		assert(index < arraySize);
		return (void*)data[index];
	}
#pragma endregion
	// -----------------------------------------------------------

	// -----------------------------------------------------------
#pragma region Float Uniform
	UniformFloat::UniformFloat(const UniformType& uniformType, const char* uniformName, const unsigned int& arraySize) : Uniform(uniformType, uniformName, arraySize) {
		data = new float[arraySize];
		for(int i = 0; i < arraySize; i++)
			data[i] = 0.0f; // Default value
	}

	void UniformFloat::SetValue(const void* value, const unsigned int& size) {
		assert(size <= arraySize);

		for(int i = 0; i < size; i++)
			data[i] = ((float*)value)[i];
	}

	void* UniformFloat::GetValue(const unsigned int& index) const {
		assert(index < arraySize);
		return (void*)&data[index];
	}
#pragma endregion
	// -----------------------------------------------------------
}