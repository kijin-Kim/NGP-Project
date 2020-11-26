#pragma once
#include "Client/Entry.h"
#include "Client/State.h"
#include "Client/Renderer.h"
#include "Client/FontData.h"


class GameState : public State
{
public:
	GameState(Game* game) : State(game)
	{
		TextureManager* textureManager = TextureManager::GetInstance();
		textureManager->LoadTexture("assets/textures/Map.png");
		Texture background = textureManager->GetTexture("assets/textures/Map.png");

		// TEMP
		Texture pickchu = textureManager->GetTextureFromAtlas("assets/textures/sprite_sheet.png", "pikachu/pikachu_0_0.png");
		Texture ball = textureManager->GetTextureFromAtlas("assets/textures/sprite_sheet.png", "ball/ball_0.png");


		m_PickachuWalking[0] = textureManager->GetTextureFromAtlas("assets/textures/sprite_sheet.png", "pikachu/pikachu_0_0.png");
		m_PickachuWalking[1] = textureManager->GetTextureFromAtlas("assets/textures/sprite_sheet.png", "pikachu/pikachu_0_1.png");
		m_PickachuWalking[2] = textureManager->GetTextureFromAtlas("assets/textures/sprite_sheet.png", "pikachu/pikachu_0_2.png");
		m_PickachuWalking[3] = textureManager->GetTextureFromAtlas("assets/textures/sprite_sheet.png", "pikachu/pikachu_0_3.png");
		m_PickachuWalking[4] = textureManager->GetTextureFromAtlas("assets/textures/sprite_sheet.png", "pikachu/pikachu_0_4.png");

		m_PickachuJumping[0] = textureManager->GetTextureFromAtlas("assets/textures/sprite_sheet.png", "pikachu/pikachu_1_0.png");
		m_PickachuJumping[1] = textureManager->GetTextureFromAtlas("assets/textures/sprite_sheet.png", "pikachu/pikachu_1_1.png");
		m_PickachuJumping[2] = textureManager->GetTextureFromAtlas("assets/textures/sprite_sheet.png", "pikachu/pikachu_1_2.png");
		m_PickachuJumping[3] = textureManager->GetTextureFromAtlas("assets/textures/sprite_sheet.png", "pikachu/pikachu_1_3.png");
		m_PickachuJumping[4] = textureManager->GetTextureFromAtlas("assets/textures/sprite_sheet.png", "pikachu/pikachu_1_4.png");

		m_PickachuPowerHiting[0] = textureManager->GetTextureFromAtlas("assets/textures/sprite_sheet.png", "pikachu/pikachu_2_0.png");
		m_PickachuPowerHiting[1] = textureManager->GetTextureFromAtlas("assets/textures/sprite_sheet.png", "pikachu/pikachu_2_1.png");

		m_BallIdle[0] = textureManager->GetTextureFromAtlas("assets/textures/sprite_sheet.png", "ball/ball_0.png");
		m_BallIdle[1] = textureManager->GetTextureFromAtlas("assets/textures/sprite_sheet.png", "ball/ball_1.png");
		m_BallIdle[2] = textureManager->GetTextureFromAtlas("assets/textures/sprite_sheet.png", "ball/ball_2.png");
		m_BallIdle[3] = textureManager->GetTextureFromAtlas("assets/textures/sprite_sheet.png", "ball/ball_3.png");
		m_BallIdle[4] = textureManager->GetTextureFromAtlas("assets/textures/sprite_sheet.png", "ball/ball_4.png");

		m_BallPowerHiting[0] = textureManager->GetTextureFromAtlas("assets/textures/sprite_sheet.png", "ball/ball_hyper.png");
		m_BallPowerHiting[1] = textureManager->GetTextureFromAtlas("assets/textures/sprite_sheet.png", "ball/ball_trail.png");


		m_Numbers[0] = textureManager->GetTextureFromAtlas("assets/textures/sprite_sheet.png", "number/number_0.png");
		m_Numbers[1] = textureManager->GetTextureFromAtlas("assets/textures/sprite_sheet.png", "number/number_1.png");
		m_Numbers[2] = textureManager->GetTextureFromAtlas("assets/textures/sprite_sheet.png", "number/number_2.png");
		m_Numbers[3] = textureManager->GetTextureFromAtlas("assets/textures/sprite_sheet.png", "number/number_3.png");
		m_Numbers[4] = textureManager->GetTextureFromAtlas("assets/textures/sprite_sheet.png", "number/number_4.png");
		m_Numbers[5] = textureManager->GetTextureFromAtlas("assets/textures/sprite_sheet.png", "number/number_5.png");
		m_Numbers[6] = textureManager->GetTextureFromAtlas("assets/textures/sprite_sheet.png", "number/number_6.png");
		m_Numbers[7] = textureManager->GetTextureFromAtlas("assets/textures/sprite_sheet.png", "number/number_7.png");
		m_Numbers[8] = textureManager->GetTextureFromAtlas("assets/textures/sprite_sheet.png", "number/number_8.png");
		m_Numbers[9] = textureManager->GetTextureFromAtlas("assets/textures/sprite_sheet.png", "number/number_9.png");


		m_MapQuad.Position = glm::vec2(432.0f/2.0f,304.0f/2.0f);
		m_MapQuad.Image = background;
		
		m_ObjectsQuad[0].Position = glm::vec2(8.0f + 16.0f * 2, 16.0f * 4);
		m_ObjectsQuad[0].Image = pickchu;
		m_ObjectsQuad[0].bUseColor = true;
		m_ObjectsQuad[0].Color = glm::vec4(0.7f, 0.7f, 0.0f, 1.0f);

		m_ObjectsQuad[1].Position = glm::vec2(8.0f + 16.0f * (2 + 4), 16.0f * 4);
		m_ObjectsQuad[1].Image = pickchu;
		m_ObjectsQuad[1].bUseColor = true;
		m_ObjectsQuad[1].Color = glm::vec4(0.7f, 0.7f, 0.0f, 1.0f);


		m_ObjectsQuad[2].Position = glm::vec2(8.0f + 16.0f * (19 - 2 + 4), 16.0f * 4);
		m_ObjectsQuad[2].Image = pickchu;
		m_ObjectsQuad[2].bShouldFlipVertical = true;
		m_ObjectsQuad[2].bUseColor = true;
		m_ObjectsQuad[2].Color = glm::vec4(0.7f, 0.7f, 0.0f, 1.0f);

		m_ObjectsQuad[3].Position = glm::vec2(8.0f + 16.0f * (19 - 2), 16.0f * 4);
		m_ObjectsQuad[3].Image = pickchu;
		m_ObjectsQuad[3].bShouldFlipVertical = true;
		m_ObjectsQuad[3].bUseColor = true;
		m_ObjectsQuad[3].Color = glm::vec4(0.7f, 0.7f, 0.0f, 1.0f);


		m_ObjectsQuad[4].Position = glm::vec2(8.0f + 16.0f * 2, 16.0f * 13);
		m_ObjectsQuad[4].Image = ball;


		m_ScoreQuads[0].Position = glm::vec2(8.0f + 16.0f * 6, 16.0f * 17);
		m_ScoreQuads[0].Image = m_Numbers[0];
		m_ScoreQuads[1].Position = glm::vec2(8.0f + 16.0f * 8, 16.0f * 17);
		m_ScoreQuads[1].Image = m_Numbers[0];


		m_ScoreQuads[2].Position = glm::vec2(8.0f + 16.0f * (7 + 13), 16.0f * 17);
		m_ScoreQuads[2].Image = m_Numbers[0];
		m_ScoreQuads[3].Position = glm::vec2(8.0f + 16.0f * (7 + 13 + 2), 16.0f * 17);
		m_ScoreQuads[3].Image = m_Numbers[0];

	} 

