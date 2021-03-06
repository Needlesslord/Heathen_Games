#include "j1GUIElements.h"
#include "j1GUI.h"
#include "j1App.h"
#include "j1Fonts.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Audio.h"
#include "j1Textures.h"


j1ScrollBar::j1ScrollBar(SCROLL_TYPE TypeInput) {

	this->type = GUItype::GUI_SCROLLBAR;
	Type = TypeInput;
}

j1ScrollBar::~j1ScrollBar() {

}

bool j1ScrollBar::Start()
{
	Button = App->gui->AddElement(GUItype::GUI_BUTTON, this, { map_position.x, map_position.y - 5 }, inside_position, true, true, { 0, 0, 25 , 25 }, nullptr, this->listener, true, false, SCROLL_TYPE::SCROLL_NONE, true, TEXTURE::ATLAS);
	Button->map_position.y = map_position.y - Button->rect.h / 2 + this->rect.h / 2 - 5;
	Value = 0;



	if (textureType == TEXTURE::SCROLL)
		texture = App->gui->Load_Texture(TEXTURE::SCROLL);

	if (textureType == TEXTURE::ATLAS)
		texture = App->gui->Load_Texture(TEXTURE::ATLAS);

	return true;
}

bool j1ScrollBar::PreUpdate()
{

	Button->enabled = enabled;
	above = OnAbove();

	return true;
}

bool j1ScrollBar::Update(float dt)
{
	if (interactable) {
		if (above)
		{
			if (App->input->GetMouseButtonDown(1) == KEY_DOWN)
				OnClick();
		}
		
	}
	if (enabled == true)
		App->render->AddBlitEventforUI(3, texture, map_position.x - App->render->camera.x, map_position.y - App->render->camera.y, rect, false, true, 0u, 0u, 0u, 255, true);
	

	Float_Value = -((float(-Button->inside_position.x) / (float(-this->rect.w) + float(Button->rect.w))) * 100);
	Value = -((float(-Button->inside_position.x) / (float(-this->rect.w) + float(Button->rect.w)))*100);
	return true;
}

bool j1ScrollBar::PostUpdate()
{
	ScrollLimits();
	return true;
}



bool j1ScrollBar::CleanUp()
{
	App->tex->UnLoad(texture);
	Button->CleanUp();
	return true;
}


void j1ScrollBar::ScrollLimits() 
{

	if (Button->map_position.x > (this->map_position.x + this->rect.w))
	{
		Button->map_position.x = this->map_position.x + this->rect.w - Button->rect.w;

		Button->inside_position.x = this->rect.w - Button->rect.w;

	}
	else if (Button->map_position.x < this->map_position.x)
	{
		Button->map_position.x = this->map_position.x;

		Button->inside_position.x = 0;

	}
}
