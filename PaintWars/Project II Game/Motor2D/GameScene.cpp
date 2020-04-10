#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1SceneManager.h"
#include "j1EntityManager.h"
#include "j1Pathfinding.h"
#include "GameScene.h"
#include "TransitionManager.h"
#include "j1GUIELements.h"
#include "j1GUI.h"

GameScene::GameScene() : Scene(SCENES::GAME_SCENE)
{

}

// Destructor
GameScene::~GameScene()
{

}

// Called before render is available
bool GameScene::Awake(pugi::xml_node& config)
{
	LOG("Loading FirstScene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool GameScene::Start()
{
	bool ret = true;
	
	debug_tex = App->tex->Load("maps/path2.png");

	//THESE BOOLS HAVE TO BE REMOVED ONCE WE HAVE THE MAIN MENU, BECAUSE WE WANT THE GAME TO LOAD THE MAP AFTER WE USE THE PLAY BUTTON NOT WHILE WE ARE IN THE MENU
	Load_Forest_Map = true;
	Change_Map = true;
	Map_Manager();


	App->entities->AddEntity(ENTITY_TYPE_TOWN_HALL,	{    0,  100 }, App->entities, nullptr, 10, true);
	App->entities->AddEntity(ENTITY_TYPE_PAINTER,	{  200,  200 }, App->entities, nullptr,  5, true);

	App->entities->AddEntity(ENTITY_TYPE_WARRIOR,	{  400,  400 }, App->entities, nullptr, 10, true);
	App->entities->AddEntity(ENTITY_TYPE_WARRIOR,	{  700,  600 }, App->entities, nullptr,  0, true);
	App->entities->AddEntity(ENTITY_TYPE_WARRIOR,	{ 1000,  800 }, App->entities, nullptr,  0, true);

	App->entities->AddEntity(ENTITY_TYPE_SLIME,		{  600,  200 }, App->entities, nullptr, true);
	App->entities->AddEntity(ENTITY_TYPE_SPAWNER,	{ 1000, 1000 }, App->entities, nullptr, true);
	

	int w, h;
	uchar* data = NULL;
	if (App->map->CreateWalkabilityMap(w, h, &data))
	{
		App->pathfinding->SetMap(w, h, data);						// Sets a new walkability map with the map passed by CreateWalkabilityMap().
	}

	//App->pathfinding->ChangeWalkability({ 7, 0 }, false);
	//App->pathfinding->ChangeWalkability({ 7, 1 }, false);
	//App->pathfinding->ChangeWalkability({ 7, 2 }, false);
	//App->pathfinding->ChangeWalkability({ 7, 3 }, false);


	//////////////////
	//      UI      //
	//////////////////

	//HUD - Bar
	hudBarImage = App->gui->AddElement(GUItype::GUI_IMAGE, nullptr, { 15 , 5 }, { 0 , 0 }, false, true, { 0, 1353, 984, 35 }, nullptr, nullptr, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::ATLAS);

	//HUD - Quests
	questsImage = App->gui->AddElement(GUItype::GUI_IMAGE, nullptr, { 15 , 50 }, { 0 , 0 }, false, true, { 0, 1388, 263, 265 }, nullptr, nullptr, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::ATLAS);
	questsLabel = App->gui->AddElement(GUItype::GUI_LABEL, questsImage , { 15 , 52 }, { 2 , 2 }, false, true, { 0, 0, 0, 0 }, "QUESTS", nullptr, false,false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::ATLAS, FONT::FONT_SMALL_WHITE);
	questsOpenButton = App->gui->AddElement(GUItype::GUI_BUTTON, questsImage, { 215, 250 }, { 200,200 }, true, true, { 317, 1388, 54, 55}, nullptr, App->scenes, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::ATLAS, FONT::FONT_MEDIUM, 2);
	questsOpenButton->hover_rect = { 317, 1443, 54, 54 };
	questsOpenButton->click_rect = { 317, 1497, 54, 54 };
	questsCloseButton = App->gui->AddElement(GUItype::GUI_BUTTON, questsImage, { 215, 520 }, { 200,200 }, true, false, { 263, 1388, 54, 55 }, nullptr, App->scenes, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::ATLAS, FONT::FONT_MEDIUM, 2);
	questsCloseButton->hover_rect = { 263, 1443, 54, 54 };
	questsCloseButton->click_rect = { 263, 1497, 54, 54 };

	//HUD - Buttons
	homeButton = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { 670, 50 }, { 0,0 }, true, true, { 785, 486, 74, 74 }, nullptr, App->scenes, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::ATLAS);
	homeButton->hover_rect = { 785, 560, 74, 74 };
	homeButton->click_rect = { 785, 634, 74, 74 };
	shopButton = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { 750, 50 }, { 0,0 }, true, true, { 785, 243, 74, 73 }, nullptr, App->scenes, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::ATLAS);
	shopButton->hover_rect = { 785, 316, 74, 73 };
	shopButton->click_rect = { 785, 389, 74, 73 };
	pauseMenuButton = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { 830, 50 }, { 0,0 }, true, true, { 933, 243, 74, 73 }, nullptr, App->scenes, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::ATLAS);
	pauseMenuButton->hover_rect = { 932, 316, 74, 73 };
	pauseMenuButton->click_rect = { 933, 389, 74, 73 };
	restartButton = App->gui->AddElement(GUItype::GUI_BUTTON, nullptr, { 910, 50 }, { 0,0 }, true, true, { 859, 486, 74, 73 }, nullptr, App->scenes, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::ATLAS);
	restartButton->hover_rect = { 859, 560, 74, 73 };
	restartButton->click_rect = { 859, 634, 74, 73 };

	//HUD - MiniMap
	miniMapImage = App->gui->AddElement(GUItype::GUI_IMAGE, nullptr, { 750 , 510 }, { 0 , 0 }, false, true, { 0, 1388, 263, 240 }, nullptr, nullptr, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::ATLAS);
	
	//Pause Menu
	pauseMenuImage = App->gui->AddElement(GUItype::GUI_IMAGE, nullptr, { 200 , 100 }, { 0 , 0 }, false, false, { 263, 729, 452, 623 }, nullptr, nullptr, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::ATLAS, FONT::FONT_MEDIUM, 5);
	pauseMenuLabel = App->gui->AddElement(GUItype::GUI_LABEL, pauseMenuImage, { 350 , 130 }, { 2 , 2 }, false, false, { 0, 0, 0, 0 }, "PAUSE", nullptr, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::ATLAS, FONT::FONT_MEDIUM, 6);
	resumeButton = App->gui->AddElement(GUItype::GUI_BUTTON, pauseMenuImage, { 310, 260 }, { 30,20 }, true, false, { 0, 91, 234, 80 }, "RESUME", App->scenes, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::ATLAS, FONT::FONT_MEDIUM, 6);
	resumeButton->hover_rect = { 263, 91, 234, 80 };
	resumeButton->click_rect = { 525, 91, 234, 80 };
	saveButton = App->gui->AddElement(GUItype::GUI_BUTTON, pauseMenuImage, { 325, 350 }, { 50,15 }, true, false, { 0, 415, 207, 71 }, "Save", App->scenes, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::ATLAS, FONT::FONT_MEDIUM, 6);
	saveButton->hover_rect = { 263, 415, 207, 71 };
	saveButton->click_rect = { 525, 415, 207, 71 };
	settingsButton = App->gui->AddElement(GUItype::GUI_BUTTON, pauseMenuImage, { 325, 430 }, { 15,20 }, true, false, { 0, 415, 207, 71 }, "Settings", App->scenes, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::ATLAS, FONT::FONT_SMALL, 6);
	settingsButton->hover_rect = { 263, 415, 207, 71 };
	settingsButton->click_rect = { 525, 415, 207, 71 };
	mainMenuButton = App->gui->AddElement(GUItype::GUI_BUTTON, pauseMenuImage, { 340, 520 }, { 30,15 }, true, false, { 1106, 359, 166, 56 }, "Title", App->scenes, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::ATLAS, FONT::FONT_SMALL, 6);
	mainMenuButton->hover_rect = { 1272, 359, 165, 56 };
	mainMenuButton->click_rect = { 1437, 359, 166, 56 };
	exitButton = App->gui->AddElement(GUItype::GUI_BUTTON, pauseMenuImage, { 340, 590 }, { 40,15 }, true, false, { 1106, 359, 166, 56 }, "EXIT", App->scenes, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::ATLAS, FONT::FONT_SMALL, 6);
	exitButton->hover_rect = { 1272, 359, 165, 56 };
	exitButton->click_rect = { 1437, 359, 166, 56 };

	//Pause Menu - Settings Menu
	musicLabel = App->gui->AddElement(GUItype::GUI_LABEL, pauseMenuImage, { 255, 260 }, { 0, 0 }, false, false, { 0, 0, 0, 0 }, "Mus", App->scenes, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::ATLAS, FONT::FONT_SMALL, 6);
	vfxLabel = App->gui->AddElement(GUItype::GUI_LABEL, pauseMenuImage, { 255, 330 }, { 0, 0 }, false, false, { 0, 0, 0, 0 }, "VFX", App->scenes, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::ATLAS, FONT::FONT_SMALL, 6);
	fullscreenLabel = App->gui->AddElement(GUItype::GUI_LABEL, pauseMenuImage, { 255, 400 }, { 0, 0 }, false, false, { 0, 0, 0, 0 }, "Fullscr", App->scenes, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::ATLAS, FONT::FONT_SMALL, 6);
	gpadLabel = App->gui->AddElement(GUItype::GUI_LABEL, pauseMenuImage, { 255, 470 }, { 0, 0 }, false, false, { 0, 0, 0, 0 }, "GamePad", App->scenes, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::ATLAS, FONT::FONT_SMALL, 6);
	musicScroll = App->gui->AddElement(GUItype::GUI_SCROLLBAR, pauseMenuImage, { 335, 260 }, { 20, -3 }, true, false, { 786, 59, 268, 25 }, nullptr, App->audio, true, false, SCROLL_TYPE::SCROLL_MUSIC, true, TEXTURE::ATLAS);
	musicScroll->Button->rect = { 786, 0, 42, 35 };
	musicScroll->Button->hover_rect = { 786, 0, 42, 35 };
	musicScroll->Button->click_rect = { 786, 0, 42, 35 };
	vfxScroll = App->gui->AddElement(GUItype::GUI_SCROLLBAR, pauseMenuImage, { 335, 330 }, { 20, -3 }, true, false, { 786, 59, 268, 25 }, nullptr, App->audio, true, false, SCROLL_TYPE::SCROLL_MUSIC, true, TEXTURE::ATLAS, FONT::FONT_MEDIUM, 6);
	vfxScroll->Button->rect = { 786, 0, 42, 35 };
	vfxScroll->Button->hover_rect = { 786, 0, 42, 35 };
	vfxScroll->Button->click_rect = { 786, 0, 42, 35 };
	fullscreenButton = App->gui->AddElement(GUItype::GUI_BUTTON, pauseMenuImage, { 435, 390 }, { 0,0 }, true, false, { 0, 1031, 182, 58 }, nullptr, App->scenes, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::ATLAS, FONT::FONT_MEDIUM, 6);
	fullscreenButton->hover_rect = { 0, 1031, 182, 58 };
	fullscreenButton->click_rect = { 0, 1031, 182, 58 };
	gpadButton = App->gui->AddElement(GUItype::GUI_BUTTON, pauseMenuImage, { 435, 460 }, { 0,0 }, true, false, { 0, 1031, 182, 58 }, nullptr, App->scenes, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::ATLAS, FONT::FONT_MEDIUM, 6);
	gpadButton->hover_rect = { 0, 1031, 182, 58 };
	gpadButton->click_rect = { 0, 1031, 182, 58 };
	resetButton = App->gui->AddElement(GUItype::GUI_BUTTON, pauseMenuImage, { 335, 530 }, { 30,15 }, true, false, { 1106, 359, 166, 56 }, "Reset", App->scenes, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::ATLAS, FONT::FONT_SMALL, 6);
	resetButton->hover_rect = { 1272, 359, 165, 56 };
	resetButton->click_rect = { 1437, 359, 166, 56 };
	backButton = App->gui->AddElement(GUItype::GUI_BUTTON, pauseMenuImage, { 455, 610 }, { 40,15 }, true, false, { 1106, 359, 166, 56 }, "BACK", App->scenes, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::ATLAS, FONT::FONT_SMALL, 6);
	backButton->hover_rect = { 1272, 359, 165, 56 };
	backButton->click_rect = { 1437, 359, 166, 56 };

	// Exit / Restart / Main Menu Menu
	exitMenuImage = App->gui->AddElement(GUItype::GUI_IMAGE, nullptr, { 200 , 200 }, { 0 , 0 }, false, false, { 787, 729, 490, 336 }, nullptr, nullptr, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::ATLAS, FONT::FONT_MEDIUM, 5);
	exitMenuLabel = App->gui->AddElement(GUItype::GUI_LABEL, exitMenuImage, { 350 , 215 }, { 2 , 2 }, false, false, { 0, 0, 0, 0 }, "EXIT", nullptr, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::ATLAS, FONT::FONT_MEDIUM, 6);
	exitTextLabel = App->gui->AddElement(GUItype::GUI_LABEL, exitMenuImage, { 280 , 350}, { 2 , 2 }, false, false, { 0, 0, 0, 0 }, "Are you sure", nullptr, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::ATLAS, FONT::FONT_MEDIUM, 6);
	yesButton = App->gui->AddElement(GUItype::GUI_BUTTON, exitMenuImage, { 270, 430 }, { 50,15 }, true, false, { 1106, 243, 165, 57 }, "YES", App->scenes, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::ATLAS, FONT::FONT_SMALL, 6);
	yesButton->hover_rect = { 1272, 243, 165, 57 };
	yesButton->click_rect = { 1437, 243, 166, 57 };
	noButton = App->gui->AddElement(GUItype::GUI_BUTTON, exitMenuImage, { 470, 430 }, { 60,15 }, true, false, { 1106, 359, 165, 57 }, "NO", App->scenes, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::ATLAS, FONT::FONT_SMALL, 6);
	noButton->hover_rect = { 1272, 359, 165, 57 };
	noButton->click_rect = { 1437, 359, 166, 57 };

	// Shop
	shopImage = App->gui->AddElement(GUItype::GUI_IMAGE, nullptr, { 720 , 130 }, { 0 , 0 }, false, false, { 0, 1388, 263, 265 }, nullptr, nullptr, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::ATLAS);
	shopLabel = App->gui->AddElement(GUItype::GUI_LABEL, shopImage, { 722 , 132 }, { 2 , 2 }, false, false, { 0, 0, 0, 0 }, "SHOP", nullptr, false, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::ATLAS, FONT::FONT_SMALL_WHITE);

	return ret;
}

