#pragma once
#include <Network/Data.h>
#include "GameObject.h"

enum class ClientState
{
	Login, Lobby, Game
};

class State
{
public:
	State() = default;
	virtual ~State() = default;
};

class GameState : public State
{
public:
	GameState()
	{
		m_Pickachus[0].SetPosition(glm::vec2(8.0f + 16.0f * 2, 16.0f * 4));
	}
	virtual ~GameState() = default;

	IData* UpdateData(float deltaTime, IData* data)
	{
		if (!data)
		{
			for (unsigned int i = 0; i < 4; i++)
			{
				m_Pickachus[i].Update(deltaTime, { GLFW_KEY_UNKNOWN, 0 });
			}
			m_Ball.Update(deltaTime);
			return nullptr;
		}
		else
		{
			ClientToServerInGame* fromClientData = (ClientToServerInGame*)data;
			ServerToClientInGame* newOutData = new ServerToClientInGame();

			for (unsigned int i = 0; i < 4; i++)
			{
				if (i != fromClientData->ID)
					m_Pickachus[i].Update(deltaTime, { GLFW_KEY_UNKNOWN, 0 });
				else
					m_Pickachus[i].Update(deltaTime, fromClientData->Input);

				newOutData->ID = data->ID;
				newOutData->ObjectPositions[i] = { m_Pickachus[i].GetPosition().x, m_Pickachus[i].GetPosition().y };
			}

			return newOutData;
		}
	}
private:
	Pickachu m_Pickachus[4]; // Client 1, 2, 3, 4
	Ball m_Ball;
	unsigned int m_Scores[2];
};

class LobbyState : public State
{
public:
	LobbyState() = default;
	virtual ~LobbyState() = default;

	
};

class LoginState : public State
{
public:
	LoginState() = default;
	virtual ~LoginState() = default;

private:
	std::wstring m_Names[4];
	unsigned int m_NameCount = 0;
};