	virtual ~GameState() = default;


	virtual void SendData() override
	{
		UserInput input = {};
		auto& inputQueue = m_Game->GetInputQueue();
		input.Key = -1;
		if (!inputQueue.empty())
		{
			input = inputQueue.front();
			inputQueue.pop();
		}

		ClientToServerInGame data = {};
		data.ID = 0;
		data.Input = input;

		Network::GetInstance()->Send(m_Game->GetSocket(), (char*)&data, sizeof(data));
	}
	virtual void ReceiveData() override
	{
		ServerToClientInGame data = {};
		Network::GetInstance()->Recv(m_Game->GetSocket(), (char*)&data, sizeof(data));
		
		for (int i = 0; i < _countof(m_ObjectsQuad); i++)
		{
			m_ObjectsQuad[i].Position.x = data.ObjectPositions[i].X;
			m_ObjectsQuad[i].Position.y = data.ObjectPositions[i].Y;
			
			// PICKACHUS SPECIFICE
			if (i != 4)
			{
				switch (data.AnimationData[i].State)
				{
				case PickachuState::Pickachu_Walking:
				case PickachuState::Pickachu_Idle:
					m_ObjectsQuad[i].Image = m_PickachuWalking[data.AnimationData[i].AnimationIndex];
					break;
				case PickachuState::Pickachu_Jumping:
					m_ObjectsQuad[i].Image = m_PickachuJumping[data.AnimationData[i].AnimationIndex];
					break;
				case PickachuState::Pickachu_PowerHiting:
					m_ObjectsQuad[i].Image = m_PickachuPowerHiting[data.AnimationData[i].AnimationIndex];
					break;
				default:
					break;
				}

				if (i == data.ID)
					m_ObjectsQuad[data.ID].bUseColor = false;
			}
			else
			{
				switch (data.AnimationData[i].State)
				{
				case BallState::Ball_Idle:
					m_ObjectsQuad[i].Image = m_BallIdle[data.AnimationData[i].AnimationIndex];
					break;
				case BallState::Ball_PowerHiting:
					if(data.AnimationData[i].AnimationIndex == 2)
						m_ObjectsQuad[i].Image = m_BallIdle[data.AnimationData[i].AnimationIndex];
					m_ObjectsQuad[i].Image = m_BallPowerHiting[data.AnimationData[i].AnimationIndex];
					break;
				default:
					break;
				}
				
			}
	
		}

		for (int i = 0; i < _countof(data.Scores); i++)
		{
			if (data.Scores[i] > 15)
				break;
			m_ScoreQuads[0 + i * 2].Image = m_Numbers[data.Scores[i] / 10];
			m_ScoreQuads[1 + i * 2].Image = m_Numbers[data.Scores[i] % 10];
		}
	}