// Called each loop iteration
bool GameScene::PreUpdate()
{
	bool ret = true;

	if (Change_Map == true) {
		if (Load_Forest_Map) {
			Create_Forest_Map();
		}
		else if (Load_Volcano_Map) {
			Create_Volcano_Map();
		}
		else if (Load_Snow_Map) {
			Create_Snow_Map();
		}
		else {}
	}



	// Debug pathfinding ------------------
	static iPoint origin;
	static bool origin_selected = false;

	fPoint mouseWP = App->input->GetMouseWorldPosition();
	iPoint p = App->map->WorldToMap(mouseWP.x, mouseWP.y);

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		if (origin_selected == true)
		{
			//App->pathfinding->CreatePath(origin, p);                  CRASHES WITH THE NEW PATHFINDING
			origin_selected = false;
		}
		else
		{
			origin = p;
			origin_selected = true;
		}
	}



	return ret;
}

// Called each loop iteration
bool GameScene::Update(float dt)
{
	bool ret = true;
	
	CameraDebugMovement(dt);

	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		App->LoadGame();

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		App->SaveGame("save_game.xml");

	if (App->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN) {
		float a, b;
		App->input->GetMousePosition(a, b);
		fPoint cd = App->render->ScreenToWorld(a, b);
		float c, d;
		c = cd.x;
		d = cd.y;

		App->entities->AddEntity(ENTITY_TYPE_TOWN_HALL, { c, d }, App->entities);
	}

	/*if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN) {
		iPoint mouse_pos = App->input->GetMousePositionWorld();

		App->render->camera.x = App->player->mouse_position.x;

	}*/

	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) {
		float a, b;
		App->input->GetMousePosition(a, b);
		fPoint cd = App->render->ScreenToWorld(a, b);
		float c, d;
		c = cd.x;
		d = cd.y;

		App->entities->AddEntity(ENTITY_TYPE_PAINTER, { c, d }, App->entities);
	}

	App->map->Draw();

	fPoint xy = App->input->GetMouseWorldPosition();
	iPoint cameraW = App->map->WorldToMap(App->render->camera.x, App->render->camera.y);
	iPoint map_coordinates = App->map->WorldToMap(xy.x - cameraW.x /*+ App->map->data.tile_width / 2*/, xy.y - cameraW.y + App->map->data.tile_height/2);

	static char title[256];
	sprintf_s(title, 256, "Map:%dx%d Tiles:%dx%d Tilesets:%d Tile:%d,%d,",
		App->map->data.width, App->map->data.height,
		App->map->data.tile_width, App->map->data.tile_height,
		App->map->data.tilesets.size(),
		map_coordinates.x, map_coordinates.y);

	App->win->SetTitle(title);


	/*p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d Tile:%d,%d, %d, %d",
		App->map->data.width, App->map->data.height,
		App->map->data.tile_width, App->map->data.tile_height,
		App->map->data.tilesets.size(),
		map_coordinates.x, map_coordinates.y, map_coordinates2.x, map_coordinates2.y);

	App->win->SetTitle(title.GetString());*/
	
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {
		debugTile = !debugTile;
	}
	// Debug pathfinding ------------------------------
	if (debugTile) {
		fPoint c = App->map->MapToWorld(map_coordinates.x, map_coordinates.y);
		App->render->AddBlitEvent(1, debug_tex, c.x, c.y, { 0,0,150,75 });
	}



	//UI
	for (int i = 0; i < App->gui->GUI_ELEMENTS.count(); i++)
	{
		App->gui->GUI_ELEMENTS[i]->map_position.x = App->gui->GUI_ELEMENTS[i]->init_map_position.x + App->render->camera.x;
		App->gui->GUI_ELEMENTS[i]->map_position.y = App->gui->GUI_ELEMENTS[i]->init_map_position.y + App->render->camera.y;
	}


	return ret;
}

