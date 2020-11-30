#pragma once
#include "Client/Entry.h"
#include "Client/State.h"
#include "Client/Renderer.h"
#include "Client/FontData.h"
#include <GLFW/glfw3.h>


class GameState : public State
{
public:
	GameState(Game* game) : State(game)
	{
		TextureManager* textureManager = TextureManager::GetInstance();
		textureManager->LoadTexture("assets/textures/Map.png");
		Texture background = textureManager->GetTexture("assets/textures/Map.png");

		// TEMP

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

		m_PickachuWin[0] = textureManager->GetTextureFromAtlas("assets/textures/sprite_sheet.png", "pikachu/pikachu_5_0.png");
		m_PickachuWin[1] = textureManager->GetTextureFromAtlas("assets/textures/sprite_sheet.png", "pikachu/pikachu_5_1.png");
		m_PickachuWin[2] = textureManager->GetTextureFromAtlas("assets/textures/sprite_sheet.png", "pikachu/pikachu_5_2.png");
		m_PickachuWin[3] = textureManager->GetTextureFromAtlas("assets/textures/sprite_sheet.png", "pikachu/pikachu_5_3.png");
		m_PickachuWin[4] = textureManager->GetTextureFromAtlas("assets/textures/sprite_sheet.png", "pikachu/pikachu_5_4.png");

		m_PickachuLose[0] = textureManager->GetTextureFromAtlas("assets/textures/sprite_sheet.png", "pikachu/pikachu_6_0.png");
		m_PickachuLose[1] = textureManager->GetTextureFromAtlas("assets/textures/sprite_sheet.png", "pikachu/pikachu_6_1.png");
		m_PickachuLose[2] = textureManager->GetTextureFromAtlas("assets/textures/sprite_sheet.png", "pikachu/pikachu_6_2.png");
		m_PickachuLose[3] = textureManager->GetTextureFromAtlas("assets/textures/sprite_sheet.png", "pikachu/pikachu_6_3.png");
		m_PickachuLose[4] = textureManager->GetTextureFromAtlas("assets/textures/sprite_sheet.png", "pikachu/pikachu_6_4.png");


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
		m_ObjectsQuad[0].Image = m_PickachuWalking[0];
		m_ObjectsQuad[0].bUseColor = true;
		m_ObjectsQuad[0].Color = glm::vec4(0.7f, 0.7f, 0.0f, 1.0f);

		m_ObjectsQuad[1].Position = glm::vec2(8.0f + 16.0f * (2 + 4), 16.0f * 4);
		m_ObjectsQuad[1].Image = m_PickachuWalking[0];
		m_ObjectsQuad[1].bUseColor = true;
		m_ObjectsQuad[1].Color = glm::vec4(0.7f, 0.7f, 0.0f, 1.0f);


		m_ObjectsQuad[2].Position = glm::vec2(8.0f + 16.0f * (19 - 2 + 4), 16.0f * 4);
		m_ObjectsQuad[2].Image = m_PickachuWalking[0];
		m_ObjectsQuad[2].bShouldFlipVertical = true;
		m_ObjectsQuad[2].bUseColor = true;
		m_ObjectsQuad[2].Color = glm::vec4(0.7f, 0.7f, 0.0f, 1.0f);

		m_ObjectsQuad[3].Position = glm::vec2(8.0f + 16.0f * (19 - 2), 16.0f * 4);
		m_ObjectsQuad[3].Image = m_PickachuWalking[0];
		m_ObjectsQuad[3].bShouldFlipVertical = true;
		m_ObjectsQuad[3].bUseColor = true;
		m_ObjectsQuad[3].Color = glm::vec4(0.7f, 0.7f, 0.0f, 1.0f);


		m_ObjectsQuad[4].Position = glm::vec2(8.0f + 16.0f * 2, 16.0f * 13);
		m_ObjectsQuad[4].Image = m_BallIdle[0];


		m_ScoreQuads[0].Position = glm::vec2(8.0f + 16.0f * 6, 16.0f * 17);
		m_ScoreQuads[0].Image = m_Numbers[0];
		m_ScoreQuads[1].Position = glm::vec2(8.0f + 16.0f * 8, 16.0f * 17);
		m_ScoreQuads[1].Image = m_Numbers[0];


		m_ScoreQuads[2].Position = glm::vec2(8.0f + 16.0f * (7 + 13), 16.0f * 17);
		m_ScoreQuads[2].Image = m_Numbers[0];
		m_ScoreQuads[3].Position = glm::vec2(8.0f + 16.0f * (7 + 13 + 2), 16.0f * 17);
		m_ScoreQuads[3].Image = m_Numbers[0];


		m_FontData = new FontData("assets/fonts/vt323/VT323-Regular.ttf", 40);
	} 

