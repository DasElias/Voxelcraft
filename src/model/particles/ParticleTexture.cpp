#include "ParticleTexture.h"

#include "../../renderModel/TextureLoader.h"

namespace vc::model {
	ParticleTexture::ParticleTexture(std::string path, int stages, bool isAdditive) :
			backboneTexture(vc::renderingModel::TextureLoader::getInstance()->loadTexture(path)),
			stages(stages),
			field_useAdditiveBlending(isAdditive) {
	}

	int ParticleTexture::getTextureId() const {
		return backboneTexture.getTexId();
	}

	int ParticleTexture::getStages() const {
		return stages;
	}

	int ParticleTexture::getWidth() const {
		return backboneTexture.getWidth();
	}

	int ParticleTexture::getHeight() const {
		return backboneTexture.getHeight();
	}

	bool ParticleTexture::useAdditiveBlending() const {
		return field_useAdditiveBlending;
	}

	void ParticleTexture::cleanUp() {
		backboneTexture.cleanUp();
	}


}

