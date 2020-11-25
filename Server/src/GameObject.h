#pragma once
#include <glm/glm.hpp>
#include <Network/Data.h>
#include <GLFW/glfw3.h>
#include <string>

class GameObject
{
public:
	GameObject() = default;
	virtual ~GameObject() = default;

	void SetPosition(const glm::vec2& position) { m_Position = position; }
	const glm::vec2& GetPosition() const { return m_Position; }

	void SetTag(const std::string& tag) { m_Tag = tag; }
	const std::string& GetTag() const { return m_Tag; }

	void SetAnimationIndex(unsigned int index) { m_AnimationIndex = index; }
	unsigned int GetAnimationIndex() const { return m_AnimationIndex; }

	unsigned int GetState() const { return m_State; }
	void SetState(unsigned int state) { m_State = state; }

private:
	glm::vec2 m_Position = glm::vec2(0.0f, 0.0f);
	std::string m_Tag = "Default";
	unsigned int m_AnimationIndex = 0;
	unsigned int m_State = 0;
};

class Pickachu : public GameObject
{
public:
	enum PickachuState
	{
		Idle = 0, Jumping, Walking, Sliding
	};


	Pickachu()
	{
		SetTag("Pickachu");
	}

	virtual ~Pickachu() = default;

	void Update(float deltaTime, UserInput input)
	{
		unsigned int currentAnimationIndex = GetAnimationIndex();
		SetAnimationIndex(currentAnimationIndex + 1.0f * deltaTime);

		if (input.Key == GLFW_KEY_UNKNOWN)
			return;

		switch (input.Key)
		{
		case GLFW_KEY_LEFT:
			if (input.Action == GLFW_PRESS)
			{
				glm::vec2 currentPosition = GetPosition();
				SetPosition({ currentPosition.x + 1.0f, currentPosition.y });
			}
				break;
			if (input.Action == GLFW_REPEAT)
				break;
			if (input.Action == GLFW_RELEASE)
				break;
			break;
		case GLFW_KEY_RIGHT:
			if (input.Action == GLFW_PRESS)
				break;
			if (input.Action == GLFW_REPEAT)
				break;
			if (input.Action == GLFW_RELEASE)
				break;
			break;
		case GLFW_KEY_UP:
			if (input.Action == GLFW_PRESS)
				break;
			if (input.Action == GLFW_REPEAT)
				break;
			if (input.Action == GLFW_RELEASE)
				break;
			break;
		case GLFW_KEY_SPACE:
			if (input.Action == GLFW_PRESS)
				break;
			if (input.Action == GLFW_REPEAT)
				break;
			if (input.Action == GLFW_RELEASE)
				break;
			break;

		default:
			break;
		}
	}
};

class Ball : public GameObject
{
public:
	Ball()
	{
		SetTag("Ball");
	}

	virtual ~Ball() = default;

	void Update(float deltaTime) {}


};
