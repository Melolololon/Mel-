#pragma once
class Game final
{
private:
	Game();
	~Game();

	void initialize();
	void update();
	void draw();
	void end();

public:
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;

	static Game* getInstance();
	void run();
};