// Called each loop iteration
bool GameScene::PostUpdate()
{
	bool ret = true;
	
	if (App->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN) {
		Change_Map = true;
		Load_Snow_Map = true;
	}

	//if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	//	ret = false;

	ExecuteTransition();

	return ret;
}

// Called before quitting
bool GameScene::CleanUp()
{
	LOG("Freeing Scene");
	bool ret = true;

	App->map->CleanUp();



	//UI
	for (int i = 0; i < App->gui->GUI_ELEMENTS.count(); i++)
	{
		App->gui->GUI_ELEMENTS[i]->CleanUp();
	}




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

	App->tex->UnLoad(debug_tex);

	return ret;
}

void GameScene::GUI_Event_Manager(GUI_Event type, j1Element* element)
{
	if (element == questsOpenButton && type == GUI_Event::EVENT_ONCLICK)
	{
		questsImage->rect.h = 535;
		questsCloseButton->enabled = true;
		questsOpenButton->enabled = false;
	}

	if (element == questsCloseButton && type == GUI_Event::EVENT_ONCLICK)
	{
		questsImage->rect.h = 265;
		questsOpenButton->enabled = true;
		questsCloseButton->enabled = false;
	}

	if (element == homeButton && type == GUI_Event::EVENT_ONCLICK)
	{
		App->render->camera.x = 0;
		App->render->camera.y = 0;
	}

	if (element == shopButton && type == GUI_Event::EVENT_ONCLICK)
	{
		shopMenu = !shopMenu;

		if (shopMenu)
		{

		}
		else
		{

		}
	}

	if (element == pauseMenuButton && type == GUI_Event::EVENT_ONCLICK)
	{
		pauseMenu = true;
		pauseMenuImage->enabled = true;
		pauseMenuLabel->enabled = true;
		resumeButton->enabled = true;
		saveButton->enabled = true;
		settingsButton->enabled = true;
		mainMenuButton->enabled = true;
		exitButton->enabled = true;

		homeButton->interactable = false;
		pauseMenuButton->interactable = false;
		shopButton->interactable = false;
		restartButton->interactable = false;
		questsOpenButton->interactable = false;
		questsCloseButton->interactable = false;
	}
	
	if (element == resumeButton && type == GUI_Event::EVENT_ONCLICK)
	{
		pauseMenu = false;
		pauseMenuImage->enabled = false;
		pauseMenuLabel->enabled = false;
		saveButton->enabled = false;
		settingsButton->enabled = false;
		mainMenuButton->enabled = false;
		exitButton->enabled = false;
		resumeButton->enabled = false;

		homeButton->interactable = true;
		pauseMenuButton->interactable = true;
		shopButton->interactable = true;
		restartButton->interactable = true;
		questsOpenButton->interactable = true;
		questsCloseButton->interactable = true;
	}


	if (element == settingsButton && type == GUI_Event::EVENT_ONCLICK)
	{
		pauseMenu = false;
		saveButton->enabled = false;
		settingsButton->enabled = false;
		mainMenuButton->enabled = false;
		exitButton->enabled = false;
		resumeButton->enabled = false;

		settingsMenu = true;
		musicLabel->enabled = true;
		vfxLabel->enabled = true;
		fullscreenLabel->enabled = true;
		gpadLabel->enabled = true;
		musicScroll->enabled = true;
		vfxScroll->enabled = true;
		fullscreenButton->enabled = true;
		gpadButton->enabled = true;
		resetButton->enabled = true;
		backButton->enabled = true;
	}

	if (element == backButton && type == GUI_Event::EVENT_ONCLICK)
	{

		pauseMenu = true;
		resumeButton->enabled = true;
		saveButton->enabled = true;
		settingsButton->enabled = true;
		mainMenuButton->enabled = true;
		exitButton->enabled = true;

		settingsMenu = false;
		musicLabel->enabled = false;
		vfxLabel->enabled = false;
		fullscreenLabel->enabled = false;
		gpadLabel->enabled = false;
		musicScroll->enabled = false;
		vfxScroll->enabled = false;
		fullscreenButton->enabled = false;
		gpadButton->enabled = false;
		resetButton->enabled = false;
		backButton->enabled = false;
	}

	if (element == restartButton && type == GUI_Event::EVENT_ONCLICK)
	{
		restartMenu = true;
		exitMenuImage->enabled = true;
		exitMenuLabel->enabled = true;
		exitMenuLabel->text = "RESTART";
		exitTextLabel->enabled = true;
		yesButton->enabled = true;
		noButton->enabled = true;

		pauseMenu = false;
		pauseMenuImage->enabled = false;
		pauseMenuLabel->enabled = false;
		saveButton->enabled = false;
		settingsButton->enabled = false;
		mainMenuButton->enabled = false;
		exitButton->enabled = false;
		resumeButton->enabled = false;
	}

	if (element == exitButton && type == GUI_Event::EVENT_ONCLICK)
	{
		exitMenu = true;
		exitMenuImage->enabled = true;
		exitMenuLabel->enabled = true;
		exitMenuLabel->text = "EXIT";
		exitTextLabel->enabled = true;
		yesButton->enabled = true;
		noButton->enabled = true;

		pauseMenu = false;
		pauseMenuImage->enabled = false;
		pauseMenuLabel->enabled = false;
		saveButton->enabled = false;
		settingsButton->enabled = false;
		mainMenuButton->enabled = false;
		exitButton->enabled = false;
		resumeButton->enabled = false;
	}

	if (element == mainMenuButton && type == GUI_Event::EVENT_ONCLICK)
	{
		mainMenu = true;
		exitMenuImage->enabled = true;
		exitMenuLabel->enabled = true;
		exitMenuLabel->text = "TITLE";
		exitTextLabel->enabled = true;
		yesButton->enabled = true;
		noButton->enabled = true;

		pauseMenu = false;
		pauseMenuImage->enabled = false;
		pauseMenuLabel->enabled = false;
		saveButton->enabled = false;
		settingsButton->enabled = false;
		mainMenuButton->enabled = false;
		exitButton->enabled = false;
		resumeButton->enabled = false;
	}

	if (element == yesButton && type == GUI_Event::EVENT_ONCLICK)
	{
		if (exitMenu)
			App->scenes->exit = true;

		if (restartMenu)
		{

		}

		if (mainMenu)
			App->scenes->SwitchScene(SCENES::MENU_SCENE);
	}

	if (element == noButton && type == GUI_Event::EVENT_ONCLICK)
	{
		if (exitMenu)
		{
			exitMenu = false;

			pauseMenu = true;
			pauseMenuImage->enabled = true;
			pauseMenuLabel->enabled = true;
			resumeButton->enabled = true;
			saveButton->enabled = true;
			settingsButton->enabled = true;
			mainMenuButton->enabled = true;
			exitButton->enabled = true;
		}

		if (restartMenu)
			restartMenu = false;

		if (mainMenu)
		{
			mainMenu = false;

			pauseMenu = true;
			pauseMenuImage->enabled = true;
			pauseMenuLabel->enabled = true;
			resumeButton->enabled = true;
			saveButton->enabled = true;
			settingsButton->enabled = true;
			mainMenuButton->enabled = true;
			exitButton->enabled = true;
		}
	
		exitMenuImage->enabled = false;
		exitMenuLabel->enabled = false;
		exitTextLabel->enabled = false;
		yesButton->enabled = false;
		noButton->enabled = false;
	}

	if (element == shopButton && type == GUI_Event::EVENT_ONCLICK)
	{
		shopImage->enabled = !shopImage->enabled;
		shopLabel->enabled = !shopLabel->enabled;
	}

}

