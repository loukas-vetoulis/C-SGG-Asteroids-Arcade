#include "gameobject.h"
#include "gamestate.h"

int GameObject::m_next_id = 1;

GameObject::GameObject(const std::string& name)
	:m_name(name),m_state(Gamestate::getInstance()), m_id(m_next_id++)
{
	
}

