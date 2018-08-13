#pragma once

#include "oaBehavior.h"
#include "oaGraphics.h"

class tileMap : public oaBehavior {
public:
	size_t**    tilesData;
	glm::ivec2  dimension;
	std::shared_ptr<oaEntity>** enitityMap;
	std::string wallModel;
	std::string foodModel;
	GLuint wallMaterial;
	GLuint foodMaterial;

	bool insideTile(glm::vec3 position);

	void     awake();
	void     start();
	void    update();
	void onDestroy();

	tileMap(oaComponent component);
	tileMap();
	~tileMap();

private:

	REGISTER_INSTANCE_METHOD(oaObject, tileMap)
};

namespace YAML {
	template<>
	struct convert<tileMap> {
		static Node encode(const tileMap& rhs) {
			Node node;
			node["tileMap"] = (oaComponent)rhs;

			node["tileMap"]["dimension"].SetStyle(YAML::EmitterStyle::Flow);
			node["tileMap"]["dimension"].push_back(rhs.dimension.x);
			node["tileMap"]["dimension"].push_back(rhs.dimension.y);

			node["tileMap"]["tilesData"].SetStyle(YAML::EmitterStyle::Flow);
			for (int i = 0; i < rhs.dimension.y; i++) {
				for (int j = 0; j < rhs.dimension.x; j++) {
					node["tileMap"]["tilesData"][i].push_back(rhs.tilesData[i][j]);
				}
			}

			node["tileMap"]["wallModel"] = rhs.wallModel;
			node["tileMap"]["wallMaterial"] = rhs.wallMaterial;
			node["tileMap"]["foodMaterial"] = rhs.foodMaterial;

			return node;
		}

		static bool decode(const Node& node, tileMap& rhs) {
			if (node.IsSequence()) {
				return false;
			}

			rhs = tileMap(node["tileMap"].as<oaComponent>());
			rhs.dimension.x = node["tileMap"]["dimension"][0].as<int>();
			rhs.dimension.y = node["tileMap"]["dimension"][1].as<int>();
			rhs.tilesData = new size_t*[rhs.dimension.y];
			for (int i = 0; i < rhs.dimension.y; i++) {
				rhs.tilesData[i] = new size_t[rhs.dimension.x];
				for (int j = 0; j < rhs.dimension.x; j++) {
					rhs.tilesData[i][j] = node["tileMap"]["tilesData"][i][j].as<size_t>();
				}
			}

			rhs.wallModel = node["tileMap"]["wallModel"].as<std::string>();
			rhs.foodModel = node["tileMap"]["foodModel"].as<std::string>();
			rhs.wallMaterial = node["tileMap"]["wallMaterial"].as<GLuint>();
			rhs.foodMaterial = node["tileMap"]["foodMaterial"].as<GLuint>();
			return true;
		}
	};
}