	virtual ~GameState() = default;


	virtual void SendData() override
	{
		UserInput input = {};
		auto& inputQueue = m_Game->GetInputQueue();
		input.Key = -1;

		if (!m_bGameIsDone) // 게임이 끝나면 조작 불가능
		{
			if (!inputQueue.empty())
			{
				input = inputQueue.front();
				inputQueue.pop();
			}
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
		m_Game->SetID(data.ID);
		
		for (int i = 0; i < _countof(m_ObjectsQuad); i++)
		{
			m_ObjectsQuad[i].Position.x = data.ObjectPositions[i].X;
			m_ObjectsQuad[i].Position.y = data.ObjectPositions[i].Y;
			
			// PICKACHUS SPECIFIC
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
				case PickachuState::Pickachu_Win:
					m_ObjectsQuad[i].Image = m_PickachuWin[data.AnimationData[i].AnimationIndex];
					break;
				case PickachuState::Pickachu_Lose:
					m_ObjectsQuad[i].Image = m_PickachuLose[data.AnimationData[i].AnimationIndex];
					break;
				default:
					break;
				}

				if (i == data.ID)
					m_ObjectsQuad[data.ID].bUseColor = false;
			}
			else if (i==4)
			{
				switch (data.AnimationData[i].State)
				{
				case BallState::Ball_Idle:
					m_ObjectsQuad[i].Image = m_BallIdle[data.AnimationData[i].AnimationIndex];
					break;
				case BallState::Ball_PowerHiting:
					if (data.AnimationData[i].AnimationIndex == 2)
						m_ObjectsQuad[i].Image = m_BallIdle[data.AnimationData[i].AnimationIndex];
					else if(data.AnimationData[i].AnimationIndex <= 2)
						m_ObjectsQuad[i].Image = m_BallPowerHiting[data.AnimationData[i].AnimationIndex];
					break;
				default:
					break;
				}
				
			}
			
		}

		for (int i = 0; i < _countof(data.Scores); i++)
		{
			if (data.Scores[i] > MAX_GAME_SCORE)
			{
				break;
			}
			if (data.Scores[i] == MAX_GAME_SCORE)
			{
				m_bGameIsDone = true;
			}
			m_ScoreQuads[0 + i * 2].Image = m_Numbers[data.Scores[i] / 10];
			m_ScoreQuads[1 + i * 2].Image = m_Numbers[data.Scores[i] % 10];
		}


