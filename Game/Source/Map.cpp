
#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"
#include "Physics.h"
#include "Player.h"
#include "Scene.h"

#include "Defs.h"
#include "Log.h"

#include <math.h>
#include "SDL_image/include/SDL_image.h"

Map::Map(bool start_Enabled) : Module(start_Enabled), mapLoaded(false)
{
	name.Create("map");
}

// Destructor
Map::~Map()
{}

// Called before render is available
bool Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	mapFileNameMapa1 = config.child("mapfile").attribute("path").as_string();
	mapFileNameMapa2 = config.child("mapfile2").attribute("path").as_string();

	mapFolder = config.child("mapfolder").attribute("path").as_string();



	if (LevelMap == 1) {
		mapFileName = mapFileNameMapa1;
	}
	else {
		mapFileName = mapFileNameMapa2;

	}

	return ret;
}

bool Map::Start() {

	if (LevelMap == 1) {
		mapFileName = mapFileNameMapa1;
	}
	else {
		mapFileName = mapFileNameMapa2;

	}

	return true;

}
bool Map::Update(float dt)
{

	if (mapLoaded == false)
		return false;


	ListItem<MapLayer*>* mapLayerItem;
	mapLayerItem = mapData.maplayers.start;

	if (app->scene->GetBoss()->tpToinBossBattle) {

		LoadCollision("Colisions");
	}


	while (mapLayerItem != NULL) {

		if (mapLayerItem->data->properties.GetProperty("Draw") != NULL && mapLayerItem->data->properties.GetProperty("Draw")->value) {


			fondox = app->scene->GetPlayer()->position.x / 32;
			fondoy = app->scene->GetPlayer()->position.y / 32;


			if (app->scene->GetBoss()->inBossBattle) {
				bossRenderArea_R = 35;
				bossRenderArea_L = 35;
			}
			else {
				bossRenderArea_R = 32;
				bossRenderArea_L = 30;
			}


			if (fondoy >= 50) {
				fondoy = 50;
			}

			if (fondoy <= 20) {
				fondoy = 20;
			}

			if (fondox >= 183) {
				fondox = 183;
			}


			for (int x = MAX(fondox - bossRenderArea_L, 0); x < MIN(fondox + bossRenderArea_R, mapLayerItem->data->width); x++)
			{
				for (int y = MAX(fondoy - 20, 0); y < MIN(fondoy + 18, mapLayerItem->data->height); y++)
				{
					uint gid = mapLayerItem->data->Get(x, y);
					TileSet* tileset = GetTilesetFromTileId(gid);

					SDL_Rect r = tileset->GetTileRect(gid);
					iPoint pos = MapToWorld(x, y);

					int bits = 0;
					SDL_RendererFlip flip = SDL_FLIP_NONE;
					int angle = 0;

					if (gid >= 100000) {
						uint tiledID = static_cast<uint>(gid & ~0xE0000000);
						bits = gid >> 29;
						tileset = GetTilesetFromTileId(tiledID);
						r = tileset->GetTileRect(tiledID);

					}
					//1 = hoz_flip -> True || 1 = vert_flip -> True  || 0 = anti-diag flip -> False
					switch (bits) {
					case 0b101: flip = SDL_FLIP_NONE;           angle = 90;         break;
					case 0b110: flip = SDL_FLIP_NONE;           angle += 180;       break;
					case 0b011: flip = SDL_FLIP_NONE;           angle += 270;       break;
					case 0b100: flip = SDL_FLIP_HORIZONTAL;     angle = 0;          break;
					case 0b111: flip = SDL_FLIP_HORIZONTAL;     angle += 90;        break;
					case 0b010: flip = SDL_FLIP_HORIZONTAL;     angle += 180;       break;
					case 0b001: flip = SDL_FLIP_HORIZONTAL;     angle += 270;       break;
					}


					app->render->DrawTexture(tileset->texture,
						pos.x,
						pos.y, flip,
						&r, 1, angle);
				}
			}
		}
		mapLayerItem = mapLayerItem->next;

	}

	return true;
}
bool Map::UpdateDelante()
{
	if (mapLoaded == false)
		return false;


	ListItem<MapLayer*>* mapLayerItem;
	mapLayerItem = mapData.maplayers.start;

	while (mapLayerItem != NULL) {

		if (mapLayerItem->data->properties.GetProperty("Delante") != NULL && mapLayerItem->data->properties.GetProperty("Delante")->value) {

			fondox = app->scene->GetPlayer()->position.x / 32;
			fondoy = app->scene->GetPlayer()->position.y / 32;

			for (int x = MAX(fondox - 18, 0); x < MIN(fondox + 32, mapLayerItem->data->width); x++)
			{
				for (int y = MAX(fondoy - 18, 0); y < MIN(fondoy + 18, mapLayerItem->data->height); y++)
				{
					uint gid = mapLayerItem->data->Get(x, y);
					TileSet* tileset = GetTilesetFromTileId(gid);

					SDL_Rect r = tileset->GetTileRect(gid);
					iPoint pos = MapToWorld(x, y);

					int bits = 0;
					SDL_RendererFlip flip = SDL_FLIP_NONE;
					int angle = 0;

					if (gid >= 100000) {
						uint tiledID = static_cast<uint>(gid & ~0xE0000000);
						bits = gid >> 29;
						tileset = GetTilesetFromTileId(tiledID);
						r = tileset->GetTileRect(tiledID);

					}
					//1 = hoz_flip -> True || 1 = vert_flip -> True  || 0 = anti-diag flip -> False
					switch (bits) {
					case 0b101: flip = SDL_FLIP_NONE;           angle = 90;         break;
					case 0b110: flip = SDL_FLIP_NONE;           angle += 180;       break;
					case 0b011: flip = SDL_FLIP_NONE;           angle += 270;       break;
					case 0b100: flip = SDL_FLIP_HORIZONTAL;     angle = 0;          break;
					case 0b111: flip = SDL_FLIP_HORIZONTAL;     angle += 90;        break;
					case 0b010: flip = SDL_FLIP_HORIZONTAL;     angle += 180;       break;
					case 0b001: flip = SDL_FLIP_HORIZONTAL;     angle += 270;       break;
					}

					app->render->DrawTexture(tileset->texture,
						pos.x,
						pos.y, flip,
						&r, 1, angle);
				}
			}
		}
		mapLayerItem = mapLayerItem->next;

	}

	return true;
}

