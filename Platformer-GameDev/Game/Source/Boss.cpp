#include "Boss.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "EntityManager.h"
#include "Map.h"

Boss::Boss() : Entity(EntityType::CERDO)
{
	name.Create("Boss");
}

Boss::~Boss() {}

bool Boss::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	speed = parameters.attribute("speed").as_float();

	return true;
}

bool Boss::Start() {

	runRight.LoadAnimations("Runright", "boss");//animacion correr
	runRight.speed = 0.167f;

	idleRight.LoadAnimations("Idleright", "boss");//animacion idle
	idleRight.speed = 0.07f;

	jumpRight.LoadAnimations("Jumpright", "cerdo");//animiacion salto
	jumpRight.speed = 0.167f;

	dead.LoadAnimations("Dead", "cerdo");//animacion muerte
	dead.speed = 0.1f;

	attack.LoadAnimations("Attackright", "boss");//animacion ataque
	attack.speed = 0.05f;

	pathTexture = app->tex->Load("Assets/Textures/tomate.png");//textura del pathfinding
	texture = app->tex->Load(texturePath);//carga textura cerdo
	pbody = app->physics->CreateCircle(position.x, position.y, 30, bodyType::DYNAMIC);//crear collider
	pbody->ctype = ColliderType::CERDO;//establecer tipo de collider
	pbody->listener = this;// Asigna el puntero 'this' (referencia al objeto actual) como el 'listener' del cuerpo físico 'pbody'

	currentAnimation = &idleRight;//asignar 

	return true;
}

bool Boss::Update(float dt)
{
	playerTilePos = app->map->WorldToMap(app->scene->player->position.x, app->scene->player->position.y);
	enemyPosition = app->map->WorldToMap(position.x, position.y);

	b2Vec2 currentVelocity = pbody->body->GetLinearVelocity();

	currentVelocity.y += 0.5;

	distance = playerTilePos.DistanceTo(enemyPosition); // calculamos la distancia entre player y enemigo

	LOG("%d", distance);

	if (tp)
	{
		pbody->body->SetTransform({ PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y) }, 0);
		tp = false;
	}

	if (isDead)
	{
		app->entityManager->DestroyEntity(this);
		app->physics->world->DestroyBody(pbody->body);
	}

	if (destroyBody)
	{
		app->physics->world->DestroyBody(AttackpBody->body);
		destroyBody = false;
	}

	/*if (currentAnimation == &attack && currentAnimation->currentFrame >= 2 && !hasAttacked)
	{
		hasAttacked = true;
		AttackpBody = app->physics->CreateCircle(position.x + 55, position.y + 15, 18, bodyType::DYNAMIC);
		AttackpBody->ctype = ColliderType::DAMAGE;
	}*/

	if (distance < 2.5f)
	{
		Attack();
	}
	else if (distance >= 2.5f && distance <= 5)
	{
		isAttacking = false;
		app->map->pathfinding->CreatePath(enemyPosition, playerTilePos);
		const DynArray<iPoint>* path = app->map->pathfinding->GetLastPath();
		if (path->Count() > 0)
		{
			nextTilePath = { path->At(1)->x, path->At(1)->y };

			int positionTilesx = position.x / 32;
			int positionTilesy = position.y / 32;
			if (path->At(1)->x <= positionTilesx && path->At(1)->x != positionTilesx)
			{
				looksRight = false;
				currentAnimation = &runRight;
				currentVelocity.x = -speed;
				currentVelocity.y += 0.5;
				pbody->body->SetLinearVelocity(currentVelocity);
			}
			else if (path->At(1)->x >= positionTilesx && path->At(1)->x != positionTilesx)
			{
				looksRight = true;
				currentVelocity.x = speed;
				currentAnimation = &runRight;
				currentVelocity.y += 0.5;
				pbody->body->SetLinearVelocity(currentVelocity);

			}
		}
	}
	else if (distance > 5)
	{
		isAttacking = false;
		isJumping = false;
		currentVelocity.x = 0;
		currentVelocity.y += 0.5;
		currentAnimation = &idleRight;
		pbody->body->SetLinearVelocity(currentVelocity);
		app->map->pathfinding->ClearLastPath();
	}

	// Obtiene la posición del cuerpo físico (pbody) y la convierte de unidades de metros a píxeles
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x);
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y);

	SDL_RendererFlip flip = SDL_FLIP_NONE;// Variable para controlar el la orientacion de la textura

	flip = looksRight ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;// Se establece el volteo dependiendo de la condición looksRight
	// Si looksRight es verdadero, se aplica un volteo horizontal; de lo contrario, no se aplica volteo (SDL_FLIP_NONE)

	app->render->DrawTexture(texture, position.x - 70, position.y-66, &currentAnimation->GetCurrentFrame(), flip);// Dibuja la textura en la posición (position.x, position.y)
	// Se aplica el volteo configurado anteriormente

	currentAnimation->Update();// Actualiza la animación

	if (app->physics->debug)// Verifica si el modo de depuración está activado
	{
		const DynArray<iPoint>* path = app->map->pathfinding->GetLastPath(); // Obtiene el último camino calculado por el sistema de búsqueda de ruta
		for (uint i = 0; i < path->Count(); ++i)
		{
			iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);// Convierte las coordenadas del punto del mapa a coordenadas del mundo
			app->render->DrawTexture(pathTexture, pos.x, pos.y);// Dibuja una textura (pathTexture) en la posición correspondiente al punto del camino
		}
	}

	if (!tp)
	{
		position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x);
		position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y);
	}

	return true;
}

bool Boss::CleanUp()
{
	return true;
}

void Boss::Attack()
{
	b2Vec2 currentVelocity = pbody->body->GetLinearVelocity();
	currentAnimation = &attack;
	currentVelocity.x = 0;
	currentVelocity.y += 0.5;
	pbody->body->SetLinearVelocity(currentVelocity);
	destroyBody = true;
	if (looksRight)
	{
		AttackpBody = app->physics->CreateCircle(position.x + 55, position.y, 18, bodyType::DYNAMIC);
		AttackpBody->ctype = ColliderType::DAMAGE;
	}
	else
	{
		AttackpBody = app->physics->CreateCircle(position.x - 55, position.y, 18, bodyType::DYNAMIC);
		AttackpBody->ctype = ColliderType::DAMAGE;
	}
}

void Boss::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype)
	{
	case ColliderType::DAMAGE:// Si el tipo de colisionador es DAMAGE (daño)
		LOG("Collision DAMAGE");
		isDead = true;// Marca al cerdo como muerto
		break;
	case ColliderType::PLATFORM:// Si el tipo de colisionador es PLATFORM (plataforma)
		LOG("Collision PLATFORM");
		isJumping = false;// Indica que el cerdo ya no está saltando
		break;
	}

}
