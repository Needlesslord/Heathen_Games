#include "p2Defs.h"
#include "p2Log.h"
#include <vector>
#include "SDL_mixer/include/SDL_mixer.h"

#include "Ranger.h"

#include "j1Audio.h"
#include "j1Player.h"
#include "j1SceneManager.h"
#include "j1Pathfinding.h"
#include "j1Map.h"
#include "j1Collision.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1EntityManager.h"
#include "j1Particles.h"

Ranger::Ranger(iPoint tile, int damage, j1Module* listener, Entity* creator) : Entity(tile, damage, listener, creator) {

	// Handle data and initialize the Warrior
	*(ENTITY_TYPE*)&entityType = ENTITY_TYPE_RANGER;
	*(ENTITY_CATEGORY*)&entityCategory = ENTITY_CATEGORY_DYNAMIC_ENTITY;
	*(ENTITY_SIZE*)&entitySize = ENTITY_SIZE_SMALL;
	*(UNIT_ORIENTATION*)&unitOrientation = UNIT_ORIENTATION_NONE;

	maxLife = 30;
	currLife = maxLife - damage;

	size = { 70, 150 };

	currentTile = tile;
	fPoint tileWorldPosition = App->map->MapToWorld(currentTile.x, currentTile.y);

	pos.x = tileWorldPosition.x + App->map->data.tile_width / size.x;
	pos.y = tileWorldPosition.y + App->map->data.tile_height / 2 - size.y;

	speed = 150.0f;

	spawningTime = 12.0f;

	destination = currentTile;

	attackDamage = 7.0f;
	attackRadius = 5;

	attackSpeed = 15.0f;
	attackCooldown = attackSpeed;

	isEntityFromPlayer = true;

	//FOW
	visibilityRadius = 5;
	fow_entity->frontier = App->fow->CreateSightQuad(visibilityRadius, fow_entity->position);
	fow_entity->LOS = App->fow->FulfillSight(fow_entity->frontier);

}

Ranger::~Ranger() {}

void Ranger::Attack(Entity* target, float dt) {

	if (attackCooldown >= attackSpeed) {

		Particles * p = AddParticle(PARTICLE_TYPE::PARTICLE_RANGER, { pos.x + size.x / 2, pos.y + size.y / 2 }, { 0, 0 });

		particles.push_back(p);

		attackCooldown = 0.0f;

		// TODO: change
		App->audio->PlayFx(App->audio->attack_Sound);

		//TODO: move to Entity manager when something dies
		if (target->GetCurrLife() <= 0)
			Mix_PlayChannel(-1, App->audio->Spawner_Destroyed, 0);
	}

	else {
		attackCooldown += 10 * dt;
	}
}

void Ranger::Draw(SDL_Texture* sprites)
{
	App->render->RenderQueue(1, sprites, pos.x, pos.y, currentAnimation->GetCurrentFrame());
}