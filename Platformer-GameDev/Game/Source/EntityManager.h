#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "Module.h"
#include "Entity.h"
#include "List.h"

class EntityManager : public Module
{
public:

	EntityManager(App* app, bool start_enabled = true);

	// Destructor
	virtual ~EntityManager();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called after Awake
	bool Start();

	// Called every frame
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	// Additional methods
	Entity* CreateEntity(EntityType type);

	void DestroyEntity(Entity* entity);

	void AddEntity(Entity* entity);

	void GetCerdo(List<Entity*>& Cerdolista) const;
	void GetCerdoVolador(List<Entity*>& CerdoVoladorlista) const;
	void GetCerdoPatrullador(List<Entity*>& CerdoPatrulladorlista) const;
	void GetDragon(List<Entity*>& Dragonlista) const;

public:

	List<Entity*> entities;

};

#endif // __ENTITYMANAGER_H__
