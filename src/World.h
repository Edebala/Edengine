class World{
	int PlayerIterator;
	long chunksize;
	std::vector <chunk*> Chunks;
	std::vector <Entity*> Entities;
	std::vector <Light*> Lights;
	SDL_Event event;
	unsigned time;
	unsigned FrameRate;
	SDL_Texture* Map;
	short m_type;
public:
	Camera* Cam;
private:
	chunk* FindChunk(int x, int y);
	void AddChunk(int x, int y, short** m);
	short GetBlock(int x, int y);
	int Update_Entities(string& NextMap);
	int EntityWallHCollision(Entity* i);
	int EntityWallVCollision(Entity* i);
	int EntityEntityCollision(Entity*, Entity*);
public:
	short Game(int &s_x, int &s_y,string& NextMap);
	void LoadMap(const char* Source_Folder);
	void LoadDoors(string Source_Folder);
	World(const string Source_Folder);
	~World();
	void DrawMapByBlock(bool Background=false);
	void DrawMapTexture();
	void DrawEntities();
	void DrawChunk(chunk*, bool Background=false);
	short** GetChunk(int x, int y);
	void Draw();
	bool SetTileset(string);
	void SpawnEntities();
};
