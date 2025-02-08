#include "stdafx.h"

#include "ResourceManager.h"

ResourceManager* ResourceManager::singletonInstance = nullptr;

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
	CLEANUP_MAP(modelResourcesLoaded);
	CLEANUP_MAP(modelResourcesUnloaded);
	CLEANUP_MAP(shaderResourcesLoaded);
	CLEANUP_MAP(shaderResourcesUnloaded);
	CLEANUP_MAP(textureResourcesLoaded);
	CLEANUP_MAP(textureResourcesUnloaded);
}

ResourceManager* ResourceManager::GetInstance()
{
	if (!singletonInstance)
	{
		singletonInstance = new ResourceManager();
	}
	
	return singletonInstance;
}

void ResourceManager::Method()
{
}

void ResourceManager::IterateThroughModelsNode(rapidxml::xml_node<>* modelsNode) {
	

	for (rapidxml::xml_node<>* folder = modelsNode->first_node("folder");
		folder;
		folder = folder->next_sibling("folder")
		) {
		std::string folderPath = "../../";
		folderPath = folderPath + folder->first_attribute("path")->value();

		for (rapidxml::xml_node<>* model = folder->first_node("model");
			model;
			model = model->next_sibling("model")
			) {
			ModelResource* modelRes;
			modelRes = new ModelResource;

			modelRes->id = std::stoi(model->first_attribute("id")->value());
			std::string currentModelFolderPath;

			currentModelFolderPath = folderPath + model->first_node("file")->value();
			modelRes->filePath = currentModelFolderPath;

			modelResourcesUnloaded[modelRes->id] = modelRes;
		}
	}
}

void ResourceManager::IterateThroughShadersNode(rapidxml::xml_node<>* shadersNode)
{

	for (rapidxml::xml_node<>* folder = shadersNode->first_node("folder");
		folder;
		folder = folder->next_sibling("folder")
		) {
		std::string folderPath = "../";
		folderPath = folderPath + folder->first_attribute("path")->value();

		for (rapidxml::xml_node<>* shader = folder->first_node("shader");
			shader;
			shader = shader->next_sibling("shader")
			) {
			ShaderResource* shaderRes;
			shaderRes = new ShaderResource;

			shaderRes->id = std::stoi(shader->first_attribute("id")->value());

			std::string vsPath;
			vsPath = folderPath + shader->first_node("vs")->value();
			strcpy(shaderRes->vsFilePath, vsPath.c_str());

			std::string fsPath;
			fsPath = folderPath + shader->first_node("fs")->value();
			strcpy(shaderRes->fsFilePath, fsPath.c_str());

			shaderResourcesUnloaded[shaderRes->id] = shaderRes;
		}
	}
}

void ResourceManager::IterateThroughTexturesNode(rapidxml::xml_node<>* texturesNode)
{

	std::unordered_map<std::string, GLenum> openGLDict = {
		{"LINEAR" , GL_LINEAR },
		{"NEAREST", GL_NEAREST},
		{"REPEAT", GL_REPEAT},
		{"CLAMP_TO_EDGE", GL_CLAMP_TO_EDGE},
		{"MIRRORED_REPEAT", GL_MIRRORED_REPEAT},
		{"2d", GL_TEXTURE_2D},
		{"cube", GL_TEXTURE_CUBE_MAP},

	};

	for (rapidxml::xml_node<>* folder = texturesNode->first_node("folder");
		folder;
		folder = folder->next_sibling("folder")
		) {
		std::string folderPath = "../../";
		folderPath = folderPath + folder->first_attribute("path")->value();

		for (rapidxml::xml_node<>* texture = folder->first_node("texture");
			texture;
			texture = texture->next_sibling("texture")
			) {
			TextureResource* textureRes;
			textureRes = new TextureResource;

			textureRes->id = std::stoi(texture->first_attribute("id")->value());
			std::string typeString = texture->first_attribute("type")->value();
			textureRes->type = openGLDict[typeString];

			std::string currentTextureFolderPath;
			currentTextureFolderPath = folderPath + texture->first_node("file")->value();
			strcpy(textureRes->filePath, currentTextureFolderPath.c_str());

			//MIN_FILTER
			std::string filter = texture->first_node("min_filter")->value();
			textureRes->minFilter = openGLDict[filter];

			//MAG_FILTER
			filter = texture->first_node("mag_filter")->value();
			textureRes->magFilter = openGLDict[filter];

			//WRAP_S
			std::string wrap = texture->first_node("wrap_s")->value();
			textureRes->wrapS = openGLDict[wrap];

			//WRAP_T
			wrap = texture->first_node("wrap_t")->value();
			textureRes->wrapT = openGLDict[wrap];

			textureResourcesUnloaded[textureRes->id] = textureRes;
		}
	}
}

void ResourceManager::Initialize() {
	rapidxml::file<> file("XMLFiles/resourceManager.xml");
	char* buffer = file.data();

	rapidxml::xml_document<> doc;
	doc.parse<0>(buffer);

	rapidxml::xml_node<>* root = doc.first_node("resourceManager");
	if (!root) {
		std::cerr << "No root node found!" << std::endl;
		return;
	}

	rapidxml::xml_node<>* modelsNode = root->first_node("models");
	if (modelsNode) {
		IterateThroughModelsNode(modelsNode);
	}
	
	rapidxml::xml_node<>* shadersNode = root->first_node("shaders");
	if (shadersNode) {
		IterateThroughShadersNode(shadersNode);
	}

	rapidxml::xml_node<>* texturesNode = root->first_node("textures");
	if (texturesNode) {
		IterateThroughTexturesNode(texturesNode);
	}
}

void ResourceManager::FreeResources() {
}

Model* ResourceManager::LoadModel(ModelResource* modelRes)
{
	if (modelResourcesLoaded.find(modelRes->id) != modelResourcesLoaded.end())
	{
		return modelResourcesLoaded[modelRes->id];
	}
	
	Model* newModel;
	newModel = new Model;
	newModel->m_modelResource = modelRes;
	newModel->Load();
	modelResourcesLoaded[modelRes->id] = newModel;
	return newModel;
}

Texture* ResourceManager::LoadTexture(TextureResource* textureRes)
{
	if (textureResourcesLoaded.find(textureRes->id) != textureResourcesLoaded.end())
	{
		return textureResourcesLoaded[textureRes->id];
	}

	Texture* newTexture;
	newTexture = new Texture;
	newTexture->m_textureResource = textureRes;
	newTexture->Load();

	textureResourcesLoaded[textureRes->id] = newTexture;

	return newTexture;
}

Shader* ResourceManager::LoadShader(ShaderResource* shaderRes)
{
	if (shaderResourcesLoaded.find(shaderRes->id) != shaderResourcesLoaded.end()) {
		return shaderResourcesLoaded[shaderRes->id];
	}

	Shader* newShader;
	newShader = new Shader;
	newShader->m_shaderResource = shaderRes;

	int error = newShader->Load();
	if (error != 0) {
		std::cerr << "Shader did not load correctly" << '\n';
	}

	shaderResourcesLoaded[shaderRes->id] = newShader;

	return newShader;
}
