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
		m_Ball.SetPosition(glm::vec2(8.0f + 16.0f * 2, 16.0f * 13));
	}
	virtual ~GameState() = default;

	

	virtual IData* UpdateData(float deltaTime, IData* data) override
	{
		AABBCollision();

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

			m_Ball.Update(deltaTime);

			if (m_Ball.GetPosition().y <= 16.0f * 4.0f)
			{
				m_Ball.SetPosition({ m_Ball.GetPosition().x, 16.0f * 4.0f });
				m_Ball.SetVelocity({ m_Ball.GetVelocity().x, -m_Ball.GetVelocity().y });

				if (m_Ball.GetPosition().x < 190)
					m_Scores[0]++;
				else if (m_Ball.GetPosition().x > 240)
					m_Scores[1]++;
			}

			m_Pickachus[0].SetPosition(glm::clamp(m_Pickachus[0].GetPosition(), { 8.0f + 16.0f * 1, 16.0f * 4.0f }, { 8.0f + 16.0f * 11, 304.0f}));
			m_Pickachus[1].SetPosition(glm::clamp(m_Pickachus[1].GetPosition(), { 8.0f + 16.0f * 1, 16.0f * 4.0f }, { 8.0f + 16.0f * 11, 304.0f }));
			m_Pickachus[2].SetPosition(glm::clamp(m_Pickachus[2].GetPosition(), { 8.0f + 16.0f * 15, 16.0f * 4.0f }, { 8.0f + 16.0f * 25, 304.0f }));
			m_Pickachus[3].SetPosition(glm::clamp(m_Pickachus[3].GetPosition(), { 8.0f + 16.0f * 15, 16.0f * 4.0f }, { 8.0f + 16.0f * 25, 304.0f }));

			newOutData->ID = data->ID;
			newOutData->ObjectPositions[i] = { m_Pickachus[i].GetPosition().x, m_Pickachus[i].GetPosition().y };
			newOutData->AnimationData[i].State = m_Pickachus[i].GetState();
			newOutData->AnimationData[i].AnimationIndex = m_Pickachus[i].GetAnimationIndex();
		}
		
		newOutData->ObjectPositions[4] = { m_Ball.GetPosition().x, m_Ball.GetPosition().y };
		newOutData->AnimationData[4].AnimationIndex = m_Ball.GetAnimationIndex();
		newOutData->AnimationData[4].State = m_Ball.GetState();
		newOutData->Scores[0] = m_Scores[0];
		newOutData->Scores[1] = m_Scores[1];

		return newOutData;
	}
