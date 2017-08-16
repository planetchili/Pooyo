#pragma once

#include "Entity.h"
#include <list>
#include <memory>

class BoardModel
{
public:
	BoardModel();

	void Update( float DeltaTime );

private:
	std::list<std::unique_ptr<Entity>> m_active;

};