		if (m_bGameIsDone)
		{	
			m_bLeftTeamWin = data.bLeftTeamWon;
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

		if (m_bGameIsDone)
		{
			unsigned int ID = m_Game->GetID();
			if (m_bLeftTeamWin)
			{
				std::cout << ID << "LEFTTEAM WIN!!" << std::endl;
			}
			
			
			if (ID == 0 || ID == 1)
			{
				if (m_bLeftTeamWin)
				{
					m_ResultString = "WIN";
					m_ResultBoxColor = glm::vec4(129.0f / 255.0f, 193.0f / 255.0f, 71.0f / 255.0f, 1.0f);
				}
				else
				{
					m_ResultString = "LOSE";
					m_ResultBoxColor = glm::vec4(255.0f / 255.0f, 40.0f / 255.0f, 0.0f, 1.0f);
				}
			}
			else
			{
				if (!m_bLeftTeamWin)
				{
					m_ResultString = "WIN";
					m_ResultBoxColor = glm::vec4(129.0f / 255.0f, 193.0f / 255.0f, 71.0f / 255.0f, 1.0f);
				}
				else
				{
					m_ResultString = "LOSE";
					m_ResultBoxColor = glm::vec4(255.0f / 255.0f, 40.0f / 255.0f, 0.0f, 1.0f);
				}
			}



			Renderer::Quad gameResultQuad;
			gameResultQuad.bUseColor = true;
			gameResultQuad.bUseTexture = false;
			gameResultQuad.Position = glm::vec2(432 / 2.0f, 304.0f / 2.0f + 20.0f);
			gameResultQuad.Color = m_ResultBoxColor;
			gameResultQuad.Size = glm::vec2(432.0 / 2.0f, 304.0f/ 4.0f);
			renderer->DrawQuad(gameResultQuad);


			int advance = 0;
			Renderer::Quad ftQuad = {};
			for (int i = 0; i < m_ResultString.size(); i++)
			{
				Font ft = m_FontData->GetFont(m_ResultString[i]);
				ftQuad.bUseTexture = false;
				ftQuad.bUseFont = true;
				ftQuad.Position = glm::vec2(432 / 2.0f - 25.0f + advance, 304.0f / 2.0f + 10.0f );
				ftQuad.Font = ft;
				ftQuad.bUseColor = true;
				ftQuad.Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
				advance += ftQuad.Font.Advance;

				renderer->DrawQuad(ftQuad);
			}
		}
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
	Texture m_PickachuWin[5];
	Texture m_PickachuLose[5];
	Texture m_BallPowerHiting[2];
	Texture m_BallIdle[5];

	ServerToClientInGame m_Data = {};

	bool m_bGameIsDone = false;
	bool m_bLeftTeamWin = false;
	std::string m_ResultString;
	glm::vec4 m_ResultBoxColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	FontData* m_FontData = nullptr;
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
		ClientToServerInLobby data = {};

		auto& charQueue = m_Game->GetCharQueue();
		if (charQueue.empty())
		{
			// Send Empty Input
		}
		else
		{
			if(m_String.size() < 45)
				m_String += charQueue.front();
			// Send User Input
			charQueue.pop();
		}
		UserInput input = {};
		auto& inputQueue = m_Game->GetInputQueue();
		input.Key = -1;
		if (!inputQueue.empty())
		{
			input = inputQueue.front();
			inputQueue.pop();
		}

		if (input.Action == GLFW_PRESS && input.Key == GLFW_KEY_ENTER)
		{
			wcscpy(data.Chat, m_String.c_str());
			m_String.clear();
		}
		else if ((input.Action == GLFW_PRESS || input.Action == GLFW_REPEAT)
			&& input.Key == GLFW_KEY_BACKSPACE
			&& !m_String.empty())
		{
			m_String.pop_back();
		}
		else
		{
			wcscpy(data.Chat, L"");
		}
		data.ID = m_Game->GetID();
		Network::GetInstance()->Send(m_Game->GetSocket(), (char*)&data, sizeof(data));

	}
	virtual void ReceiveData() override
	{
		ServerToClientInLobby data = {};
		Network::GetInstance()->Recv(m_Game->GetSocket(), (char*)&data, sizeof(data));
		memcpy(m_Chats, data.Chats, sizeof(data.Chats));
		m_Game->SetID(data.ID);

		if (data.bShouldStartMatch)
			m_Game->SetGameState(new GameState(m_Game));
	}

