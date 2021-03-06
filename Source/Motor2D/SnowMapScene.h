#ifndef __MENUSCENE_H__
#define __MENUSCENE_H__

#include "j1Module.h"
#include "Scene.h"

class SnowScene : public Scene
{
public:

	SnowScene();									// Constructor
	~SnowScene();									// Destructor

	bool Awake(pugi::xml_node& config);				// Called before render is available
	bool Start();									// Called before the first frame
	bool PreUpdate();								// Called before all Updates
	bool Update(float dt);							// Called each loop iteration
	bool PostUpdate();								// Called before all Updates
	bool CleanUp();									// Called before quitting



public:
	//void InitScene();								// Method that will be used to initialize all scene elements.

	//void DrawScene();								// Method that will draw all scene elements on screen.
	//SDL_Texture* SceneToTexture();				// Method that will be used to get a texture from the current screen.

	void ExecuteTransition();						// Method that will trigger a new transition depending on the input received.

public:

	j1Element* mainMenuButton = nullptr;
	j1Element* newGameButton = nullptr;
	j1Element* continueButton = nullptr;
	j1Element* settingsButton = nullptr;
	j1Element* exitButton = nullptr;

};

#endif // __FIRSTSCENE_H_