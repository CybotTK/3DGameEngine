#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "Camera.h"
#include "SceneObject.h"
#include "Light.h"

#include <unordered_map>
#include <vector>

class ResourceManager;
class Shader;

enum KeyAction {
	MOVE_CAMERA_POSITIVE_Z,
	MOVE_CAMERA_NEGATIVE_Z,
	MOVE_CAMERA_POSITIVE_X,
	MOVE_CAMERA_NEGATIVE_X,
	MOVE_CAMERA_POSITIVE_Y,
	MOVE_CAMERA_NEGATIVE_Y,
	ROTATE_CAMERA_OX_POSITIVE,
	ROTATE_CAMERA_OX_NEGATIVE,
	ROTATE_CAMERA_OY_POSITIVE,
	ROTATE_CAMERA_OY_NEGATIVE,
	ROTATE_CAMERA_OZ_POSITIVE,
	ROTATE_CAMERA_OZ_NEGATIVE,
	DEBUG,
	WIREFRAME,
};

class SceneManager {
public:
	virtual ~SceneManager();

	void Init();

	static SceneManager* GetInstance();
	void Method();

	void ParseXML();

	void InitializeWindow();
	void Initialize();
	void FreeResources();

	void Draw();
	void Update(float deltaTime);

	struct WindowSettings {
		float r;
		float g;
		float b;
		bool fullscreen;
		float width;
		float height;
		std::string name;
	} m_windowSettings;

	struct DebugSettings {
		// We use x = r, y = g, z = b
		Vector3 oX;
		Vector3 oY;
		Vector3 oZ;
	} objectAxes, camAxes;

	std::unordered_map<char, KeyAction> keyMapping;

	std::unordered_map<int, Camera*> m_cameras;
	int activeCamera = 1;

	ResourceManager* m_resourceManager;
	std::unordered_map<int, SceneObject*> sceneObjects;

	float fogSmallRadius;
	float fogBigRadius;

	std::vector<Texture*> reflectionTextures;

	std::unordered_map<int, Light*> lights;
	Vector3 ambiLight; // color
	float ratioAmbiLight;

	// debugging
	bool debug = false;
	bool wireframe = false;
	Shader* shaderDebug;
	Shader* shaderAxes;

	// Axes Data
	float cylinderHeight;
	int cylinderSegments;
	float cylinderRadius;
	GLuint cylinderVboId;
	GLuint cylinderEboId;
	std::vector<Vector3> axesVertices;
	std::vector<unsigned short> axesIndices;
private:
	static SceneManager* singletonInstance;
	SceneManager();

	void CreateAxes();
};

#endif // !SCENE_MANAGER_H