void GameScene::InitScene()
{
	//tileset_texture = App->tex->Load("maps/tiles_first_map.png", scene_renderer);	// This texture will be used SceneToTexture(). Needed to get a single whole texture of the map.

	App->map->Load("map_forest.tmx") == true;
	Create_Forest_Map();
	//App->map->GetMapSize(map_width, map_height);
	//App->map->GetTileOffset(x_offset, y_offset);
	
	//App->render->camera.x = map_width * 0.3f;										// This camera position gets the camera close to the center of the map.
	//App->render->camera.y = -40;

	// --- TRANSITIONS WITH TEXTURE
	/*App->render->camera.x = map_width * 0.5f;										// This camera position is to have the renderer render all the scene_texture.
	App->render->camera.y = 0;

	SceneToTexture();

	App->render->camera.x = map_width * 0.3f;										// This camera position gets the camera close to the center of the map.
	App->render->camera.y = -40;*/
}

void GameScene::DrawScene()
{
	App->map->Draw();


	// --- TRANSITIONS WITH TEXTURE
	/*if (scene_texture != nullptr)
	{
		App->render->Blit(scene_texture, -(map_width) * 0.5f, 0, NULL);
	}*/	
}

void GameScene::Map_Manager() {
	////THESE FUNCTION HAS TO BE REMOVED ONCE WE HAVE THE MAIN MENU, BECAUSE WE WANT THE GAME TO LOAD THE MAP AFTER WE USE THE PLAY BUTTON NOT WHILE WE ARE IN THE MENU
	if (Load_Forest_Map) {
		App->map->Load("map_forest.tmx") == true;
		Create_Forest_Map();
	}
	else if (Load_Volcano_Map) {
		App->map->Load("map_forest.tmx") == true;
		Create_Volcano_Map();
	}
	else if (Load_Snow_Map) {
		App->map->Load("map_snow.tmx") == true;
		Create_Snow_Map();
	}
	else {
		LOG("No map has been loaded in the scene");
	}
}

