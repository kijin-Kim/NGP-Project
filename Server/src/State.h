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


		// BALL VS NET
		

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
