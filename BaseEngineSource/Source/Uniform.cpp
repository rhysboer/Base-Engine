#include "Uniform.h"
#include "Logging.h"
#include "glad/glad.h"
#include "ITexture.h"
#include "BEGlobal.h"

#define BE_NORMAL_PREFIX "norm"

namespace BE {

	// -----------------------------------------------------------
#pragma region Base_Uniform

	Uniform::Uniform(const UniformType& uniformType, const char* uniformName, const unsigned int& arraySize, const unsigned int offset)
		: type(uniformType), name(uniformName), arraySize(arraySize), offset(offset) {
	}

	Uniform* Uniform::CreateUniform(const unsigned int& shaderProg, const unsigned int& uniformIndex, const bool& checkLoc) {//const UniformType& uniformType, const char* uniformName, const unsigned int& arraySize) {
		char uniName[100];
		int length;
		int uniType, uniSize, uniOffset, uniArray;

		glGetActiveUniformName(shaderProg, uniformIndex, 100, &length, uniName);
		glGetActiveUniformsiv(shaderProg, 1, &uniformIndex, GL_UNIFORM_TYPE, &uniType); // Uniform Type
		glGetActiveUniformsiv(shaderProg, 1, &uniformIndex, GL_UNIFORM_SIZE, &uniSize); // Uniform Array Size
		glGetActiveUniformsiv(shaderProg, 1, &uniformIndex, GL_UNIFORM_OFFSET, &uniOffset); //
		glGetActiveUniformsiv(shaderProg, 1, &uniformIndex, GL_UNIFORM_ARRAY_STRIDE, &uniArray); // TODO: Use this
		
		if (glGetUniformLocation(shaderProg, uniName) == -1 && checkLoc) {
			// BE_WARNING("CreateUniform - Uniform %s not in use", uniName);
			return nullptr;
		}

		switch((UniformType)uniType) {
			case BE::UniformType::FLOAT: return new UniformFloat((UniformType)uniType, uniName, 1 * (unsigned int)uniSize, (unsigned int)uniOffset);
			case BE::UniformType::INT: return new UniformInt((UniformType)uniType, uniName, 1 * uniSize, uniOffset);
			case BE::UniformType::VECTOR_2: return new UniformFloat((UniformType)uniType, uniName, 2 * uniSize, uniOffset);
			case BE::UniformType::VECTOR_3: return new UniformFloat((UniformType)uniType, uniName, 3 * uniSize, uniOffset);
			case BE::UniformType::VECTOR_4: return new UniformFloat((UniformType)uniType, uniName, 4 * uniSize, uniOffset);
			case BE::UniformType::MATRIX_3: return new UniformFloat((UniformType)uniType, uniName, 9 * uniSize, uniOffset);
			case BE::UniformType::MATRIX_4: return new UniformFloat((UniformType)uniType, uniName, 16 * uniSize, uniOffset);
			case BE::UniformType::TEXTURE_2D: return new UniformTexture((UniformType)uniType, uniName, 1 * uniSize, uniOffset);
			case BE::UniformType::BOOLEAN: return new UniformInt((UniformType)uniType, uniName, 1 * uniSize, uniOffset);
			default: BE_ASSERT(false, "CreateUniform - Unsupported Uniform Type %i", uniType);
		}

		return nullptr;
	}

#pragma endregion
	// -----------------------------------------------------------

	// -----------------------------------------------------------
#pragma region Int_Uniform
	UniformInt::UniformInt(const UniformType& uniformType, const char* uniformName, const unsigned int& arraySize, const unsigned int offset)
		: Uniform(uniformType, uniformName, arraySize, offset) {
		data = new int[arraySize];
		for(int i = 0; i < arraySize; i++)
			data[i] = 0; // Default value
	}

	void UniformInt::SetValue(const void* value, const unsigned int& size) {
		BE_ASSERT(size <= arraySize, "UniformInt::SetValue - Size was above arraySize");

		for(int i = 0; i < size; i++)
			data[i] = ((int*)value)[i];
	}

	void* UniformInt::GetValue(const unsigned int& index) const {
		BE_ASSERT(index < arraySize, "UniformInt::GetValue - Index was above arraySize");
		return (void*)data[index];
	}
#pragma endregion
	// -----------------------------------------------------------

	// -----------------------------------------------------------
#pragma region Float_Uniform
	BE::UniformFloat::UniformFloat(const BE::UniformType& uniformType, const char* uniformName, const unsigned int& arraySize, const unsigned int& offset) 
		: Uniform(uniformType, uniformName, arraySize, offset) {
		data = new float[arraySize];
		for(int i = 0; i < arraySize; i++)
			data[i] = 0.0f; // Default value
	}

	void BE::UniformFloat::SetValue(const void* value, const unsigned int& size) {
		BE_ASSERT(size <= arraySize, "UniformFloat::SetValue - Size was above arraySize");

		for(int i = 0; i < size; i++)
			data[i] = ((float*)value)[i];
	}

	void* BE::UniformFloat::GetValue(const unsigned int& index) const {
		BE_ASSERT(index < arraySize, "UniformFloat::GetValue - Index was above arraySize");
		return (void*)&data[index];
	}
#pragma endregion
	// -----------------------------------------------------------


#pragma region Texture_Uniform
	// -----------------------------------------------------------
	UniformTexture::UniformTexture(const UniformType& uniformType, const char* uniformName, const unsigned int& arraySize, const unsigned int& offset)
		: Uniform(uniformType, uniformName, arraySize, offset)
	{
		texture = (ITexture*)((strncmp(uniformName, BE_NORMAL_PREFIX, 4) == 0 ? BE::BEGlobal::GetDefaultNormalTexture() : BEGlobal::GetDefaultTexture()));
	}

	void UniformTexture::SetValue(const void* value, const unsigned int& size)
	{
		ITexture* t = (ITexture*)value;
		if (t != nullptr)
			t->Bind();

		texture = t;
	}

	void* UniformTexture::GetValue(const unsigned int& index) const
	{
		texture->Bind();
		return (void*)texture->GetActiveID();
	}

	// -----------------------------------------------------------
#pragma endregion
}