iPoint Map::MapToWorld(int x, int y) const
{
	iPoint ret;

	if (mapData.type == MapTypes::MAPTYPE_ORTHOGONAL) {
		ret.x = x * mapData.tileWidth;
		ret.y = y * mapData.tileHeight;
	}

	if (mapData.type == MapTypes::MAPTYPE_ISOMETRIC) {
		ret.x = x * mapData.tileWidth / 2 - y * mapData.tileWidth / 2;
		ret.y = x * mapData.tileHeight / 2 + y * mapData.tileHeight / 2;
	}

	return ret;
}

iPoint Map::WorldToMap(int x, int y) {

	iPoint ret(0, 0);

	if (mapData.type == MapTypes::MAPTYPE_ORTHOGONAL || mapData.type == MAPTYPE_UNKNOWN) {
		ret.x = x / mapData.tileWidth;
		ret.y = y / mapData.tileHeight;

	}

	if (mapData.type == MapTypes::MAPTYPE_ISOMETRIC) {
		float half_width = mapData.tileWidth / 2;
		float half_height = mapData.tileHeight / 2;
		ret.x = int((x / half_width + y / half_height) / 2);
		ret.y = int((y / half_height - (x / half_width)) / 2);
	}

	return ret;
}


// Get relative Tile rectangle
SDL_Rect TileSet::GetTileRect(int gid) const
{
	SDL_Rect rect = { 0 };
	int relativeIndex = gid - firstgid;

	rect.w = tileWidth;
	rect.h = tileHeight;
	rect.x = margin + (tileWidth + spacing) * (relativeIndex % columns);
	rect.y = margin + (tileWidth + spacing) * (relativeIndex / columns);

	return rect;
}

TileSet* Map::GetTilesetFromTileId(int gid) const
{
	ListItem<TileSet*>* item = mapData.tilesets.start;
	TileSet* set = NULL;

	while (item)
	{
		set = item->data;

		if (gid < (item->data->firstgid + item->data->tilecount))
		{
			break;
		}
		item = item->next;
	}

	return set;
}

