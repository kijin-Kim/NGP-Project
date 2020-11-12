#pragma once
#include <unordered_map>
#include <iostream>
#include <fstream>
#include "json/json.h"
#include "Texture.h"

class TextureManager final
{
public:
	static TextureManager* GetInstance() { static TextureManager instance; return &instance; }

	void LoadTexture(const std::string& texturePath, const std::string& alias = "")
	{
		std::string textureAlias;
		textureAlias = alias == "" ? texturePath : alias;
		if (m_TextureCache.find(textureAlias) != m_TextureCache.end())
		{
			return;
		}

		m_TextureCache[textureAlias] = new Texture(texturePath);
	}

	void LoadTextureAtlas(const std::string& jsonFilePath)
	{
		std::ifstream jsonFile(jsonFilePath);
		Json::CharReaderBuilder builder;
		builder["collectComments"] = false;
		Json::Value value;

		JSONCPP_STRING errs;
		bool bIsOk = parseFromStream(builder, jsonFile, &value, &errs);
		if (!bIsOk)
			assert(false && "Json File을 파싱하는데 실패하였습니다.");

		auto a = value["frames"]
		/*for (int i = 0; i < value["frames"].size(); i++)
		{
			std::cout << value["frames"][i].asCString() << std::endl;
		}*/
		


		/*if (bIsOk)
		{
			for (int i = 0; i < value["Actors"].size(); i++)
			{
				std::string actorType = value["Actors"][i]["ActorType"].asString();
				auto* actor = s_ActorCreationMap[actorType](renderer, layer, value);
				layer->GetWorld().push_back(actor);

				for (int j = 0; j < value["Actors"][i]["Components"].size(); j++)
				{
					std::string componentType = value["Actors"][i]["Components"][j]["ComponentType"].asString();
					s_ComponentCreationMap[componentType](actor, value["Actors"][i]["Components"][j]["ComponentProperties"]);
				}
			}
		}*/
	}

	Texture* GetTexture(const std::string& alias)
	{
		if (m_TextureCache.find(alias) == m_TextureCache.end())
			assert(false && "텍스쳐를 찾을 수 없습니다. 먼저 로드 해주세요");
		return m_TextureCache[alias];
	}


private:
	TextureManager() = default;
	~TextureManager() = default;

private:
	std::unordered_map<std::string, Texture*> m_TextureCache;
};
