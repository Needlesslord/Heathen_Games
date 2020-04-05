#include "SDL\include\SDL.h"
#include "j1App.h"
#include "j1Module.h"
#include "p2Point.h"

struct SDL_Texture;
struct SDL_Rect;


class j1Minimap : public j1Module
{
public:
	j1Minimap();
	virtual ~j1Minimap();

	bool Awake(pugi::xml_node& config);

	bool Start();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	void Scale();
	void Descale();

public:
	bool display;
	iPoint position;

private:
	void Load();
	bool MinimapCoords(int& map_x, int& map_y);
	void DrawCamera();
	void MinimapBorders();
	void DrawMinimap();

private:
	SDL_Texture* minimap_tex = nullptr;
	int size;
	float minimap_scale;
	int minimap_width;
	int minimap_height;
	SDL_Rect rect = { 0,0,100,100 };

};