	virtual void Render() override
	{
		Renderer* renderer = m_Game->GetRenderer();
		for (int i = 0; i < _countof(m_BackGroundTiles); i++)
			renderer->DrawQuad(m_BackGroundTiles[i]);
		
		renderer->DrawQuad(m_ChatBoard);

		Renderer::Quad ftQuad;
	
		int advance = 0;
		for (int i = 0; i < _countof(m_Chats); i++)
		{
			advance = 0;
			for (int j = 0; j < _countof(m_Chats[i].Line); j++)
			{
				Font ft = m_FontData->GetFont(m_Chats[i].Line[j]);
				ftQuad.bUseTexture = false;
				ftQuad.bUseFont = true;
				ftQuad.Position = glm::vec2(20.0f + advance, 304.0f - 30.0f - i * 15.0f);
				ftQuad.Font = ft;
				ftQuad.bUseColor = true;
				ftQuad.Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
				advance += ftQuad.Font.Advance;
				renderer->DrawQuad(ftQuad);
			}
		}

		Renderer::Quad chatRegionQuad;
		chatRegionQuad.bUseColor = true;
		chatRegionQuad.bUseTexture = false;
		chatRegionQuad.Position = glm::vec2(432 / 2.0f, 8.0f + 16.0f * 1.0f + 2.0f);
		chatRegionQuad.Color = glm::vec4(0.2f, 0.5f, 0.2f, 0.8f);
		chatRegionQuad.Size = glm::vec2(432 - 20 - 20, (304 - 20) / 15.0f);
		renderer->DrawQuad(chatRegionQuad);

		advance = 0;
		for (int i = 0; i < m_String.size(); i++)
		{
			Font ft = m_FontData->GetFont(m_String[i]);
			ftQuad.bUseTexture = false;
			ftQuad.bUseFont = true;
			ftQuad.Position = glm::vec2(20.0f + advance, 16.0f * 1.0f + 3.0f);
			ftQuad.Font = ft;
			ftQuad.bUseColor = true;
			ftQuad.Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
			advance += ftQuad.Font.Advance;

			renderer->DrawQuad(ftQuad);
		}

		static bool bCarretBlink = false;
		static float carretBlinkTimer = 0.0f;
		carretBlinkTimer += 0.016f * 2;
		if (carretBlinkTimer >= 1.0f)
		{
			carretBlinkTimer = 0.0f;
			bCarretBlink = !bCarretBlink;
		}

		Font ft = m_FontData->GetFont('|');
		ftQuad.bUseTexture = false;
		ftQuad.bUseFont = true;
		ftQuad.Position = glm::vec2(20.0f + advance + 1.0f, 16.0f * 1.0f + 3.0f);
		ftQuad.Font = ft;
		ftQuad.bUseColor = true;

		ftQuad.Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		if (bCarretBlink)
			ftQuad.Color.a = 0.0f;
		else
			ftQuad.Color.a = 1.0f;

		renderer->DrawQuad(ftQuad);
	}

private:
	Renderer::Quad m_BackGroundTiles[20];
	Renderer::Quad m_ChatBoard;
	FontData* m_FontData;
	ServerToClientInLobby m_Data = {};
	std::wstring m_String;
	ChatLine m_Chats[16] = {};
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

		m_ChatBoard.bUseColor = true;
		m_ChatBoard.bUseTexture = false;
		m_ChatBoard.Position = glm::vec2(432 / 2.0f, 304 / 2.0f + 20.0f);
		m_ChatBoard.Color = glm::vec4(0.3f, 0.3f, 0.3f, 0.8f);
		m_ChatBoard.Size = glm::vec2(200, 50);

		m_LoginBoard.bUseColor = true;
		m_LoginBoard.bUseTexture = false;
		m_LoginBoard.Position = glm::vec2(432 / 2.0f, 304 / 2.0f + 20.0f);
		m_LoginBoard.Color = glm::vec4(1.0f, 1.0f, 1.0f, 0.8f);
		m_LoginBoard.Size = glm::vec2(180, 30);