// Called before quitting
bool Map::CleanUp()
{
	LOG("Unloading map");

	ListItem<TileSet*>* item;
	item = mapData.tilesets.start;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	mapData.tilesets.Clear();

	// Remove all layers
	ListItem<MapLayer*>* layerItem;
	layerItem = mapData.maplayers.start;

	while (layerItem != NULL)
	{
		RELEASE(layerItem->data);
		layerItem = layerItem->next;
	}
	mapData.maplayers.Clear();

	// Remove all groupobject
	ListItem<MapObjects*>* objectsItem;
	objectsItem = mapData.mapObjects.start;

	while (objectsItem != NULL)
	{
		RELEASE(objectsItem->data);
		objectsItem = objectsItem->next;
	}
	mapData.mapObjects.Clear();

	// Remove all colisions
	ListItem<PhysBody*>* collision;
	collision = collisionsList.start;
	while (collision != NULL) {

		app->physics->GetWorld()->DestroyBody(collision->data->body);
		collision = collision->next;
	}
	collisionsList.Clear();

	//Remove traspassed list
	traspasedPlatformList.Clear();


	return true;
}

// Load new map
bool Map::Load()
{
	bool ret = true;

	pugi::xml_document mapFileXML;

	pugi::xml_parse_result result = mapFileXML.load_file(mapFileName.GetString());

	if (result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", mapFileName, result.description());
		ret = false;
	}

	if (ret == true)
	{
		ret = LoadMap(mapFileXML);
	}

	if (ret == true)
	{
		ret = LoadTileSet(mapFileXML);
	}

	if (ret == true)
	{
		ret = LoadAllLayers(mapFileXML.child("map"));
	}

	// Find the navigation layer

	ListItem<MapLayer*>* mapLayerItem;
	mapLayerItem = mapData.maplayers.start;
	navigationLayer = mapLayerItem->data;

	//Search the layer in the map that contains information for navigation
	while (mapLayerItem != NULL) {
		if (mapLayerItem->data->properties.GetProperty("Navigation") != NULL && mapLayerItem->data->properties.GetProperty("Navigation")->value) {
			navigationLayer = mapLayerItem->data;
			break;
		}
		mapLayerItem = mapLayerItem->next;
	}

	pathfinding = new PathFinding();



	//Initialize the navigation map
	uchar* navigationMap = NULL;
	CreateNavigationMap(mapData.width, mapData.height, &navigationMap);
	pathfinding->SetNavigationMap((uint)mapData.width, (uint)mapData.height, navigationMap);
	RELEASE_ARRAY(navigationMap);

	LoadCollision("Colisions");


	if (ret == true)
	{
		LOG("Successfully parsed map XML file :%s", mapFileName.GetString());
		LOG("width : %d height : %d", mapData.width, mapData.height);
		LOG("tile_width : %d tile_height : %d", mapData.tileWidth, mapData.tileHeight);

		LOG("Tilesets----");

		ListItem<TileSet*>* tileset;
		tileset = mapData.tilesets.start;

		while (tileset != NULL) {
			LOG("name : %s firstgid : %d", tileset->data->name.GetString(), tileset->data->firstgid);
			LOG("tile width : %d tile height : %d", tileset->data->tileWidth, tileset->data->tileHeight);
			LOG("spacing : %d margin : %d", tileset->data->spacing, tileset->data->margin);
			tileset = tileset->next;
		}

		ListItem<MapLayer*>* mapLayer;
		mapLayer = mapData.maplayers.start;

		while (mapLayer != NULL) {
			LOG("id : %d name : %s", mapLayer->data->id, mapLayer->data->name.GetString());
			LOG("Layer width : %d Layer height : %d", mapLayer->data->width, mapLayer->data->height);
			mapLayer = mapLayer->next;
		}
	}

	if (mapFileXML) mapFileXML.reset();

	mapLoaded = ret;

	return ret;
}

void Map::CreateNavigationMap(int& width, int& height, uchar** buffer) const
{
	bool ret = false;

	//Sets the size of the map. The navigation map is a unidimensional array 
	uchar* navigationMap = new uchar[navigationLayer->width * navigationLayer->height];
	//reserves the memory for the navigation map
	memset(navigationMap, 1, navigationLayer->width * navigationLayer->height);

	for (int x = 0; x < mapData.width; x++)
	{
		for (int y = 0; y < mapData.height; y++)
		{
			//i is the index of x,y coordinate in a unidimensional array that represents the navigation map
			int i = (y * navigationLayer->width) + x;

			//Gets the gid of the map in the navigation layer
			int gid = navigationLayer->Get(x, y);

			//If the gid is a blockedGid is an area that I cannot navigate, so is set in the navigation map as 0, all the other areas can be navigated
			//!!!! make sure that you assign blockedGid according to your map
			if (gid == blockedGid) navigationMap[i] = 1;
			//if (gid == blockedGid2) navigationMap[i] = 1;
			else navigationMap[i] = 0;
		}
	}

	*buffer = navigationMap;
	width = mapData.width;
	height = mapData.height;
}


