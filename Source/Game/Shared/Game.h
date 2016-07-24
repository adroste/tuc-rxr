#pragma once
#include <memory>
#include "Map.h"

class Game
{
public:
	Game();
	virtual ~Game();

#ifdef _CLIENT
	void draw(Drawing& draw);
#endif // _CLIENT

private:
	std::unique_ptr<Map> m_pMap;
};
