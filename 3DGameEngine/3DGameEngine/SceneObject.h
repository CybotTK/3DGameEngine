#ifndef SCENE_OBJECT_H
#define SCENE_OBJECT_H

#include "../Utilities/utilities.h"
#include <string>
#include <vector>

class Model;
class Shader;
class Texture;
class Camera;

class SceneObject {
public:
	SceneObject();
	virtual ~SceneObject();

	void sendCommonData();
	virtual void sendSpecificData();
	virtual void Draw();

	virtual void Update(float deltaTime);

	int objectId; //objectId from XML

	std::string objectName;
	
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;

	Model* model;
	Shader* shader;
	std::vector<Texture*> textures;

	enum TypeOfModel {
		NORMAL,
		TERRAIN,
	} type;

	// Height is for terrain
	// x = r, y = g, z = b
	Vector3 height;

	enum TerrainType {
		GENERATED,
	}terrainType;

	Camera* m_camera;

	Vector3 followingCamera = { 0.f, 0.f, 0.f };
	bool hasFollowingCamera = false;
	Vector3 offsetCamera;

	bool enableDepthTest = true;
	bool enableBlend = false;
	bool isWired = false;

	// 0 means no reflection
	float reflection = 0.f; // Value between 0 and 1 
	Texture* reflectionTexture;

	int countIndices;
	GLuint modelVboId, modelEboId, modelTextureId;

	Matrix modelMatrix;
private:
};

#endif // !SCENE_OBJECT_H