bool Map::LoadMap(pugi::xml_node mapFile)
{
	bool ret = true;
	pugi::xml_node map = mapFile.child("map");

	if (map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{

		//Load map general properties
		mapData.height = map.attribute("height").as_int();
		mapData.width = map.attribute("width").as_int();
		mapData.tileHeight = map.attribute("tileheight").as_int();
		mapData.tileWidth = map.attribute("tilewidth").as_int();
		mapData.type = MAPTYPE_UNKNOWN;

		SString orientationStr = map.attribute("orientation").as_string();
		if (orientationStr == "orthogonal") {
			mapData.type = MAPTYPE_ORTHOGONAL;
		}
		else if (orientationStr == "isometric") {
			mapData.type = MAPTYPE_ISOMETRIC;
		}
		else {
			LOG("Map orientation not found");
			ret = false;
		}
	}


	return ret;
}

bool Map::LoadTileSet(pugi::xml_node mapFile) {

	bool ret = true;

	pugi::xml_node tileset;
	for (tileset = mapFile.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		set->name = tileset.attribute("name").as_string();
		set->firstgid = tileset.attribute("firstgid").as_int();
		set->margin = tileset.attribute("margin").as_int();
		set->spacing = tileset.attribute("spacing").as_int();
		set->tileWidth = tileset.attribute("tilewidth").as_int();
		set->tileHeight = tileset.attribute("tileheight").as_int();
		set->columns = tileset.attribute("columns").as_int();
		set->tilecount = tileset.attribute("tilecount").as_int();

		SString tmp("%s%s", mapFolder.GetString(), tileset.child("image").attribute("source").as_string());
		set->texture = app->tex->Load(tmp.GetString());

		mapData.tilesets.Add(set);
	}

	return ret;
}

bool Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;

	//Load the attributes
	layer->id = node.attribute("id").as_int();
	layer->name = node.attribute("name").as_string();
	layer->width = node.attribute("width").as_int();
	layer->height = node.attribute("height").as_int();

	LoadProperties(node, layer->properties);

	//Reserve the memory for the data 
	layer->data = new uint[layer->width * layer->height];
	memset(layer->data, 0, layer->width * layer->height);

	//Iterate over all the tiles and assign the values
	pugi::xml_node tile;
	int i = 0;
	for (tile = node.child("data").child("tile"); tile && ret; tile = tile.next_sibling("tile"))
	{
		layer->data[i] = tile.attribute("gid").as_uint();
		i++;
	}

	return ret;
}

bool Map::LoadAllLayers(pugi::xml_node mapNode) {
	bool ret = true;

	for (pugi::xml_node layerNode = mapNode.child("layer"); layerNode && ret; layerNode = layerNode.next_sibling("layer"))
	{
		//Load the layer
		MapLayer* mapLayer = new MapLayer();
		ret = LoadLayer(layerNode, mapLayer);

		//add the layer to the map
		mapData.maplayers.Add(mapLayer);
	}

	return ret;
}

bool Map::LoadProperties(pugi::xml_node& node, Properties& properties)
{
	bool ret = false;

	for (pugi::xml_node propertieNode = node.child("properties").child("property"); propertieNode; propertieNode = propertieNode.next_sibling("property"))
	{
		Properties::Property* p = new Properties::Property();
		p->name = propertieNode.attribute("name").as_string();
		p->value = propertieNode.attribute("value").as_bool(); // (!!) I'm assuming that all values are bool !!

		properties.list.Add(p);
	}

	return ret;
}

Properties::Property* Properties::GetProperty(const char* name)
{
	ListItem<Property*>* item = list.start;
	Property* p = NULL;

	while (item)
	{
		if (item->data->name == name) {
			p = item->data;
			break;
		}
		item = item->next;
	}

	return p;
}



