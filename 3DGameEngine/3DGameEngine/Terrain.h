#ifndef TERRAIN_H
#define TERRAIN_H

#include "SceneObject.h"

class Terrain : public SceneObject {
public:
	Terrain();
	virtual ~Terrain();

	void sendSpecificData() override;

	void Update(float deltaTime) override;

	void TerrainUpdate(Vector3 cameraPosition);
	void TerrainUpdateUVs(Vector3 cameraPosition);


private:
	Vector3 terrainCenter;
	Matrix terrainTranslation;
};


#endif // !TERRAIN_H

