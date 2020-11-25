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
		Texture skyBlueTile = textureManager->GetTextureFromAtlas("assets/textures/sprite_sheet.png", "objects/sky_blue.png");
		Texture mountainTile = textureManager->GetTextureFromAtlas("assets/textures/sprite_sheet.png", "objects/mountain.png");
		Texture groundTile = textureManager->GetTextureFromAtlas("assets/textures/sprite_sheet.png", "objects/ground_yellow.png");
		Texture redGroundTile = textureManager->GetTextureFromAtlas("assets/textures/sprite_sheet.png", "objects/ground_red.png");
		Texture groundLine = textureManager->GetTextureFromAtlas("assets/textures/sprite_sheet.png", "objects/ground_line.png");
		Texture groundLineLeft = textureManager->GetTextureFromAtlas("assets/textures/sprite_sheet.png", "objects/ground_line_leftmost.png");
		Texture groundLineRight = textureManager->GetTextureFromAtlas("assets/textures/sprite_sheet.png", "objects/ground_line_rightmost.png");
		Texture netPillar = textureManager->GetTextureFromAtlas("assets/textures/sprite_sheet.png", "objects/net_pillar.png");
		Texture netPillarTop = textureManager->GetTextureFromAtlas("assets/textures/sprite_sheet.png", "objects/net_pillar_top.png");

		// TEMP
		Texture pickchu = textureManager->GetTextureFromAtlas("assets/textures/sprite_sheet.png", "pikachu/pikachu_0_0.png");


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


		for (int x = 0; x < 27; x++)
		{
			for (int y = 0; y < 19; y++)
			{
				if (y== 4 || y == 5 || y == 6)
					continue;

				m_SceneQuads[y + 19 * x].Position = glm::vec2(8.0f + 16.0f * x, 8.0f + 16.0f * y);

				if (y >= 4)
				{
					m_SceneQuads[y + 19 * x].Image = skyBlueTile;
				}
				else if (y == 3)
				{
					m_SceneQuads[y + 19 * x].Image = redGroundTile;
				}
				else if (y == 2)
				{
					if (x == 0)
						m_SceneQuads[y + 19 * x].Image = groundLineLeft;
					else if(x== 26)
						m_SceneQuads[y + 19 * x].Image = groundLineRight;
					else
						m_SceneQuads[y + 19 * x].Image = groundLine;
				}
				else
					m_SceneQuads[y + 19 * x].Image = groundTile;	
			}
		}
		
		m_SceneQuads[27 * 19].Position = glm::vec2(8.0f + 16.0f * 13, 8.0f + 16.0f * 5);
		m_SceneQuads[27 * 19].Image = mountainTile;


		for (int i = 0; i < 12; i++)
		{
			if(i != 11)
				m_NetQuads[i].Image = netPillar;
			else
				m_NetQuads[i].Image = netPillarTop;

			m_NetQuads[i].Position = glm::vec2(8.0f + 16.0f * 13, 40.0f + 8.0f * i);
		}
		
		m_ObjectsQuad[0].Position = glm::vec2(8.0f + 16.0f * 2, 16.0f * 4);
		m_ObjectsQuad[0].Image = pickchu;

		m_ObjectsQuad[1].Position = glm::vec2(8.0f + 16.0f * (2 + 4), 16.0f * 4);
		m_ObjectsQuad[1].Image = pickchu;
		m_ObjectsQuad[1].bUseColor = true;
		m_ObjectsQuad[1].Color = glm::vec4(0.5f, 0.5f, 0.0f, 1.0f);


		m_ObjectsQuad[2].Position = glm::vec2(8.0f + 16.0f * (19 - 2 + 4), 16.0f * 4);
		m_ObjectsQuad[2].Image = pickchu;
		m_ObjectsQuad[2].bShouldFlipVertical = true;
		m_ObjectsQuad[2].bUseColor = true;
		m_ObjectsQuad[2].Color = glm::vec4(0.5f, 0.5f, 0.0f, 1.0f);

		m_ObjectsQuad[3].Position = glm::vec2(8.0f + 16.0f * (19 - 2), 16.0f * 4);
		m_ObjectsQuad[3].Image = pickchu;
		m_ObjectsQuad[3].bShouldFlipVertical = true;
		m_ObjectsQuad[3].bUseColor = true;
		m_ObjectsQuad[3].Color = glm::vec4(0.5f, 0.5f, 0.0f, 1.0f);


		m_ScoreQuads[0].Position = glm::vec2(8.0f + 16.0f * 7, 16.0f * 17);
		m_ScoreQuads[0].Image = m_Numbers[0];

		m_ScoreQuads[1].Position = glm::vec2(8.0f + 16.0f * (7 + 13), 16.0f * 17);
		m_ScoreQuads[1].Image = m_Numbers[0];
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
		
		Network::GetInstance()->Send((char*)&data, 108);
	}
	virtual void ReceiveData() override
	{
		ServerToClientInGame data = {};
		Network::GetInstance()->Recv((char*)&data, 108);
	}

	virtual void Render() override
	{
		Renderer::RegisterQuads(m_SceneQuads, _countof(m_SceneQuads));
		Renderer::RegisterQuads(m_NetQuads, _countof(m_NetQuads));
		Renderer::RegisterQuads(m_ObjectsQuad, _countof(m_ObjectsQuad));
		Renderer::RegisterQuads(m_ScoreQuads, _countof(m_ScoreQuads));
	}

private:
	Renderer::Quad m_SceneQuads[27 * 19 + 1];
	Renderer::Quad m_NetQuads[12];
	Renderer::Quad m_ObjectsQuad[5];
	Renderer::Quad m_ScoreQuads[2];
	

	Texture m_Numbers[10];

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
		Renderer::RegisterQuads(m_BackGroundTiles, _countof(m_BackGroundTiles));
		Renderer::RegisterQuads(&m_ChatBoard, 1);

		
		std::vector<Renderer::Quad> ftQuads;
		ftQuads.resize(m_String.size());

		int advance = 0;
		int lineCount = 0;
		for (int i = 0; i < m_String.size(); i++)
		{
			Font ft = m_FontData->GetFont(m_String[i]);
			ftQuads[i].bUseTexture = false;
			ftQuads[i].bUseFont = true;
			ftQuads[i].Position = glm::vec2(20.0f + advance,  304.0f - 30.0f - lineCount * 15.0f);
			ftQuads[i].Font = ft;
			ftQuads[i].bUseColor = true;
			ftQuads[i].Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
			advance += ftQuads[i].Font.Advance;

			if (20.0f + advance >= 432.0f - 20.0f)
			{
				lineCount++;
				advance = 0.0f;
			}
		}

		
		Renderer::RegisterQuads(ftQuads);

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
	}
	virtual void ReceiveData() override
	{
	}

	virtual void Render() override
	{
		Renderer::RegisterQuads(m_BackGroundTiles, 20);
	}

private:
	Renderer::Quad m_BackGroundTiles[20];

	ServerToClientInLogin m_Data = {};
};


class PickachuVolleyBall : public Game
{
public:
	PickachuVolleyBall(int width, int height) : Game(width, height)
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