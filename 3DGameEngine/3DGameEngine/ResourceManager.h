#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "Texture.h"
#include "Model.h"
#include "Shaders.h"

#include <unordered_map>
#include <string>

#define CLEANUP_MAP(map)         \
    do {                         \
        for (auto& pair : map) { \
            delete pair.second;  \
        }                        \
        map.clear();             \
    } while (0)

class ResourceManager {
public:
	virtual ~ResourceManager();

	static ResourceManager* GetInstance();
	void Method();

	void Initialize();
	void FreeResources();

	void IterateThroughModelsNode(rapidxml::xml_node<>* modelsNode);
	void IterateThroughShadersNode(rapidxml::xml_node<>* shadersNode);
	void IterateThroughTexturesNode(rapidxml::xml_node<>* texturesNode);

	Model* LoadModel(ModelResource* modelRes);
	Texture* LoadTexture(TextureResource* textureRes);
	Shader* LoadShader(ShaderResource* shaderRes);

	std::unordered_map <int, ModelResource*>   modelResourcesUnloaded;
	std::unordered_map <int, TextureResource*> textureResourcesUnloaded;
	std::unordered_map <int, ShaderResource*>  shaderResourcesUnloaded;

	std::unordered_map <int, Model*>   modelResourcesLoaded;
	std::unordered_map <int, Texture*> textureResourcesLoaded;
	std::unordered_map <int, Shader*>  shaderResourcesLoaded;

private:
	static ResourceManager* singletonInstance;
	ResourceManager();

};

#endif // !RESOURCE_MANAGER_H


