#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "Module.h"
#include "Entity.h"
#include "List.h"

class EntityManager : public Module
{
public:

	EntityManager(bool start_Enabled = true);

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

	void DestroyAllEntities();

	void AddEntity(Entity* entity);
	void DestroyAllEnemis();

public:

	List<Entity*> entities;
	List<Entity*> enemys;
	List<iPoint> enemys_destroy;


	
};

#endif // __ENTITYMANAGER_H__