private:
	void AABBCollision()
	{
		// BALL VS PLAYER
		AABBBox box1 = m_Ball.GetAABBBox();
		for (int i = 0; i < _countof(m_Pickachus); i++)
		{
			AABBBox box2 = m_Pickachus[i].GetAABBBox();
			bool bShouldPowerHit = m_Pickachus[i].GetState() == PickachuState::Pickachu_PowerHiting;
			bool bBallWasPowerHiting = m_Ball.GetState() == BallState::Ball_PowerHiting;
			if (box2.Left <= box1.Right &&
				box1.Left <= box2.Right &&
				box2.Bottom <= box1.Top &&
				box1.Bottom <= box2.Top
				)
			{

				bool bBallIsOnRight = box1.Right - box2.Right >= 0;
				bool bBallIsOnTop = box1.Top - box2.Top >= 0;

				if ((box1.Right - box2.Right) <= (box1.Top - box2.Top))
				{
					if (bBallIsOnTop)
					{
						float collidedHeight = box2.Top - box1.Bottom;
						m_Ball.SetPosition({ m_Ball.GetPosition().x, m_Ball.GetPosition().y + collidedHeight });
						m_Ball.SetVelocity({ m_Ball.GetVelocity().x, -m_Ball.GetVelocity().y });
					}
					else
					{
						float collidedHeight = box1.Top - box2.Bottom;
						m_Ball.SetPosition({ m_Ball.GetPosition().x, m_Ball.GetPosition().y - collidedHeight });
						m_Ball.SetVelocity({ m_Ball.GetVelocity().x, -m_Ball.GetVelocity().y });
					}
				}
				else
				{
					if (bBallIsOnRight)
					{
						float collidedWidth = box2.Right - box1.Left;
						m_Ball.SetPosition({ m_Ball.GetPosition().x + collidedWidth, m_Ball.GetPosition().y });
						m_Ball.SetVelocity({ -m_Ball.GetVelocity().x, m_Ball.GetVelocity().y });
					}
					else
					{
						float collidedWidth = box1.Right - box2.Left;
						m_Ball.SetPosition({ m_Ball.GetPosition().x - collidedWidth, m_Ball.GetPosition().y });
						m_Ball.SetVelocity({ -m_Ball.GetVelocity().x, m_Ball.GetVelocity().y });
					}
			 

				}
				
				if (bShouldPowerHit)
				{
					m_Ball.SetVelocity({ m_Ball.GetVelocity().x * 1.5f, m_Ball.GetVelocity().y * 1.5f });
					m_Ball.SetState(BallState::Ball_PowerHiting);
				}
				else if (bBallWasPowerHiting)
				{
					m_Ball.SetVelocity({ m_Ball.GetVelocity().x / 2.0f, m_Ball.GetVelocity().y / 2.0f });
					m_Ball.SetState(BallState::Ball_Idle);
				}

			}
		}

		AABBBox box2 = {};
		box2.Left = 200.0f;
		box2.Right = 230.0f;
		box2.Top = 304.0f - 170.0f;
		box2.Bottom = 304.0f - 267.0f;

		if (box2.Left <= box1.Right &&
			box1.Left <= box2.Right &&
			box2.Bottom <= box1.Top &&
			box1.Bottom <= box2.Top
			)
		{
			bool bBallIsOnRight = box1.Right - box2.Right >= 0;
			bool bBallIsOnTop = box1.Top - box2.Top >= 0;


			if ((box1.Right - box2.Right) <= (box1.Top - box2.Top))
			{
				if (bBallIsOnTop)
				{
					float collidedHeight = box2.Top - box1.Bottom;
					m_Ball.SetPosition({ m_Ball.GetPosition().x, m_Ball.GetPosition().y + collidedHeight });
					m_Ball.SetVelocity({ m_Ball.GetVelocity().x, -m_Ball.GetVelocity().y });

				}
				else
				{
					float collidedHeight = box1.Top - box2.Bottom;
					m_Ball.SetPosition({ m_Ball.GetPosition().x, m_Ball.GetPosition().y - collidedHeight });
					m_Ball.SetVelocity({ m_Ball.GetVelocity().x, -m_Ball.GetVelocity().y });
				}
			}
			else
			{
				if (bBallIsOnRight)
				{
					float collidedWidth = box2.Right - box1.Left;
					m_Ball.SetPosition({ m_Ball.GetPosition().x + collidedWidth, m_Ball.GetPosition().y });
					m_Ball.SetVelocity({ -m_Ball.GetVelocity().x, m_Ball.GetVelocity().y });
				}
				else
				{
					float collidedWidth = box1.Right - box2.Left;
					m_Ball.SetPosition({ m_Ball.GetPosition().x - collidedWidth, m_Ball.GetPosition().y });
					m_Ball.SetVelocity({ -m_Ball.GetVelocity().x, m_Ball.GetVelocity().y });
				}
			}
			
		}		

	}

private:
	Pickachu m_Pickachus[4]; // Client 1, 2, 3, 4
	Ball m_Ball;
	unsigned int m_Scores[2] = { 0, 0 };
};

class LobbyState : public State
{
public:
	LobbyState() = default;
	virtual ~LobbyState() = default;
	virtual IData* UpdateData(float deltaTime, IData* data) override
	{
		ServerToClientInLobby* newOutData = new ServerToClientInLobby();
		if (data)
		{
			ClientToServerInLobby* fromClientData = (ClientToServerInLobby*)data;
			if (fromClientData->Chat[0] != 0)
			{
				wchar_t buffer[200];
				wsprintfW(buffer, L"[Client %d ]: %s", data->ID, fromClientData->Chat);

				wcscpy(m_Chats[m_ChatIndex].Line, buffer);
				if (wcslen(buffer) >= 45)
					m_ChatIndex++;
				m_ChatIndex++;

			}
			newOutData->ID = data->ID;
			newOutData->bShouldStartMatch = false;
			memcpy(newOutData->Chats, m_Chats, sizeof(m_Chats));
			if (m_ChatIndex >= 16)
			{
				m_ChatIndex = 0;
				ZeroMemory(m_Chats, sizeof(m_Chats));
			}
		}
		return newOutData;
	}

private:
	ChatLine m_Chats[16];
	unsigned int m_ChatIndex = 0;
};

class LoginState : public State
{
public:
	LoginState() = default;
	virtual ~LoginState() = default;

	virtual IData* UpdateData(float deltaTime, IData* data) override
	{
		ServerToClientInLogin* newOutData = new ServerToClientInLogin();
		if (data)
		{
			ClientToServerInLogin* fromClientData = (ClientToServerInLogin*)data;
			newOutData->ID = fromClientData->ID;
			auto result = std::find(std::begin(m_Names), std::end(m_Names), fromClientData->NickName);
			if (fromClientData->NickName[0] == 0)
			{
				newOutData->Result = LoginResult::None;
			}
			else if (result == std::end(m_Names) && m_NameCount < 4)
			{
				newOutData->Result = LoginResult::Succeded;
				m_Names[m_NameCount++] = fromClientData->NickName;
			}
			else
			{
				printf("FAILED!");
				newOutData->Result = LoginResult::Failed;
			}
		}
		

		return newOutData;

	}

private:
	std::wstring m_Names[4];
	unsigned int m_NameCount = 0;
};
