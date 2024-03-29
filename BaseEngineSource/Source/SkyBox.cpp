//#include "SkyBox.h"
//
//namespace BE {
//	SkyBox::SkyBox(const char* cubemapPath, Shader* shader) {
//		// Vertices & attributes
//		std::vector<float> vertices = std::vector<float>({
//			/* FRONT */
//			-1.0f, 1.0f,-1.0f,
//			-1.0f,-1.0f,-1.0f,
//			 1.0f,-1.0f,-1.0f,
//
//			 1.0f,-1.0f,-1.0f,
//			 1.0f, 1.0f,-1.0f,
//			-1.0f, 1.0f,-1.0f,
//
//			/* RIGHT */
//			 1.0f,-1.0f, 1.0f,
//			 1.0f, 1.0f, 1.0f,
//			 1.0f,-1.0f,-1.0f,
//
//			 1.0f, 1.0f,-1.0f,
//			 1.0f,-1.0f,-1.0f,
//			 1.0f, 1.0f, 1.0f,
//
//			 /* BACK */
//			-1.0f,-1.0f, 1.0f,
//			-1.0f, 1.0f, 1.0f,
//			 1.0f,-1.0f, 1.0f,
//
//			 1.0f, 1.0f, 1.0f,
//			 1.0f,-1.0f, 1.0f,
//			-1.0f, 1.0f, 1.0f,
//
//			/* LEFT */
//			-1.0f,-1.0f,-1.0f,
//			-1.0f, 1.0f,-1.0f,
//			-1.0f,-1.0f, 1.0f,
//
//			-1.0f, 1.0f, 1.0f,
//			-1.0f,-1.0f, 1.0f,
//			-1.0f, 1.0f,-1.0f,
//
//			/* TOP */
//			-1.0f, 1.0f,-1.0f,
//			 1.0f, 1.0f,-1.0f,
//			-1.0f, 1.0f, 1.0f,
//
//			 1.0f, 1.0f, 1.0f,
//			-1.0f, 1.0f, 1.0f,
//			 1.0f, 1.0f,-1.0f,
//
//			 /* BOT */
//			-1.0f,-1.0f,-1.0f,
//			-1.0f,-1.0f, 1.0f,
//			 1.0f,-1.0f,-1.0f,
//
//			 1.0f,-1.0f,-1.0f,
//			-1.0f,-1.0f, 1.0f,
//			 1.0f,-1.0f, 1.0f,
//														 });
//		std::vector<unsigned int> attributes = std::vector<unsigned int>({
//			3
//																		 });
//
//		texture = new Cubemap(cubemapPath);
//		cube = new Object3D(glm::vec3(0), vertices, attributes);
//
//		cube->shader = shader;
//		cube->shader->SetTextureUnit("cubemap", 0);
//	}
//
//	SkyBox::~SkyBox() {
//		delete texture;
//		delete cube;
//	}
//
//	void SkyBox::Render(Camera& camera) {
//		//texture->BindTexture(0);
//		//
//		//cube->shader->SetMatrix4("projection", camera.Projection());
//		//cube->shader->SetMatrix4("view", glm::mat4(glm::mat3(camera.View())));
//		//
//		//glDepthMask(GL_FALSE);
//		//cube->RawRender();
//		//glDepthMask(GL_TRUE);
//	}
//
//	Shader* SkyBox::GetShader() {
//		return cube->shader;
//	}
//}