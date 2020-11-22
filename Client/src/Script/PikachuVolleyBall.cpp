#pragma once
#include "Client/Entry.h"
#include "Client/State.h"
#include "Client/Renderer.h"

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
		Texture Pickchu = textureManager->GetTextureFromAtlas("assets/textures/sprite_sheet.png", "pikachu/pikachu_0_0.png");


		for (int x = 0; x < 27; x++)
		{
			for (int y = 0; y < 19; y++)
			{
				if (y== 4 || y == 5 || y == 6)
					continue;

				m_SceneQuads[y + 19 * x].Position = glm::vec3(8.0f + 16.0f * x, 8.0f + 16.0f * y, 0.0f);

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
		
		m_SceneQuads[27 * 19].Position = glm::vec3(8.0f + 16.0f * 13, 8.0f + 16.0f * 5, 0.0f);
		m_SceneQuads[27 * 19].Image = mountainTile;


		for (int i = 0; i < 12; i++)
		{
			if(i != 11)
				m_NetQuads[i].Image = netPillar;
			else
				m_NetQuads[i].Image = netPillarTop;

			m_NetQuads[i].Position = glm::vec3(8.0f + 16.0f * 13, 40.0f + 8.0f * i, 0.0f);
		}
		
		m_ObjectsQuad[0].Position = glm::vec3(8.0f + 16.0f * 7, 16.0f * 4, 0.0f);
		m_ObjectsQuad[0].Image = Pickchu;

		m_ObjectsQuad[1].Position = glm::vec3(8.0f + 16.0f * (7 + 13), 16.0f * 4, 0.0f);
		m_ObjectsQuad[1].Image = Pickchu;
		m_ObjectsQuad[1].bShouldFlipVertical = true;
	} 

	virtual ~GameState() = default;


	virtual void SendData() override
	{
		UserInput input = {};
		auto inputQueue = m_Game->GetInputQueue();
		input.Key = -1;
		if (inputQueue.empty())
		{
			// Send Empty Input
		}
		else
		{
			input = inputQueue.front();
			// Send User Input
			inputQueue.pop();
		}
	}
	virtual void ReceiveData() override
	{

	}

	virtual void Render() override
	{
		Renderer::RegisterQuads(m_SceneQuads, _countof(m_SceneQuads));
		Renderer::RegisterQuads(m_NetQuads, _countof(m_NetQuads));
		Renderer::RegisterQuads(m_ObjectsQuad, _countof(m_ObjectsQuad));
	}

private:
	Renderer::Quad m_SceneQuads[27 * 19 + 1];
	Renderer::Quad m_NetQuads[12];
	Renderer::Quad m_ObjectsQuad[3];


	ServerToClientInGame m_Data;
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
	}
	virtual ~PickachuVolleyBall() = default;

};

RegisterApplication(PickachuVolleyBall, 432, 304)