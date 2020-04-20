#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1UIElements.h"
#include "j1UI_manager.h"
#include "j1SceneManager.h"
#include "MenuScene.h"
#include "TransitionManager.h"
#include "j1Audio.h"

MenuScene::MenuScene() : Scene(SCENES::MENU_SCENE)
{

}

// Destructor
MenuScene::~MenuScene()
{

}

// Called before render is available
bool MenuScene::Awake(pugi::xml_node& config)
{
	LOG("Loading MenuScene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool MenuScene::Start()
{
	bool ret = true;

	backgroundImage = App->gui->AddElement(TypeOfUI::GUI_IMAGE, nullptr, { 0, 0 }, { 0,0 }, true, true, { 0, 0, App->win->width, App->win->width }, nullptr, App->scenes, TEXTURE::MAIN_IMAGE);

	playButton = App->gui->AddElement(TypeOfUI::GUI_BUTTON, nullptr, { 475, 100 }, { 70,25}, true, true, { 0, 0, 263, 91 }, "PLAY", App->scenes, TEXTURE::ATLAS);
	playButton->hover_rect = { 263, 0, 263, 91 };
	playButton->click_rect = { 526, 0, 263, 91 };

	settingsButton = App->gui->AddElement(TypeOfUI::GUI_BUTTON, nullptr, { 490, 230 }, { 3,20 }, true, true, { 0, 334, 234, 79 }, "Settings", App->scenes, TEXTURE::ATLAS);
	settingsButton->hover_rect = { 263, 334, 234, 79 };
	settingsButton->click_rect = { 525, 334, 234, 79 };

	scoreButton = App->gui->AddElement(TypeOfUI::GUI_BUTTON, nullptr, { 490, 340 }, { 50,20 }, true, true, { 0, 334, 234, 79 }, "Score", App->scenes, TEXTURE::ATLAS);
	scoreButton->hover_rect = { 263, 334, 234, 79 };
	scoreButton->click_rect = { 525, 334, 234, 79 };

	creditsButton = App->gui->AddElement(TypeOfUI::GUI_BUTTON, nullptr, { 490, 450 }, { 25,20 }, true, true, { 0, 334, 234, 79 }, "Credits", App->scenes, TEXTURE::ATLAS);
	creditsButton->hover_rect = { 263, 334, 234, 79 };
	creditsButton->click_rect = { 525, 334, 234, 79 };

	exitButton = App->gui->AddElement(TypeOfUI::GUI_BUTTON, nullptr, { 505, 570 }, { 50,15 }, true, true, { 0, 658, 207, 71 }, "Exit", App->scenes, TEXTURE::ATLAS);
	exitButton->hover_rect = { 263, 658, 207, 71 };
	exitButton->click_rect = { 525, 658, 207, 71 };

	if (App->audio->PlayingMenuMusic != true) {
		App->audio->PlayMusic("audio/music/MainMenu_Music.ogg");
		App->audio->PlayingMenuMusic = true;
	}

	return ret;
}

// Called each loop iteration
bool MenuScene::PreUpdate()
{
	bool ret = true;

	return ret;
}

// Called each loop iteration
bool MenuScene::Update(float dt)
{
	bool ret = true;
	
	CameraDebugMovement(dt);

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		App->scenes->SwitchScene(SCENES::START_SCENE);
	}




	return ret;
}

// Called each loop iteration
bool MenuScene::PostUpdate()
{
	bool ret = true;

	if ( exit)
		ret = false;

	//ExecuteTransition();

	return ret;
}

// Called before quitting
bool MenuScene::CleanUp()
{
	LOG("Freeing Scene");
	bool ret = true;

	playButton->CleanUp();
	scoreButton->CleanUp();
	creditsButton->CleanUp();
	settingsButton->CleanUp();
	exitButton->CleanUp();
	backgroundImage->CleanUp();

	if (scene_texture != nullptr)
	{
		App->tex->UnLoad(scene_texture);
	}

	if (tileset_texture != nullptr)
	{
		App->tex->UnLoad(tileset_texture);
	}

	if (SDL_RenderClear(scene_renderer) == 0)
	{
		scene_renderer = nullptr;
	}

	if (scene_surface != nullptr)
	{
		SDL_FreeSurface(scene_surface);
	}

	return ret;
}


void MenuScene::GUI_Event_Manager(GUI_Event type, j1UIElement* element)
{
	if (element == playButton && type == GUI_Event::EVENT_ONCLICK)
	{
		//App->transition_manager->CreateFadeToColour(SCENES::START_SCENE);
		//App->transition_manager->CreateFadeToColour(SCENES::START_SCENE);
		App->transition_manager->CreateSlide(SCENES::START_SCENE, 0.5f, true);
	}

	if (element == settingsButton && type == GUI_Event::EVENT_ONCLICK)
	{
		App->transition_manager->CreateFadeToColour(SCENES::SETTINGS_SCENE);
		
	}

	if (element == exitButton && type == GUI_Event::EVENT_ONCLICK)
	{
		App->scenes->exit = true;
	}

	if (element == creditsButton && type == GUI_Event::EVENT_ONCLICK) {
		ShellExecuteA(NULL, "open", "https://github.com/Needlesslord/BrainDeadStudios", NULL, NULL, SW_SHOWNORMAL);
	}
}


//void MenuScene::InitScene()
//{
//	tileset_texture = App->tex->Load("maps/tiles_first_map.png", scene_renderer);	// This texture will be used SceneToTexture(). Needed to get a single whole texture of the map.
//
//	App->map->GetMapSize(map_width, map_height);
//	App->map->GetTileOffset(x_offset, y_offset);
//	
//	App->render->camera.x = map_width * 0.3f;										// This camera position gets the camera close to the center of the map.
//	App->render->camera.y = -40;
//
//	// --- TRANSITIONS WITH TEXTURE
//	/*App->render->camera.x = map_width * 0.5f;										// This camera position is to have the renderer render all the scene_texture.
//	App->render->camera.y = 0;
//
//	SceneToTexture();
//
//	App->render->camera.x = map_width * 0.3f;										// This camera position gets the camera close to the center of the map.
//	App->render->camera.y = -40;*/
//}

//void MenuScene::DrawScene()
//{
//	App->map->Draw();
//
//
//	// --- TRANSITIONS WITH TEXTURE
//	/*if (scene_texture != nullptr)
//	{
//		App->render->Blit(scene_texture, -(map_width) * 0.5f, 0, NULL);
//	}*/	
//}


//SDL_Texture* MenuScene::SceneToTexture()
//{
//	App->render->CreateSubRenderer(map_width + x_offset, map_height + y_offset, scene_surface, scene_renderer);		// Both scene_surface and scene renderer are passed by reference.
//
//	tileset_texture = App->tex->Load("maps/tiles_first_map.png", scene_renderer);
//	App->map->DrawToSubRenderer(scene_renderer, tileset_texture);
//
//	scene_texture = SDL_CreateTextureFromSurface(App->render->renderer, scene_surface);
//
//	return scene_texture;
//}


void MenuScene::ExecuteTransition()
{
	if (!App->transition_manager->is_transitioning)
	{
		if (App->input->GetKey(SDL_SCANCODE_H) == KEY_DOWN)
		{
			App->transition_manager->CreateCut(SCENES::GAME_SCENE);
		}

		if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
		{
			App->transition_manager->CreateFadeToColour(SCENES::GAME_SCENE);
		}

			if (App->input->GetKey(SDL_SCANCODE_0) == KEY_DOWN)
			{
				App->transition_manager->CreateSlide(SCENES::GAME_SCENE, 0.5f, true);
			}

			if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN)
			{
				//App->transition_manager->CreateSlide(SCENES::SECOND_SCENE, 0.5f, true, true);
			}

			if (App->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN)
			{
				//App->transition_manager->CreateWipe(SCENES::SECOND_SCENE, 0.5f, true);
			}

			if (App->input->GetKey(SDL_SCANCODE_6) == KEY_DOWN)
			{
				//App->transition_manager->CreateWipe(SCENES::SECOND_SCENE, 0.5f, true, true);
			}

			if (App->input->GetKey(SDL_SCANCODE_7) == KEY_DOWN)
			{
				//App->transition_manager->CreateAlternatingBars(SCENES::SECOND_SCENE, 0.5f, true);
			}

		//	if (App->input->GetKey(SDL_SCANCODE_8) == KEY_DOWN)
		//	{
		//		App->transition_manager->CreateExpandingBars(SCENES::SECOND_SCENE, 0.5f, true);
		//	}

		//	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN)
		//	{
		//		iPoint mouse_pos = App->input->GetMouseToWorld();

		//		App->transition_manager->CreateZoomToMouse(SCENES::SECOND_SCENE, mouse_pos, 0.5f, true);
		//	}

		//	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
		//	{
		//		iPoint mouse_pos = App->input->GetMouseToWorld();

		//		App->transition_manager->CreateCameraToMouse(mouse_pos, 0.5f, true);
		//	}


		//	// --- TRANSITION WITH TEXTURE METHODS (NOT IMPLEMENTED)
		//	if (App->input->GetKey(SDL_SCANCODE_9) == KEY_DOWN)
		//	{
		//		if (scene_texture != nullptr)
		//		{
		//			App->transition_manager->CreateDissolve(SCENES::SECOND_SCENE, 1.0f);
		//		}
		//	}

		//	if (App->input->GetMouseButtonDown(SDL_BUTTON_MIDDLE) == KEY_DOWN)
		//	{
		//		iPoint mouse_pos = App->input->GetMouseToWorld();

		//		if (scene_texture != nullptr)
		//		{
		//			App->transition_manager->CreateZoomToTexture(SCENES::SECOND_SCENE, mouse_pos);
		//		}
		//	}
		}
	}