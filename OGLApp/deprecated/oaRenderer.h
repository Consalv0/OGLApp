#pragma once

#include "oaBehavior.h"
#include "oaGraphics.h"
#include "oaMaterial.h"
#include "oaMesh.h"

#include <list>
#include <memory>

#include <cereal\types\memory.hpp>
#include <cereal\types\list.hpp>


class oaRenderer : public oaBehavior {
public:
	std::shared_ptr<oaMesh> sharedMesh;
	std::unique_ptr<oaMaterial> material;

	void     awake();
	void     start();
	void    update();
	void onDestroy();

	oaRenderer();

private:
	friend class oaEngine;
	friend class cereal::access;
	void activateDrawConfiguration();
	void draw();

	template<class Archive>
	void serialize(Archive & archive) {
		archive(CEREAL_NVP(sharedMesh));
		archive(CEREAL_NVP(material));
		archive(cereal::virtual_base_class<oaComponent>(this));
	}
};