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
	Game(const Game& g) = delete;
	Game& operator=(const Game& g) = delete;

	static Game* getInstance();
	void run();
};

