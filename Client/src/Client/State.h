#pragma once
#include "Game.h"

class State
{
public:
	State(Game* game) : m_Game(game) {}
	virtual ~State() = default;

	virtual void SendData() = 0;
	virtual void ReceiveData() = 0;

	virtual void Render() = 0;

protected:
	Game* m_Game;
};