	virtual void Render() override
	{
		Renderer* renderer = m_Game->GetRenderer();
		renderer->DrawQuad(m_MapQuad);

		for(auto & quad : m_ObjectsQuad)
			renderer->DrawQuad(quad);
		for(auto & quad : m_ScoreQuads)
			renderer->DrawQuad(quad);
	}

private:
	Renderer::Quad m_ObjectsQuad[5];
	Renderer::Quad m_ScoreQuads[4];
	Renderer::Quad m_MapQuad;
	
	Texture m_Numbers[10];

	// ANIMATION TEXTURES
	Texture m_PickachuWalking[5];
	Texture m_PickachuJumping[5];
	Texture m_PickachuPowerHiting[2];
	Texture m_BallPowerHiting[2];

	Texture m_BallIdle[5];
	

	ServerToClientInGame m_Data = {};
};

class LobbyState : public State
{
public:
	LobbyState(Game* game) : State(game)
	{
		TextureManager* textureManager = TextureManager::GetInstance();
		Texture sittingPickachu = textureManager->GetTextureFromAtlas("assets/textures/sprite_sheet.png", "sitting_pikachu.png");

		for (int x = 0; x < 5; x++)
		{
			for (int y = 0; y < 4; y++)
			{
				m_BackGroundTiles[y + 4 * x].Image = sittingPickachu;
				m_BackGroundTiles[y + 4 * x].Position = glm::vec2(104.0f * x, 104.0f * y);
			}

		}

		m_ChatBoard.bUseColor = true;
		m_ChatBoard.bUseTexture = false;
		m_ChatBoard.Position = glm::vec2(432/ 2.0f, 304 / 2.0f);
		m_ChatBoard.Color = glm::vec4(0.2f, 0.2f, 0.2f, 0.8f);
		m_ChatBoard.Size = glm::vec2(432 - 20,  304 - 20);

		m_FontData = new FontData("assets/fonts/vt323/VT323-Regular.ttf", 20);
	}
	virtual ~LobbyState()
	{
		delete m_FontData;
	}

