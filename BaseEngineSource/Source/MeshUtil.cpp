#include "MeshUtil.h"

namespace BE::MeshUtil {
	Mesh* CreateCube(const float& size) {
		return new Mesh(
			{
				 // Bot
				-0.5f * size,-0.5f * size, 0.5f * size,  0.0f,-1.0f, 0.0f,  0.0f, 0.0f,
				-0.5f * size,-0.5f * size,-0.5f * size,  0.0f,-1.0f, 0.0f,  0.0f, 1.0f,
				 0.5f * size,-0.5f * size,-0.5f * size,  0.0f,-1.0f, 0.0f,  1.0f, 1.0f,
				 0.5f * size,-0.5f * size, 0.5f * size,  0.0f,-1.0f, 0.0f,  1.0f, 0.0f,

				 // Top
				-0.5f * size, 0.5f * size, 0.5f * size,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
				-0.5f * size, 0.5f * size,-0.5f * size,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
				 0.5f * size, 0.5f * size,-0.5f * size,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
				 0.5f * size, 0.5f * size, 0.5f * size,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,

				 // Left
				-0.5f * size,-0.5f * size, 0.5f * size, -1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
				-0.5f * size,-0.5f * size,-0.5f * size, -1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
				-0.5f * size, 0.5f * size,-0.5f * size, -1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
				-0.5f * size, 0.5f * size, 0.5f * size, -1.0f, 0.0f, 0.0f,  1.0f, 1.0f,

				 // Right
				 0.5f * size,-0.5f * size, 0.5f * size,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
				 0.5f * size,-0.5f * size,-0.5f * size,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
				 0.5f * size, 0.5f * size,-0.5f * size,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
				 0.5f * size, 0.5f * size, 0.5f * size,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,

				 // Front
				-0.5f * size,-0.5f * size, 0.5f * size,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
				-0.5f * size, 0.5f * size, 0.5f * size,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,
				 0.5f * size, 0.5f * size, 0.5f * size,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
				 0.5f * size,-0.5f * size, 0.5f * size,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,

				 // Back
				-0.5f * size,-0.5f * size,-0.5f * size,  0.0f, 0.0f,-1.0f,  1.0f, 1.0f,
				-0.5f * size, 0.5f * size,-0.5f * size,  0.0f, 0.0f,-1.0f,  1.0f, 0.0f,
				 0.5f * size, 0.5f * size,-0.5f * size,  0.0f, 0.0f,-1.0f,  0.0f, 0.0f,
				 0.5f * size,-0.5f * size,-0.5f * size,  0.0f, 0.0f,-1.0f,  0.0f, 1.0f
			},
			{
				3, 3, 2
			},
			{
				0, 1, 2,  0, 2, 3, // BOT
				4, 5, 6,  4, 6, 7, // TOP
				8, 9,10,  8,10,11, // LEFT
			   12,13,14, 12,14,15, // RIGHT
			   16,17,18, 16,18,19, // FRONT
			   20,21,22, 20,22,23  // BACK
			}
		);
	}

    Mesh* CreatePlane(const float& size) {
		return new Mesh(
			{
				// Bot
				-0.5f * size, 0.0f , 0.5f * size,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
				-0.5f * size, 0.0f ,-0.5f * size,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
				 0.5f * size, 0.0f ,-0.5f * size,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
				 0.5f * size, 0.0f , 0.5f * size,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
			}, 
			{
				3, 3, 2
			}, 
			{
				0, 1, 2,  0, 2, 3
			}
		);
    }
}