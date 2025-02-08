#include "stdafx.h"

#include "SceneManager.h"
#include "ResourceManager.h"
#include "Terrain.h"
#include "SkyBox.h"

SceneManager* SceneManager::singletonInstance = nullptr;

SceneManager::SceneManager() {

}

SceneManager::~SceneManager()
{
	CLEANUP_MAP(m_cameras);
	CLEANUP_MAP(sceneObjects);
	delete m_resourceManager;
}

SceneManager* SceneManager::GetInstance()
{
	if (!singletonInstance)
	{
		singletonInstance = new SceneManager();
	}

	return singletonInstance;
}

void SceneManager::Method()
{
}

void SceneManager::ParseXML()
{

}

void SceneManager::InitializeWindow()
{
	rapidxml::file<> file("XMLFiles/sceneManager.xml");
	char* buffer = file.data();

	rapidxml::xml_document<> doc;
	doc.parse<0>(buffer);

	rapidxml::xml_node<>* root = doc.first_node("sceneManager");
	if (!root) {
		std::cerr << "No root node found!" << std::endl;
		return;
	}

	rapidxml::xml_node<>* nextNode = root->first_node("gameName");
	m_windowSettings.name = nextNode->value();

	rapidxml::xml_node<>* screenSizeNode = root->first_node("defaultScreenSize");
	m_windowSettings.width = std::stoi(screenSizeNode->first_node("width")->value());
	m_windowSettings.height = std::stoi(screenSizeNode->first_node("height")->value());

	rapidxml::xml_node<>* fullscreenNode = screenSizeNode->first_node("fullscreen");
	if (fullscreenNode) {
		m_windowSettings.fullscreen = true;
	}
	m_windowSettings.fullscreen = false;

	rapidxml::xml_node<>* bkgColorNode = root->first_node("backgroundColor");
	m_windowSettings.r = std::stof(bkgColorNode->first_node("r")->value());
	m_windowSettings.g = std::stof(bkgColorNode->first_node("g")->value());
	m_windowSettings.b = std::stof(bkgColorNode->first_node("b")->value());
}

