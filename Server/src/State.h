#pragma once
#include <Network/Data.h>
#include "GameObject.h"

class State
{
public:
	State() = default;
	virtual ~State() = default;

};

class GameState : public State
{
public:
	GameState() = default;
	virtual ~GameState() = default;

	ServerToClientInGame UpdateData(float deltaTime, const ClientToServerInGame& fromClientData)
	{
		for (auto& p : m_Pickachus)
			p.Update(deltaTime, fromClientData.Input);

		m_Ball.Update(deltaTime);

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

	ServerToClientInLobby UpdateData(float deltaTime, const ClientToServerInLobby& fromClientData) {}
};

class LoginState : public State
{
public:
	LoginState() = default;
	virtual ~LoginState() = default;

	ServerToClientInLogin UpdateData(float deltaTime, const ClientToServerInLogin& fromClientData)
	{
		ServerToClientInLogin outData = {};
		outData.bLoginResult = false;
		if (m_NameCount >= 4 || std::find(std::begin(m_Names), m_Names + m_NameCount, fromClientData.NickName) != std::end(m_Names))
		{
			m_Names[m_NameCount++] = fromClientData.NickName;
			outData.bLoginResult = true;
		}

		return outData;
	}

private:
	std::wstring m_Names[4];
	unsigned int m_NameCount = 0;
};