void GameScene::Create_Forest_Map()
{

	App->map->CleanUp();
	App->map->Load("map_forest.tmx") == true;

	Change_Map = false;
	Forest_Map_Active = true;
	Load_Forest_Map = false;
}

void GameScene::Create_Snow_Map()
{
	App->map->CleanUp();
	App->map->Load("map_snow.tmx") == true;

	App->entities->AddEntity(ENTITY_TYPE_TOWN_HALL, { -200, 300 }, App->entities, nullptr, 10); //TESTING ONLY

	Change_Map = false;
	Snow_Map_Active = true;
	Load_Snow_Map = false;
}

void GameScene::Create_Volcano_Map()
{
	App->map->CleanUp();
	App->map->Load("map_forest.tmx") == true;


	Change_Map = false;
	Volcano_Map_Active = true;
	Load_Volcano_Map = false;
}


//SDL_Texture* FirstScene::SceneToTexture()
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

void GameScene::ExecuteTransition()
{
	if (!App->transition_manager->is_transitioning)
	{
		if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		{
			App->transition_manager->CreateCut(SCENES::MENU_SCENE);
		}

		if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
		{
			App->transition_manager->CreateFadeToColour(SCENES::MENU_SCENE);
		}

		if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
		{
			App->transition_manager->CreateSlide(SCENES::WIN_SCENE, 0.5f, true);
		}

		if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN)
		{
			App->transition_manager->CreateSlide(SCENES::LOSE_SCENE, 0.5f, true, true);
		}

	//	if (App->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN)
	//	{
	//		App->transition_manager->CreateWipe(SCENES::SECOND_SCENE, 0.5f, true);
	//	}

	//	if (App->input->GetKey(SDL_SCANCODE_6) == KEY_DOWN)
	//	{
	//		App->transition_manager->CreateWipe(SCENES::SECOND_SCENE, 0.5f, true, true);
	//	}

	//	if (App->input->GetKey(SDL_SCANCODE_7) == KEY_DOWN)
	//	{
	//		App->transition_manager->CreateAlternatingBars(SCENES::SECOND_SCENE, 0.5f, true);
	//	}

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