bool Map::LoadCollision(std::string layerName) {



	ListItem<MapLayer*>* mapLayerItem;
	mapLayerItem = mapData.maplayers.start;

	while (mapLayerItem != NULL) {

		if (mapLayerItem->data->name.GetString() == layerName) {

			for (int x = 0; x < mapLayerItem->data->width; x++)
			{
				for (int y = 0; y < mapLayerItem->data->height; y++)
				{
					int gid = mapLayerItem->data->Get(x, y);
					TileSet* tileset = GetTilesetFromTileId(gid);

					SDL_Rect r = tileset->GetTileRect(gid);
					iPoint pos = MapToWorld(x, y);

					//tipo
					PhysBody* c1;
					if (gid == tileset->firstgid) {

						c1 = app->physics->CreateRectangle(pos.x + 16, pos.y + 16, 32, 32, STATIC);
						c1->ctype = ColliderType::PLATFORM;
						collisionsList.Add(c1);
					}

					if (gid == tileset->firstgid + 1) {
						if (colisionsPointsSize.x == -1 || colisionsPointsSize.y == -1) {
							startPointcolisions.x = pos.x;
							startPointcolisions.y = pos.y;
							colisionsPointsSize.x = 0;
							colisionsPointsSize.y = 0;

						}

						colisionsPointsSize.y += 32;
					}

					else {

						if (colisionsPointsSize.y != -1) {

							c1 = app->physics->CreateRectangle(startPointcolisions.x + 16, startPointcolisions.y + colisionsPointsSize.y / 2, 32, colisionsPointsSize.y, STATIC);
							c1->ctype = ColliderType::WALL;
							collisionsList.Add(c1);

							colisionsPointsSize.x = -1;
							colisionsPointsSize.y = -1;
						}
					}


					if (gid == tileset->firstgid + 5) {

						int* puntos = new int[6];
						memset(puntos, 0, 6);

						puntos[0] = +16;
						puntos[1] = -16;

						puntos[2] = +16;
						puntos[3] = +16;

						puntos[4] = -16;
						puntos[5] = +16;

						c1 = app->physics->CreateChain(pos.x + 16, pos.y + 16, puntos, 6, STATIC);
						c1->ctype = ColliderType::PLATFORM;
						collisionsList.Add(c1);

					}
					if (gid == tileset->firstgid + 6) {
						int* puntos = new int[6];
						memset(puntos, 0, 6);

						puntos[0] = -16;
						puntos[1] = +16;

						puntos[2] = -16;
						puntos[3] = -16;

						puntos[4] = +16;
						puntos[5] = +16;

						c1 = app->physics->CreateChain(pos.x + 16, pos.y + 16, puntos, 6, STATIC);
						c1->ctype = ColliderType::PLATFORM;
						collisionsList.Add(c1);
					}

					if (gid == tileset->firstgid + 8) {
						c1 = app->physics->CreateRectangleSensor(pos.x + 16, pos.y + 16, 32, 32, STATIC);
						c1->ctype = ColliderType::PLAYERLEAVE;
						collisionsList.Add(c1);
					}

					if (gid == tileset->firstgid + 9) {

						c1 = app->physics->CreateRectangleSensor(pos.x + 16, pos.y + 16, 32, 32, STATIC);
						c1->ctype = ColliderType::ENEMYAREA;
						collisionsList.Add(c1);
					}


					if (gid == tileset->firstgid + 11) {
						c1 = app->physics->CreateRectangle(pos.x - 16, pos.y + 16, 32, 32, STATIC);
						c1->ctype = ColliderType::WALL;
						collisionsList.Add(c1);

					}

					if (gid == tileset->firstgid + 12) {
						c1 = app->physics->CreateRectangle(pos.x - 16, pos.y + 245, 32, 32, STATIC);
						c1->ctype = ColliderType::DEADPLATFORM;
						collisionsList.Add(c1);
					}

					if (gid == tileset->firstgid + 13) {
						c1 = app->physics->CreateRectangle(pos.x + 64, pos.y + 16, 32, 32, STATIC);
						c1->ctype = ColliderType::WALL;
						collisionsList.Add(c1);

					}

					if (gid == tileset->firstgid + 14) {
						PhysBody* c1 = app->physics->CreateRectangle(pos.x - 16, pos.y - 64, 32, 32, STATIC);
						c1->ctype = ColliderType::WALL;

					}
					if (gid == tileset->firstgid + 15) {
						c1 = app->physics->CreateRectangleSensor(pos.x + 16, pos.y + 16, 32, 32, STATIC);
						c1->ctype = ColliderType::VICTORYCOLLISION;
						collisionsList.Add(c1);

					}


					if (app->scene->GetBoss()->inBossBattle) {
						if (gid == tileset->firstgid + 10) {
							c1 = app->physics->CreateRectangle(pos.x, pos.y + 16, 32, 32, STATIC);
							c1->ctype = ColliderType::WALL;
							collisionsList2.Add(c1);
						}
					}


				}
			}
		}
		mapLayerItem = mapLayerItem->next;
	}
	return true;
}