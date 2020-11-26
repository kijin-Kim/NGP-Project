#pragma once
#include <glm/glm.hpp>
#include <Network/Data.h>
#include <GLFW/glfw3.h>
#include <string>

#define GRAVITY 9.8f * 40.0f

class GameObject
{
public:
	GameObject() = default;
	virtual ~GameObject() = default;

	void SetPosition(const glm::vec2& position) { m_Position = position; }
	const glm::vec2& GetPosition() const { return m_Position; }

	void SetTag(const std::string& tag) { m_Tag = tag; }
	const std::string& GetTag() const { return m_Tag; }

	void SetAnimationIndex(float index) { m_AnimationIndex = index; }
	float GetAnimationIndex() const { return m_AnimationIndex; }

	unsigned int GetState() const { return m_State; }
	void SetState(unsigned int state) { m_State = state; }

	glm::vec2 GetVelocity() const { return m_Velocity; }
	void SetVelocity(const glm::vec2& val) { m_Velocity = val; }
private:
	glm::vec2 m_Position = glm::vec2(0.0f, 0.0f);
	glm::vec2 m_Velocity = glm::vec2(0.0f, 0.0f);
	std::string m_Tag = "Default";
	float m_AnimationIndex = 0.0f;
	unsigned int m_State = 0;
};

class Pickachu : public GameObject
{
public:
	Pickachu()
	{
		SetTag("Pickachu");
	}

	virtual ~Pickachu() = default;

	void Update(float deltaTime, UserInput input)
	{
		
		if (GetPosition().y <= 16.0f * 4.0f)
			SetState(Idle);

		switch (GetState())
		{
		case Walking:
		{
			float currentAnimationIndex = GetAnimationIndex();
			SetAnimationIndex(currentAnimationIndex + 5.0f * deltaTime);
			if (GetAnimationIndex() > 5.0f)
				SetAnimationIndex(0.0f);
			break;
		}
		case Idle:
		{
			float currentAnimationIndex = GetAnimationIndex();
			SetAnimationIndex(currentAnimationIndex + 5.0f * deltaTime);
			if (GetAnimationIndex() > 5.0f)
				SetAnimationIndex(0.0f);
			break;
		}
		case Jumping:
		{
			float currentAnimationIndex = GetAnimationIndex();
			SetAnimationIndex(currentAnimationIndex + 30.0f * deltaTime);
			if (GetAnimationIndex() > 4.0f)
				SetAnimationIndex(0.0f);
			break;
		}
		default:
			break;
		}
		switch (input.Key)
		{
		case GLFW_KEY_LEFT:
			if (input.Action == GLFW_PRESS || input.Action == GLFW_REPEAT)
			{
				if (GetState() != Jumping)
				{
					SetState(Walking);
				}
				SetVelocity({ -150.0f, GetVelocity().y });
			}
			if (input.Action == GLFW_RELEASE)
			{
				if (GetState() != Jumping)
				{
					SetState(Idle);
				}
				SetVelocity({ 0.0f, GetVelocity().y });
			}
			break;
		case GLFW_KEY_RIGHT:
			if (input.Action == GLFW_PRESS || input.Action == GLFW_REPEAT)
			{
				if (GetState() != Jumping)
				{
					SetState(Walking);
				}
				SetVelocity({ 150.0f, GetVelocity().y });
			}
			if (input.Action == GLFW_RELEASE)
			{
				if (GetState() != Jumping)
				{
					SetState(Idle);
				}
				SetVelocity({ 0.0f, GetVelocity().y });
			}
			break;
		case GLFW_KEY_UP:
			if (input.Action == GLFW_PRESS)
			{
				if (GetState() != Jumping)
				{
					SetState(Jumping);
					SetVelocity({ GetVelocity().x, 300.0f });
				}
			}
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

		SetVelocity({ GetVelocity().x, GetVelocity().y - (GRAVITY * deltaTime) });
		SetPosition(GetPosition() + GetVelocity() * deltaTime);
	}
private:
	bool m_bIsNotOnGround = false;
};

class Ball : public GameObject
{
public:
	Ball()
	{
		SetTag("Ball");
		SetVelocity({ 100.0f, 100.0f });
	}

	virtual ~Ball() = default;

	void Update(float deltaTime)
	{
		//SetVelocity({ GetVelocity().x, GetVelocity().y - (GRAVITY * deltaTime) });

		float currentAnimationIndex = GetAnimationIndex();
		SetAnimationIndex(currentAnimationIndex + 2.0f * deltaTime);
		if (GetAnimationIndex() > 4.0f)
			SetAnimationIndex(0.0f);
		

		if (GetPosition().y < 16.0f * 4.0f)
		{
			SetPosition({ GetPosition().x, 16.0f * 4.0f });
			SetVelocity({ GetVelocity().x, -GetVelocity().y });
		}
		else if (GetPosition().y > 16.0f * 18.0f)
		{
			SetPosition({ GetPosition().x, 16.0f * 18.0f });
			SetVelocity({ -GetVelocity().x, -GetVelocity().y });
		}

		if (GetPosition().x < 16.0f * 2.0f  )
		{
			SetPosition({ 16.0f * 2.0f, GetPosition().y });
			SetVelocity({ -GetVelocity().x, GetVelocity().y });
		}
		else if (GetPosition().x > 16.0f * 24.0f)
		{
			SetPosition({ 16.0f * 24.0f, GetPosition().y });
			SetVelocity({ -GetVelocity().x, GetVelocity().y });
		}

		SetPosition(GetPosition() + GetVelocity() * deltaTime);
	}
};
