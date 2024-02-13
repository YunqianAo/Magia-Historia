#ifndef __APP_H__
#define __APP_H__

#include "Module.h"
#include "List.h"
#include "PerfTimer.h"
#include "Timer.h"
#include "EntityManager.h"
#include "Animation.h"
#include "ModuleFadeToBlack.h"
#include "ModuleParticles.h"
#include "Fonts.h"
#include "SceneMenu.h"

#include "PugiXml/src/pugixml.hpp"

// Modules

class Window;
class Input;
class Render;
class Textures;
class Audio;
class Scene;
class EntityManager;
class Map;
class Physics;
class Animation;
class ModuleFadeToBlack;
class ModuleParticles;
class Fonts;
class GuiManager;
class SceneMenu;

class App
{
public:

	// Constructor
	App(int argc, char* args[]);

	// Destructor
	virtual ~App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

	bool LoadRequest();

	bool SaveRequest();

	void resetMap();

	uint64 GetFrameCount();

private:

	// Load config file
	bool LoadConfig();

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

	bool LoadFromFile();

	bool SaveFromFile();

	


public:

	bool debug = false ;
	bool godMode = false;

	// Modules
	Window* win;
	Input* input;
	Render* render;
	Textures* tex;
	Audio* audio;
	Scene* scene;
	EntityManager* entityManager;
	Map* map;
	Physics* physics;
	Animation* animation;
	ModuleFadeToBlack* fade;
	ModuleParticles* par;
	Fonts* fonts;
	GuiManager* guiManager;
	SceneMenu* scenemenu;

	int FpsNum = 16;
	int numbers = -1;
	const char* TimeBorde;
	bool closeGame = true;

	bool pausa = false;


private:

	int argc;
	char** args;
	SString title;
	const char* fontsNumber;

	SString organization;

	List<Module *> modules;

	// xml_document to store the config file and
	// xml_node(s) to read specific branches of the xml
	pugi::xml_document configFile;
	pugi::xml_node configNode;

	uint frames;
	float dt;

    // required variables are provided:
	Timer startupTime;
	PerfTimer frameTime;
	PerfTimer lastSecFrameTime;

	uint64 frameCount = 0;
	uint32 framesPerSecond = 0;
	uint32 lastSecFrameCount = 0;

	float averageFps = 0.0f;
	uint32 secondsSinceStartup = 0;

	uint32 maxFrameDuration;

	bool loadRequest = false;
	bool saveRequest = false;



};

extern App* app;

#endif	// __APP_H__