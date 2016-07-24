#pragma once
#include <memory>
#include "Map.h"

class Game : public IDrawable
{
public:
	Game();
	virtual ~Game();

#ifdef _CLIENT
	virtual void draw(Drawing& draw) override;
#endif // _CLIENT

private:
	std::unique_ptr<Map> m_pMap;
};
