#include "Cerdo.h"
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

Cerdo::Cerdo() : Entity(EntityType::CERDO)
{
	name.Create("Cerdo");
}

Cerdo::~Cerdo() {}

bool Cerdo::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	speed = parameters.attribute("speed").as_float();

	return true;
}

bool Cerdo::Start() {

	runRight.LoadAnimations("Runright", "cerdo");//animacion correr
	runRight.speed = 0.167f;

	idleRight.LoadAnimations("Idleright", "cerdo");//animacion idle
	idleRight.speed = 0.167f;

	jumpRight.LoadAnimations("Jumpright", "cerdo");//animiacion salto
	jumpRight.speed = 0.167f;

	dead.LoadAnimations("Dead", "cerdo");//animacion muerte
	dead.speed = 0.1f;

	attack.LoadAnimations("Attackright", "cerdo");//animacion ataque
	attack.speed = 0.01f;

	pathTexture = app->tex->Load("Assets/Textures/tomate.png");//textura del pathfinding
	texture = app->tex->Load(texturePath);//carga textura cerdo
	pbody = app->physics->CreateCircle(position.x, position.y, 10, bodyType::DYNAMIC);//crear collider
	pbody->ctype = ColliderType::CERDO;//establecer tipo de collider
	pbody->listener = this;// Asigna el puntero 'this' (referencia al objeto actual) como el 'listener' del cuerpo f�sico 'pbody'

	currentAnimation = &idleRight;//asignar 

	return true;
}

bool Cerdo::Update(float dt)
{
	playerTilePos = app->map->WorldToMap(app->scene->player->position.x + 16, app->scene->player->position.y);
	enemyPosition = app->map->WorldToMap(position.x + 8, position.y);

	b2Vec2 vel = b2Vec2(0, -GRAVITY_Y);
	b2Vec2 currentVelocity = pbody->body->GetLinearVelocity();

	currentVelocity.y += 0.5;

	distance = playerTilePos.DistanceTo(enemyPosition); // calculamos la distancia entre player y enemigo

	if (isDead)
	{
		app->entityManager->DestroyEntity(this);
		app->physics->world->DestroyBody(pbody->body);
	}

	if (distance < 2)
	{
		currentAnimation = &idleRight;
		currentVelocity.x = 0;
		currentVelocity.y += 0.5;
		pbody->body->SetLinearVelocity(currentVelocity);
	}
	else if (distance >= 2 && distance <= 5)
	{
		app->map->pathfinding->CreatePath(enemyPosition, playerTilePos);
		const DynArray<iPoint>* path = app->map->pathfinding->GetLastPath();
		if (path->Count() > 1)
		{
			nextTilePath = { path->At(1)->x, path->At(1)->y };

			int positionTilesx = position.x / 32;
			int positionTilesy = position.y / 32;
			if (path->At(1)->x < positionTilesx && path->At(1)->x != positionTilesx)
			{
				looksRight = false;
				currentAnimation = &runRight;
				currentVelocity.x = -speed * 3;
				currentVelocity.y += 0.5;
				pbody->body->SetLinearVelocity(currentVelocity);
			}
			else if (path->At(1)->x > positionTilesx && path->At(1)->x != positionTilesx)
			{
				looksRight = true;
				currentVelocity.x = speed * 3;
				currentAnimation = &runRight;
				currentVelocity.y += 0.5;
				pbody->body->SetLinearVelocity(currentVelocity);

			}
			else if (path->At(1)->y < positionTilesy && path->At(1)->y != positionTilesy)
			{
				if (!isJumping)
				{
					isJumping = true;
					currentAnimation = &jumpRight;
					currentVelocity.y = -15;
					pbody->body->SetLinearVelocity(currentVelocity);
				}
			}
		}
	}
	else if (distance > 5)
	{
		isJumping = false;
		currentVelocity.x = 0;
		currentVelocity.y += 0.5;
		currentAnimation = &idleRight;
		pbody->body->SetLinearVelocity(currentVelocity);
		app->map->pathfinding->ClearLastPath();
	}

	// Obtiene la posici�n del cuerpo f�sico (pbody) y la convierte de unidades de metros a p�xeles
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 18;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 15;

	SDL_RendererFlip flip = SDL_FLIP_NONE;// Variable para controlar el la orientacion de la textura

	flip = looksRight ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;// Se establece el volteo dependiendo de la condici�n looksRight
	// Si looksRight es verdadero, se aplica un volteo horizontal; de lo contrario, no se aplica volteo (SDL_FLIP_NONE)

	app->render->DrawTexture(texture, position.x, position.y - 2, &currentAnimation->GetCurrentFrame(), flip);// Dibuja la textura en la posici�n (position.x, position.y)
	// Se aplica el volteo configurado anteriormente

	currentAnimation->Update();// Actualiza la animaci�n

	if (app->physics->debug)// Verifica si el modo de depuraci�n est� activado
	{
		const DynArray<iPoint>* path = app->map->pathfinding->GetLastPath(); // Obtiene el �ltimo camino calculado por el sistema de b�squeda de ruta
		for (uint i = 0; i < path->Count(); ++i)
		{
			iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);// Convierte las coordenadas del punto del mapa a coordenadas del mundo
			app->render->DrawTexture(pathTexture, pos.x + 8, pos.y + 8);// Dibuja una textura (pathTexture) en la posici�n correspondiente al punto del camino
		}
	}

	return true;
}

bool Cerdo::CleanUp()
{
	return true;
}

void Cerdo::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype)
	{
	case ColliderType::DAMAGE:// Si el tipo de colisionador es DAMAGE (da�o)
		LOG("Collision DAMAGE");
		isDead = true;// Marca al cerdo como muerto
		break;
	case ColliderType::PLATFORM:// Si el tipo de colisionador es PLATFORM (plataforma)
		LOG("Collision PLATFORM");
		isJumping = false;// Indica que el cerdo ya no est� saltando
		break;
	}

}
