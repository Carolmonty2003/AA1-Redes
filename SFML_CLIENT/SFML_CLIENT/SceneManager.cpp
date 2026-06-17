#include "SceneManager.h"

bool SceneManager::AddScene(std::string name, Scene* scene) {
	//Añade escena a la lista de escenas
	if (scenes.find(name) == scenes.end()) {
		scenes.emplace(name, scene);
		return true;
	}
	return false;
}

Scene* SceneManager::GetScene(std::string name) {
	//Obtiene una escena por su nombre si existe
	if (scenes.find(name) != scenes.end()) {
		return scenes[name];
	}
	return nullptr;
}

bool SceneManager::InitFirstScene(std::string name) {
	//Inicia la primera escena
	if (scenes.find(name) != scenes.end()) {
		currentScene = scenes[name];
		currentScene->OnEnter();
		return true;
	}
	return false;
}

bool SceneManager::SetNextScene(std::string name) {
	//Si existe, prepara cambio de escena
	if (scenes.find(name) == scenes.end()) {
		return false;
	}
	nextScene = name;
	return true;
}

void SceneManager::UpdateCurrentScene(float dt) {
	//Actualiza/cambia de escena
	if (nextScene != "") {
		if (currentScene)
			currentScene->OnExit();
		std::cout << "Exiting from:  " << currentScene << ", netxt scene: " << scenes[nextScene] << std::endl;
		currentScene = scenes[nextScene];
		if (currentScene)
			currentScene->OnEnter();
		nextScene = "";
	}
	if (currentScene) currentScene->Update(dt);
}