		m_FontData = new FontData("assets/fonts/vt323/VT323-Regular.ttf", 20);


	}
	virtual ~LoginState() = default;

	virtual void SendData() override
	{
		ClientToServerInLogin data = {};

		auto& charQueue = m_Game->GetCharQueue();
		if (charQueue.empty())
		{
			// Send Empty Input
		}
		else
		{
			if(m_String.size() < 18)
				m_String += charQueue.front();
			// Send User Input
			charQueue.pop();
		}
		UserInput input = {};
		auto& inputQueue = m_Game->GetInputQueue();
		input.Key = -1;
		if (!inputQueue.empty())
		{
			input = inputQueue.front();
			inputQueue.pop();
		}

		if (input.Action == GLFW_PRESS && input.Key == GLFW_KEY_ENTER)
		{
			wcscpy(data.NickName, m_String.c_str());
			m_String.clear();
		}
		else if ((input.Action == GLFW_PRESS || input.Action == GLFW_REPEAT) 
			&& input.Key == GLFW_KEY_BACKSPACE
			&& !m_String.empty())
		{
			m_String.pop_back();
		}
		else
		{
			wcscpy(data.NickName, L"");
		}
		data.ID = m_Game->GetID();
		Network::GetInstance()->Send(m_Game->GetSocket(), (char*)&data, sizeof(data));
	}
	virtual void ReceiveData() override
	{
		ServerToClientInLogin data = {};
		Network::GetInstance()->Recv(m_Game->GetSocket(), (char*)&data, sizeof(data));
		m_Game->SetID(data.ID);

		m_bResult = data.Result;	
		if (m_bResult == LoginResult::Failed)
			m_bShouldShowFailedMessage = true;

		if (m_bResult == LoginResult::Succeded)
		{
			m_Game->SetGameState(new LobbyState(m_Game));
		}
	}

	virtual void Render() override
	{
		Renderer* renderer = m_Game->GetRenderer();
		for (int i = 0; i < _countof(m_BackGroundTiles); i++)
			renderer->DrawQuad(m_BackGroundTiles[i]);

		renderer->DrawQuad(m_ChatBoard);
		renderer->DrawQuad(m_LoginBoard);

		Renderer::Quad ftQuad;

		int advance = 0;
		for (int i = 0; i < m_String.size(); i++)
		{
			Font ft = m_FontData->GetFont(m_String[i]);
			ftQuad.bUseTexture = false;
			ftQuad.bUseFont = true;
			ftQuad.Position = glm::vec2(432 / 2.0f - 100.0f + 20.0f + advance, 304 / 2.0f + 15.0f);
			ftQuad.Font = ft;
			ftQuad.bUseColor = true;
			ftQuad.Color = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
			advance += ftQuad.Font.Advance;

			renderer->DrawQuad(ftQuad);
		}

		static bool bCarretBlink = false;
		static float carretBlinkTimer = 0.0f;
		carretBlinkTimer += 0.016f * 2;
		if (carretBlinkTimer >= 1.0f)
		{
			carretBlinkTimer = 0.0f;
			bCarretBlink = !bCarretBlink;
		}


		Font ft = m_FontData->GetFont('|');
		ftQuad.bUseTexture = false;
		ftQuad.bUseFont = true;
		ftQuad.Position = glm::vec2(432 / 2.0f - 100.0f + 20.0f + advance + 1.0f, 304 / 2.0f + 15.0f);
		ftQuad.Font = ft;
		ftQuad.bUseColor = true;
		
		ftQuad.Color = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
		if (bCarretBlink)
			ftQuad.Color.a = 0.0f;
		else
			ftQuad.Color.a = 1.0f;

		renderer->DrawQuad(ftQuad);


		if (m_bShouldShowFailedMessage)
		{
			Renderer::Quad resultQuad = {};
			resultQuad.bUseColor = true;
			resultQuad.bUseTexture = false;
			resultQuad.Position = glm::vec2(432 / 2.0f, 304 / 2.0f + 20.0f - 50.0f);
			resultQuad.Color = glm::vec4(1.0f, 1.0f, 1.0f, 0.8f);
			resultQuad.Size = glm::vec2(220, 30);
			renderer->DrawQuad(resultQuad);


			std::string resultString = "NickName Is Already Taken";
			advance = 0;
			for (int i = 0; i < resultString.size(); i++)
			{
				Font ft = m_FontData->GetFont(resultString[i]);
				ftQuad.bUseTexture = false;
				ftQuad.bUseFont = true;
				ftQuad.Position = glm::vec2(432 / 2.0f - 100.0f + advance, 304 / 2.0f + 15.0f - 50.0f);
				ftQuad.Font = ft;
				ftQuad.bUseColor = true;
				ftQuad.Color = glm::vec4(0.9f, 0.1f, 0.1f, 0.8f);
				advance += ftQuad.Font.Advance;

				renderer->DrawQuad(ftQuad);
			}
		}

	}

private:
	Renderer::Quad m_BackGroundTiles[20];
	Renderer::Quad m_ChatBoard;
	Renderer::Quad m_LoginBoard;
	ServerToClientInLogin m_Data = {};
	FontData* m_FontData = nullptr;
	std::wstring m_String;

	LoginResult m_bResult;
	bool m_bShouldShowFailedMessage = false;
};


class PickachuVolleyBall : public Game
{
public:
	PickachuVolleyBall(int width, int height, const std::string& serverIP) : Game(width, height, serverIP)
	{
		// Pre Load Textures
		TextureManager* textureManager = TextureManager::GetInstance();
		textureManager->LoadTextureAtlas("assets/textures/sprite_sheet.json", "assets/textures/sprite_sheet.png");

		SetGameState(new LoginState(this));
	}
	virtual ~PickachuVolleyBall() = default;

};

RegisterApplication(PickachuVolleyBall, 432, 304)