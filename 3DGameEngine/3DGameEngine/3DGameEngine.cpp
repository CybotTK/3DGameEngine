// 3DGameEngine.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Vertex.h"
#include "Shaders.h"
#include "Globals.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "SceneManager.h"

#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include

#include <conio.h>
#include <iostream>
#include <fstream>
#include <vector>

#define PI 3.14


GLuint vboId, eboId,
lineVboId,
terrainModelVboId, terrainModelEboId, modelTextureId;
Shader cameraShaders, lineShaders, shapeShaders;
Shader* terrainShaders;
Shader* modelShaders;


int countIndices;
float angle = 0.f;
float step = 0.01f;
float totalTime = 0.f;
Camera camera;

ResourceManager* resourceManager;
SceneManager* sceneManager;

int Init ( ESContext *esContext )
{
	
	//stuff for Model
	std::vector<Vertex> verticesCoords;
	std::vector<unsigned short> modelIndicesData;

	glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );
	
	///TESTING RESOURCE MANAGER
	resourceManager = ResourceManager::GetInstance();
	resourceManager->Initialize();

	//TESTING SCENE MANAGER
	sceneManager->m_resourceManager = resourceManager;
	sceneManager->Initialize();

	//creation of shaders and program 
	int error=0;

	if (error != 0)
		return error;

	return error;

}

void Draw ( ESContext *esContext )
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	sceneManager->Draw();

	eglSwapBuffers ( esContext->eglDisplay, esContext->eglSurface );
}

void Update ( ESContext *esContext, float deltaTime )
{
	totalTime += deltaTime;

	if (totalTime > Globals::frameTime)
	{
		sceneManager->Update(totalTime);

		totalTime -= Globals::frameTime;
	}

}

void Key ( ESContext *esContext, unsigned char key, bool bIsPressed)
{
	if (!bIsPressed)
		return;

	if (sceneManager->keyMapping.find(key) != sceneManager->keyMapping.end()) {
		KeyAction action = sceneManager->keyMapping[key];

		switch (action) {
			//forward backward translation
		case MOVE_CAMERA_POSITIVE_Z:
			sceneManager->m_cameras[sceneManager->activeCamera]->MoveOz(1);
			break;
		case MOVE_CAMERA_NEGATIVE_Z:
			sceneManager->m_cameras[sceneManager->activeCamera]->MoveOz(-1);
			break;
			//left right translation
		case MOVE_CAMERA_NEGATIVE_X:
			sceneManager->m_cameras[sceneManager->activeCamera]->MoveOx(-1);
			break;
		case MOVE_CAMERA_POSITIVE_X:
			sceneManager->m_cameras[sceneManager->activeCamera]->MoveOx(1);
			break;
			//Oy translation
		case MOVE_CAMERA_NEGATIVE_Y:
			sceneManager->m_cameras[sceneManager->activeCamera]->MoveOy(-1);
			break;
		case MOVE_CAMERA_POSITIVE_Y:
			sceneManager->m_cameras[sceneManager->activeCamera]->MoveOy(1);
			break;
		case ROTATE_CAMERA_OX_POSITIVE:
			sceneManager->m_cameras[sceneManager->activeCamera]->RotateOx(1);
			break;
		case ROTATE_CAMERA_OX_NEGATIVE:
			sceneManager->m_cameras[sceneManager->activeCamera]->RotateOx(-1);
			break;
		case ROTATE_CAMERA_OY_POSITIVE:
			sceneManager->m_cameras[sceneManager->activeCamera]->RotateOy(1);
			break;
		case ROTATE_CAMERA_OY_NEGATIVE:
			sceneManager->m_cameras[sceneManager->activeCamera]->RotateOy(-1);
			break;
		case ROTATE_CAMERA_OZ_POSITIVE:
			sceneManager->m_cameras[sceneManager->activeCamera]->RotateOz(1); // counter clockwise
			break;
		case ROTATE_CAMERA_OZ_NEGATIVE:
			sceneManager->m_cameras[sceneManager->activeCamera]->RotateOz(-1); //clockwise
			break;
		default:
			std::cout << "Key pressed: " << key;
			break;
		}
	}
}

void MouseButton(ESContext *esContext, mouse::MouseButton button, mouse::MouseEvent mouseEvent, int xCoord, int yCoord)
{
	switch (button)
	{
		case mouse::LEFT_CLICK:
			if (xCoord < (Globals::screenWidth / 2.f))
			{
				angle += 0.1f;
			}
			else
			{
				angle -= 0.1f;
			}
	}
}

void CleanUp()
{
	glDeleteBuffers(1, &vboId);
	glDeleteBuffers(1, &terrainModelVboId);
	glDeleteBuffers(1, &eboId);
	glDeleteBuffers(1, &terrainModelEboId);
	glDeleteBuffers(1, &lineVboId);
}

int _tmain(int argc, _TCHAR* argv[])
{
	//identifying memory leaks
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF ); 

	ESContext esContext;

    esInitContext ( &esContext );

	sceneManager = SceneManager::GetInstance();
	sceneManager->InitializeWindow();

	esCreateWindow ( &esContext, sceneManager->m_windowSettings.name.c_str(), Globals::screenWidth, Globals::screenHeight, ES_WINDOW_RGB | ES_WINDOW_DEPTH);

	if ( Init ( &esContext ) != 0 )
		return 0;

	esRegisterDrawFunc ( &esContext, Draw );
	esRegisterUpdateFunc ( &esContext, Update );
	esRegisterKeyFunc ( &esContext, Key);
	esRegisterButtonFunc(&esContext, MouseButton);

	esMainLoop ( &esContext );

	//releasing OpenGL resources
	CleanUp();


	printf("Press any key...\n");
	_getch();

	return 0;
}

