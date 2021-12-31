#include "IRender.h"
#include "Entity.h"

namespace BE {
	IRender::IRender() : mesh(nullptr), material(nullptr) {
	}

	IRender::~IRender() {
	}
}