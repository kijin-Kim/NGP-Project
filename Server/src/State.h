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
	
	virtual IData* UpdateData(float deltaTime, IData* data) = 0;
};

class GameState : public State
{
public:
	GameState()
	{
		m_Pickachus[0].SetPosition(glm::vec2(8.0f + 16.0f * 2, 16.0f * 4));
		m_Pickachus[1].SetPosition(glm::vec2(8.0f + 16.0f * (2 + 4), 16.0f * 4));
		m_Pickachus[2].SetPosition(glm::vec2(8.0f + 16.0f * (19 - 2 + 4), 16.0f * 4));
		m_Pickachus[3].SetPosition(glm::vec2(8.0f + 16.0f * (19 - 2), 16.0f * 4));
	}
	virtual ~GameState() = default;

	virtual IData* UpdateData(float deltaTime, IData* data) override
	{
		if (!data)
		{
			for (auto& pickachu : m_Pickachus)
			{
				pickachu.Update(deltaTime, { GLFW_KEY_UNKNOWN, 0 });
			}
			m_Ball.Update(deltaTime);
			return nullptr;
		}
		
		ClientToServerInGame* fromClientData = (ClientToServerInGame*)data;
		ServerToClientInGame* newOutData = new ServerToClientInGame();

		for (unsigned int i = 0; i < 4; i++)
		{
			if (i != fromClientData->ID)
				m_Pickachus[i].Update(deltaTime, { GLFW_KEY_UNKNOWN, 0 });
			else
				m_Pickachus[i].Update(deltaTime, fromClientData->Input);


			m_Pickachus[0].SetPosition(glm::clamp(m_Pickachus[0].GetPosition(), { 8.0f + 16.0f * 1, 16.0f * 4.0f }, { 8.0f + 16.0f * 11, 304.0f}));
			m_Pickachus[1].SetPosition(glm::clamp(m_Pickachus[1].GetPosition(), { 8.0f + 16.0f * 1, 16.0f * 4.0f }, { 8.0f + 16.0f * 11, 304.0f }));
			m_Pickachus[2].SetPosition(glm::clamp(m_Pickachus[2].GetPosition(), { 8.0f + 16.0f * 15, 16.0f * 4.0f }, { 8.0f + 16.0f * 25, 304.0f }));
			m_Pickachus[3].SetPosition(glm::clamp(m_Pickachus[3].GetPosition(), { 8.0f + 16.0f * 15, 16.0f * 4.0f }, { 8.0f + 16.0f * 25, 304.0f }));



			newOutData->ID = data->ID;
			newOutData->ObjectPositions[i] = { m_Pickachus[i].GetPosition().x, m_Pickachus[i].GetPosition().y };
		}
		return newOutData;
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
	virtual IData* UpdateData(float deltaTime, IData* data) override { return nullptr; }
	
};

class LoginState : public State
{
public:
	LoginState() = default;
	virtual ~LoginState() = default;

	virtual IData* UpdateData(float deltaTime, IData* data) override
	{
		ClientToServerInLogin* fromClientData = (ClientToServerInLogin*)data;
		
		ServerToClientInLogin* newOutData = new ServerToClientInLogin();
		newOutData->bLoginResult = true;
		newOutData->ID = data->ID;

		return newOutData;
	}

private:
	std::wstring m_Names[4];
	unsigned int m_NameCount = 0;
};