	virtual void SendData() override
	{
		auto& charQueue = m_Game->GetCharQueue();
		if (charQueue.empty())
		{
			// Send Empty Input
		}
		else
		{
			wchar_t character = charQueue.front();
			// Send User Input
			charQueue.pop();
		}

	}
	virtual void ReceiveData() override
	{
	}


	virtual void Render() override
	{
		Renderer* renderer = m_Game->GetRenderer();
		for (int i = 0; i < _countof(m_BackGroundTiles); i++)
			renderer->DrawQuad(m_BackGroundTiles[i]);
		
		renderer->DrawQuad(m_ChatBoard);

		Renderer::Quad ftQuad;
	
		int advance = 0;
		int lineCount = 0;
		for (int i = 0; i < m_String.size(); i++)
		{
			Font ft = m_FontData->GetFont(m_String[i]);
			ftQuad.bUseTexture = false;
			ftQuad.bUseFont = true;
			ftQuad.Position = glm::vec2(20.0f + advance, 304.0f - 30.0f - lineCount * 15.0f);
			ftQuad.Font = ft;
			ftQuad.bUseColor = true;
			ftQuad.Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
			advance += ftQuad.Font.Advance;

			if (20.0f + advance >= 432.0f - 20.0f)
			{
				lineCount++;
				advance = 0.0f;
			}

			renderer->DrawQuad(ftQuad);
		}

	}

private:
	Renderer::Quad m_BackGroundTiles[20];
	Renderer::Quad m_ChatBoard;
	FontData* m_FontData;
	ServerToClientInLobby m_Data = {};
	std::string m_String = "PBR, or more commonly known as physically based rendering, is a collection of render techniques that are more or less based on the same underlying theory that more closely matches that of the physical world. As physically based rendering aims to mimic light in a physically plausible way, it generally looks more realistic compared to our original lighting algorithms like Phong and Blinn-Phong. Not only does it look better, as it closely approximates actual physics, we (and especially the artists) can author surface materials based on physical parameters without having to resort to cheap hacks and tweaks to make the lighting look right. One of the bigger advantages of authoring materials based on physical parameters is that these materials will look correct regardless of lighting conditions; something that is not true in non-PBR pipelines.";
};

class LoginState : public State
{
public:
	LoginState(Game* game) : State(game)
	{
		TextureManager* textureManager = TextureManager::GetInstance();
		Texture sittingPickachu = textureManager->GetTextureFromAtlas("assets/textures/sprite_sheet.png", "sitting_pikachu.png");
		
		for (int x = 0; x < 5; x++)
		{
			for (int y = 0; y < 4; y++)
			{
				m_BackGroundTiles[y + 4 * x].Image = sittingPickachu;
				m_BackGroundTiles[y + 4 * x].Position = glm::vec2(104.0f * x, 104.0f * y);
			}

		}

	}
	virtual ~LoginState() = default;

	virtual void SendData() override
	{
		ServerToClientInLogin data = {};
		Network::GetInstance()->Send(m_Game->GetSocket(), (char*)&data, sizeof(data));
	}
	virtual void ReceiveData() override
	{
		ServerToClientInLogin data = {};
		Network::GetInstance()->Recv(m_Game->GetSocket(), (char*)&data, sizeof(data));
		if (data.bLoginResult)
		{
			m_Game->SetID(data.ID);
			m_Game->SetState(new GameState(m_Game));
		}	
	}

	virtual void Render() override
	{
		Renderer* renderer = m_Game->GetRenderer();
		for (int i = 0; i < _countof(m_BackGroundTiles); i++)
			renderer->DrawQuad(m_BackGroundTiles[i]);
	}

private:
	Renderer::Quad m_BackGroundTiles[20];
	ServerToClientInLogin m_Data = {};
};


class PickachuVolleyBall : public Game
{
public:
	PickachuVolleyBall(int width, int height, const std::string& serverIP) : Game(width, height, serverIP)
	{
		// Pre Load Textures
		TextureManager* textureManager = TextureManager::GetInstance();
		textureManager->LoadTextureAtlas("assets/textures/sprite_sheet.json", "assets/textures/sprite_sheet.png");

		SetState(new GameState(this));
		//SetState(new LobbyState(this));
		//SetState(new LoginState(this));
	}
	virtual ~PickachuVolleyBall() = default;

};

RegisterApplication(PickachuVolleyBall, 432, 304)