void SceneManager::Initialize()
{
	rapidxml::file<> file("XMLFiles/sceneManager.xml");
	char* buffer = file.data();

	rapidxml::xml_document<> doc;
	doc.parse<0>(buffer);

	rapidxml::xml_node<>* root = doc.first_node("sceneManager");
	if (!root) {
		std::cerr << "No root node found!" << std::endl;
		return;
	}

#pragma region Controls

	rapidxml::xml_node<>* controlsNode = root->first_node("controls");

	if (controlsNode) {
		for (rapidxml::xml_node<>* controlNode = controlsNode->first_node("control");
			controlNode;
			controlNode = controlNode->next_sibling("control")
			) {
			std::string stringValue = controlNode->first_node("key")->value();
			char keyControl = stringValue[0];

			std::string action = controlNode->first_node("action")->value();
			if (action == "MOVE_CAMERA_POSITIVE_Z") {
				keyMapping[keyControl] = KeyAction::MOVE_CAMERA_POSITIVE_Z;
			}
			if (action == "MOVE_CAMERA_NEGATIVE_Z") {
				keyMapping[keyControl] = KeyAction::MOVE_CAMERA_NEGATIVE_Z;
			}
			if (action == "MOVE_CAMERA_POSITIVE_X") {
				keyMapping[keyControl] = KeyAction::MOVE_CAMERA_POSITIVE_X;
			}
			if (action == "MOVE_CAMERA_NEGATIVE_X") {
				keyMapping[keyControl] = KeyAction::MOVE_CAMERA_NEGATIVE_X;
			}
			if (action == "MOVE_CAMERA_POSITIVE_Y") {
				keyMapping[keyControl] = KeyAction::MOVE_CAMERA_POSITIVE_Y;
			}
			if (action == "MOVE_CAMERA_NEGATIVE_Y") {
				keyMapping[keyControl] = KeyAction::MOVE_CAMERA_NEGATIVE_Y;
			}
			if (action == "ROTATE_CAMERA_OZ_POSITIVE") {
				keyMapping[keyControl] = KeyAction::ROTATE_CAMERA_OZ_POSITIVE;
			}
			if (action == "ROTATE_CAMERA_OZ_NEGATIVE") {
				keyMapping[keyControl] = KeyAction::ROTATE_CAMERA_OZ_NEGATIVE;
			}
			if (stringValue == "VK_UP") {
				keyMapping[VK_UP] = KeyAction::ROTATE_CAMERA_OX_POSITIVE;
			}
			if (stringValue == "VK_DOWN") {
				keyMapping[VK_DOWN] = KeyAction::ROTATE_CAMERA_OX_NEGATIVE;
			}
			if (stringValue == "VK_LEFT") {
				keyMapping[VK_LEFT] = KeyAction::ROTATE_CAMERA_OY_POSITIVE;
			}
			if (stringValue == "VK_RIGHT") {
				keyMapping[VK_RIGHT] = KeyAction::ROTATE_CAMERA_OY_NEGATIVE;
			}

		}
	}

#pragma endregion

#pragma region Cameras

	rapidxml::xml_node<>* camerasNode = root->first_node("cameras");

	if (camerasNode) {
		for (rapidxml::xml_node<>* cameraNode = camerasNode->first_node("camera");
			cameraNode;
			cameraNode = cameraNode->next_sibling("camera")
			) {
			int id = std::stoi(cameraNode->first_attribute("id")->value());

			Camera* camera = new Camera;

			rapidxml::xml_node<>* positionNode = cameraNode->first_node("position");
			std::string positionX = positionNode->first_node("x")->value();
			camera->position.x = std::stof(positionNode->first_node("x")->value());
			camera->position.y = std::stof(positionNode->first_node("y")->value());
			camera->position.z = std::stof(positionNode->first_node("z")->value());

			rapidxml::xml_node<>* targetNode = cameraNode->first_node("target");
			camera->target.x = std::stof(targetNode->first_node("x")->value());
			camera->target.y = std::stof(targetNode->first_node("y")->value());
			camera->target.z = std::stof(targetNode->first_node("z")->value());

			rapidxml::xml_node<>* upNode = cameraNode->first_node("up");
			camera->up.x = std::stof(upNode->first_node("x")->value());
			camera->up.y = std::stof(upNode->first_node("y")->value());
			camera->up.z = std::stof(upNode->first_node("z")->value());

			camera->moveSpeed = std::stof(cameraNode->first_node("translationSpeed")->value());

			camera->rotateSpeed = std::stof(cameraNode->first_node("rotationSpeed")->value());

			camera->fov = std::stof(cameraNode->first_node("fov")->value());

			camera->nearPlane = std::stof(cameraNode->first_node("near")->value());
			camera->farPlane = std::stof(cameraNode->first_node("far")->value());

			GLfloat aspectRatio = (float)m_windowSettings.width / (float)m_windowSettings.height;
			camera->SetPerspectiveMatrix(aspectRatio);

			m_cameras[id] = camera;
			
		}
	}

	activeCamera = std::stoi(root->first_node("activeCamera")->value());

#pragma endregion

#pragma region Fog

	rapidxml::xml_node<>* fogNode = root->first_node("fog");

	fogSmallRadius = std::stof(fogNode->first_node("smallRadius")->value());
	fogBigRadius = std::stof(fogNode->first_node("bigRadius")->value());

#pragma endregion

#pragma region Objects

	rapidxml::xml_node<>* objectsNode = root->first_node("objects");

	if (objectsNode) {
		for (rapidxml::xml_node<>* objectNode = objectsNode->first_node("object");
			objectNode;
			objectNode = objectNode->next_sibling("object")
			) {

			int id = std::stoi(objectNode->first_attribute("id")->value());

			SceneObject* newSceneObject;

			std::string typeString = objectNode->first_node("type")->value();

			if (typeString == "normal") {
				newSceneObject = new SceneObject;

				newSceneObject->type = newSceneObject->NORMAL;

				int modelId = std::stoi(objectNode->first_node("model")->value());
				newSceneObject->model = 
					m_resourceManager->LoadModel(m_resourceManager->modelResourcesUnloaded[modelId]);
			}

			if (typeString == "terrain") {
				newSceneObject = new Terrain;

				newSceneObject->type = newSceneObject->TERRAIN;

				newSceneObject->terrainType = newSceneObject->GENERATED;

				//get data for terrain
				int numberOfCells = std::stoi(objectNode->first_node("cells")->value());
				int sizeOfCells = std::stoi(objectNode->first_node("cellSize")->value());

				Model* terrainModel = new Model;
				terrainModel->GenerateSquareModel(numberOfCells, sizeOfCells);

				newSceneObject->model=terrainModel;

				rapidxml::xml_node<>* heightNode = objectNode->first_node("height");

				newSceneObject->height.x = std::stoi(heightNode->first_node("r")->value());
				newSceneObject->height.y = std::stoi(heightNode->first_node("g")->value());
				newSceneObject->height.z = std::stoi(heightNode->first_node("b")->value());
			}

			if (typeString == "skybox") {
				newSceneObject = new SkyBox;

				newSceneObject->type = newSceneObject->NORMAL;

				int modelId = std::stoi(objectNode->first_node("model")->value());
				newSceneObject->model =
					m_resourceManager->LoadModel(m_resourceManager->modelResourcesUnloaded[modelId]);

			}

			newSceneObject->objectId = id;

			int shaderId = std::stoi(objectNode->first_node("shader")->value());
			newSceneObject->shader = 
				m_resourceManager->LoadShader(m_resourceManager->shaderResourcesUnloaded[shaderId]);

			newSceneObject->objectName = objectNode->first_node("name")->value();

			rapidxml::xml_node<>* texturesNode = objectNode->first_node("textures");

			if (texturesNode) {
				for (rapidxml::xml_node<>* textureNode = texturesNode->first_node("texture");
					textureNode;
					textureNode = textureNode->next_sibling("texture")
					) {
					int textureId = std::stoi(textureNode->first_attribute("id")->value());
					Texture* newTexture = new Texture;
					newSceneObject->textures.push_back(newTexture);//Allocate memory

					newTexture = m_resourceManager->LoadTexture(m_resourceManager->textureResourcesUnloaded[textureId]);
					newSceneObject->textures[newSceneObject->textures.size() - 1] = newTexture;
				}

				newSceneObject->enableDepthTest = true;
			}

			rapidxml::xml_node<>* wiredNode = objectNode->first_node("wired");
			if (wiredNode) {
				newSceneObject->isWired = true;
				newSceneObject->enableDepthTest = false;
			}

			rapidxml::xml_node<>* positionNode = objectNode->first_node("position");
			newSceneObject->position.x = std::stof(positionNode->first_node("x")->value());
			newSceneObject->position.y = std::stof(positionNode->first_node("y")->value());
			newSceneObject->position.z = std::stof(positionNode->first_node("z")->value());

			rapidxml::xml_node<>* rotationNode = objectNode->first_node("rotation");
			newSceneObject->rotation.x = std::stof(rotationNode->first_node("x")->value());
			newSceneObject->rotation.y = std::stof(rotationNode->first_node("y")->value());
			newSceneObject->rotation.z = std::stof(rotationNode->first_node("z")->value());

			rapidxml::xml_node<>* scaleNode = objectNode->first_node("scale");
			newSceneObject->scale.x = std::stof(scaleNode->first_node("x")->value());
			newSceneObject->scale.y = std::stof(scaleNode->first_node("y")->value());
			newSceneObject->scale.z = std::stof(scaleNode->first_node("z")->value());

			newSceneObject->m_camera = m_cameras[activeCamera];

			rapidxml::xml_node<>* followingCameraNode = objectNode->first_node("followingCamera");
			if (followingCameraNode){
				newSceneObject->hasFollowingCamera = true;

				if (followingCameraNode->first_node("ox")) {
					newSceneObject->followingCamera.x = 1;
				}
				if (followingCameraNode->first_node("oy")) {
					newSceneObject->followingCamera.y = 1;
				}
				if (followingCameraNode->first_node("oz")) {
					newSceneObject->followingCamera.z = 1;
				}

				newSceneObject->offsetCamera = newSceneObject->position;
			}

			sceneObjects[id] = newSceneObject;
		}
	}

#pragma endregion

#pragma region DebugSettings

	rapidxml::xml_node<>* debugSettingsNode = root->first_node("debugSettings");

	if (debugSettingsNode) {
		
		rapidxml::xml_node<>* objectAxesNode = debugSettingsNode->first_node("objectAxes");
		if (objectAxesNode) {
			rapidxml::xml_node<>* oXColor = objectAxesNode->first_node("OXColor");
			objectAxes.oX.x = std::stof(oXColor->first_node("r")->value());
			objectAxes.oX.y = std::stof(oXColor->first_node("g")->value());
			objectAxes.oX.z = std::stof(oXColor->first_node("b")->value());

			rapidxml::xml_node<>* oYColor = objectAxesNode->first_node("OYColor");
			objectAxes.oY.x = std::stof(oYColor->first_node("r")->value());
			objectAxes.oY.y = std::stof(oYColor->first_node("g")->value());
			objectAxes.oY.z = std::stof(oYColor->first_node("b")->value());

			rapidxml::xml_node<>* oZColor = objectAxesNode->first_node("OZColor");
			objectAxes.oZ.x = std::stof(oZColor->first_node("r")->value());
			objectAxes.oZ.y = std::stof(oZColor->first_node("g")->value());
			objectAxes.oZ.z = std::stof(oZColor->first_node("b")->value());
		}

		rapidxml::xml_node<>* camAxesNode = debugSettingsNode->first_node("camAxes");
		if (camAxesNode) {
			rapidxml::xml_node<>* oXColor = camAxesNode->first_node("OXColor");
			camAxes.oX.x = std::stof(oXColor->first_node("r")->value());
			camAxes.oX.y = std::stof(oXColor->first_node("g")->value());
			camAxes.oX.z = std::stof(oXColor->first_node("b")->value());

			rapidxml::xml_node<>* oYColor = camAxesNode->first_node("OYColor");
			camAxes.oY.x = std::stof(oYColor->first_node("r")->value());
			camAxes.oY.y = std::stof(oYColor->first_node("g")->value());
			camAxes.oY.z = std::stof(oYColor->first_node("b")->value());

			rapidxml::xml_node<>* oZColor = camAxesNode->first_node("OZColor");
			camAxes.oZ.x = std::stof(oZColor->first_node("r")->value());
			camAxes.oZ.y = std::stof(oZColor->first_node("g")->value());
			camAxes.oZ.z = std::stof(oZColor->first_node("b")->value());
		}
	}

#pragma endregion

}

void SceneManager::FreeResources()
{
}

void SceneManager::Draw() {

	for (auto obj : sceneObjects) {
		obj.second->Draw();
	}

}

void SceneManager::Update(float deltaTime)
{
	for (auto obj : sceneObjects) {
		obj.second->Update(deltaTime);
	}

	m_cameras[activeCamera]->setDeltaTime(deltaTime);
	m_cameras[activeCamera]->UpdateAxes();
	m_cameras[activeCamera]->UpdateWorldView();
}
