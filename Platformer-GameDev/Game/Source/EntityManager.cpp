#include "EntityManager.h"
#include "Player.h"
#include "Cerdo.h"
#include "Cerdo_bomba.h"
#include "Dragon.h"
#include "Cerdo_Volador.h"
#include "Rey.h"
#include "Food.h"
#include "Lives.h"
#include "App.h"
#include "Textures.h"
#include "Scene.h"
#include "Defs.h"
#include "Log.h"

EntityManager::EntityManager() : Module()
{
	name.Create("entitymanager");
}

// Destructor
EntityManager::~EntityManager()
{}

// Called before render is available
bool EntityManager::Awake(pugi::xml_node& config)
{
	LOG("Loading Entity Manager");
	bool ret = true;

	//Iterates over the entities and calls the Awake
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Awake();
	}

	return ret;

}

bool EntityManager::Start() {

	bool ret = true; 

	//Iterates over the entities and calls Start
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Start();
	}

	return ret;
}

// Called before quitting
bool EntityManager::CleanUp()
{
	bool ret = true;
	ListItem<Entity*>* item;
	item = entities.end;

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	entities.Clear();

	return ret;
}

Entity* EntityManager::CreateEntity(EntityType type)
{
	Entity* entity = nullptr; 

	switch (type)
	{
	case EntityType::PLAYER:
		entity = new Player();
		break;
	case EntityType::ITEM:
		entity = new Item();
		break;
	case EntityType::TRAP:
		entity = new Trap();
		break;
	case EntityType::REY:
		entity = new Rey();
		break;
	case EntityType::CERDO:
		entity = new Cerdo();
		break;
	case EntityType::CERDO_PATRULLADOR:
		entity = new CerdoPatrullador();
		break;
	case EntityType::DRAGON:
		entity = new Dragon();
		break;
	case EntityType::CERDO_VOLADOR:
		entity = new Cerdo_Volador();
		break;
	case EntityType::COMIDA:
		entity = new Food();
		break;
	case EntityType::VIDA:
		entity = new Lives();
		break;
	default:
		break;
	}

	entities.Add(entity);

	return entity;
}

void EntityManager::DestroyEntity(Entity* entity)
{
	ListItem<Entity*>* item;

	for (item = entities.start; item != NULL; item = item->next)
	{
		if (item->data == entity) entities.Del(item);
	}
}


void EntityManager::AddEntity(Entity* entity)
{
	if ( entity != nullptr) entities.Add(entity);
}

void EntityManager::GetCerdo(List<Entity*>& Cerdolista) const
{
	Cerdolista.Clear();

	ListItem<Entity*>* entity;

	for (entity = entities.start; entity != NULL; entity = entity->next)
	{
		if (entity->data->type == EntityType::CERDO)
		{
			Cerdolista.Add(entity->data);
		}
	}
}

void EntityManager::GetCerdoVolador(List<Entity*>& CerdoVoladorlista) const
{
	CerdoVoladorlista.Clear();

	ListItem<Entity*>* entity;

	for (entity = entities.start; entity != NULL; entity = entity->next)
	{
		if (entity->data->type == EntityType::CERDO_VOLADOR)
		{
			CerdoVoladorlista.Add(entity->data);
		}
	}
}

void EntityManager::GetCerdoPatrullador(List<Entity*>& CerdoPatrulladorlista) const
{
	CerdoPatrulladorlista.Clear();

	ListItem<Entity*>* entity;

	for (entity = entities.start; entity != NULL; entity = entity->next)
	{
		if (entity->data->type == EntityType::CERDO_PATRULLADOR)
		{
			CerdoPatrulladorlista.Add(entity->data);
		}
	}
}

void EntityManager::GetDragon(List<Entity*>& Dragonlista) const
{
	Dragonlista.Clear();

	ListItem<Entity*>* entity;

	for (entity = entities.start; entity != NULL; entity = entity->next)
	{
		if (entity->data->type == EntityType::DRAGON)
		{
			Dragonlista.Add(entity->data);
		}
	}
}

bool EntityManager::Update(float dt)
{
	bool ret = true;
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		if (item != NULL)
		{
			pEntity = item->data;

			if (pEntity->active == false) continue; 
			ret = item->data->Update(dt);

		}
		
	}

	return ret;
}