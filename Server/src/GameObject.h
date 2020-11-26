#pragma once
#include <glm/glm.hpp>
#include <Network/Data.h>
#include <GLFW/glfw3.h>
#include <string>

#define GRAVITY 9.8f * 40.0f

struct AABBBox
{
	float Left = 0.0f;
	float Top = 0.0f;
	float Right = 0.0f;
	float Bottom = 0.0f;
};

class GameObject
{
public:
	GameObject() = default;
	virtual ~GameObject() = default;

	void SetPosition(const glm::vec2& position) 
	{ 
		m_Position = position; 
		m_AABB.Left = m_Position.x - m_Size.x / 2.0f;
		m_AABB.Top = m_Position.y + m_Size.y / 2.0f;
		m_AABB.Right = m_Position.x + m_Size.x / 2.0f;
		m_AABB.Bottom = m_Position.y - m_Size.y / 2.0f;
	}
	const glm::vec2& GetPosition() const { return m_Position; }

	void SetTag(const std::string& tag) { m_Tag = tag; }
	const std::string& GetTag() const { return m_Tag; }

	void SetAnimationIndex(float index) { m_AnimationIndex = index; }
	float GetAnimationIndex() const { return m_AnimationIndex; }

	unsigned int GetState() const { return m_State; }
	void SetState(unsigned int state) { m_State = state; }

	const glm::vec2& GetVelocity() const { return m_Velocity; }
	void SetVelocity(const glm::vec2& val) { m_Velocity = val; }

	void SetAABBBox(const AABBBox& aabb) { m_AABB = aabb; }
	const AABBBox& GetAABBBox() const { return m_AABB; }

	void SetSize(const glm::vec2& size) { m_Size = size; }
	const glm::vec2& GetSize() const { return m_Size; }
	
private:
	glm::vec2 m_Position = glm::vec2(0.0f, 0.0f);
	glm::vec2 m_Velocity = glm::vec2(0.0f, 0.0f);
	glm::vec2 m_Size = glm::vec2(0.0f, 0.0f);
	std::string m_Tag = "Default";
	float m_AnimationIndex = 0.0f;
	unsigned int m_State = 0;

	AABBBox m_AABB = {};
};

class Pickachu : public GameObject
{
public:
	Pickachu()
	{
		SetTag("Pickachu");
		SetSize({ 16.0f, 32.0f});
	}

	virtual ~Pickachu() = default;

	void Update(float deltaTime, UserInput input)
	{

		static float powerHitRemainTimer = 0.0f;
		static float powerHitCoolTimer = 0.3f;
		
		if (GetPosition().y <= 16.0f * 4.0f)
			SetState(PickachuState::Pickachu_Idle);

		switch (GetState())
		{
		case PickachuState::Pickachu_Walking:
		{
			float currentAnimationIndex = GetAnimationIndex();
			SetAnimationIndex(currentAnimationIndex + 5.0f * deltaTime);
			if (GetAnimationIndex() > 5.0f)
				SetAnimationIndex(0.0f);
			break;
		}
		case PickachuState::Pickachu_Idle:
		{
			float currentAnimationIndex = GetAnimationIndex();
			SetAnimationIndex(currentAnimationIndex + 5.0f * deltaTime);
			if (GetAnimationIndex() > 5.0f)
				SetAnimationIndex(0.0f);
			break;
		}
		case PickachuState::Pickachu_Jumping:
		{
			powerHitCoolTimer += deltaTime;
			float currentAnimationIndex = GetAnimationIndex();
			SetAnimationIndex(currentAnimationIndex + 30.0f * deltaTime);
			if (GetAnimationIndex() > 4.0f)
				SetAnimationIndex(0.0f);
			break;
		}
		case PickachuState::Pickachu_PowerHiting:
		{
			float currentAnimationIndex = GetAnimationIndex();
			SetAnimationIndex(currentAnimationIndex + 10.0f * deltaTime);
			if (GetAnimationIndex() > 2.0f)
				SetAnimationIndex(0.0f);

			powerHitRemainTimer += deltaTime;

			if (powerHitRemainTimer >= 1.0f)
			{
				SetState(Pickachu_Jumping);
				powerHitRemainTimer = 0.0f;
			}
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
				if (GetState() != PickachuState::Pickachu_Jumping && GetState() != PickachuState::Pickachu_PowerHiting)
				{
					SetState(PickachuState::Pickachu_Walking);
				}
				SetVelocity({ -150.0f, GetVelocity().y });
			}
			if (input.Action == GLFW_RELEASE)
			{
				if (GetState() != PickachuState::Pickachu_Jumping && GetState() != PickachuState::Pickachu_PowerHiting)
				{
					SetState(PickachuState::Pickachu_Idle);
				}
				SetVelocity({ 0.0f, GetVelocity().y });
			}
			break;
		case GLFW_KEY_RIGHT:
			if (input.Action == GLFW_PRESS || input.Action == GLFW_REPEAT)
			{
				if (GetState() != PickachuState::Pickachu_Jumping && GetState() != PickachuState::Pickachu_PowerHiting)
				{
					SetState(PickachuState::Pickachu_Walking);
				}
				SetVelocity({ 150.0f, GetVelocity().y });
			}
			if (input.Action == GLFW_RELEASE)
			{
				if (GetState() != PickachuState::Pickachu_Jumping && GetState() != PickachuState::Pickachu_PowerHiting)
				{
					SetState(PickachuState::Pickachu_Idle);
				}
				SetVelocity({ 0.0f, GetVelocity().y });
			}
			break;
		case GLFW_KEY_UP:
			if (input.Action == GLFW_PRESS)
			{
				if (GetState() != PickachuState::Pickachu_Jumping && GetState() != PickachuState::Pickachu_PowerHiting)
				{
					SetState(Pickachu_Jumping);
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
				if (GetState() == PickachuState::Pickachu_Jumping)
				{
					if (powerHitCoolTimer >= 0.3f)
						powerHitCoolTimer = 0.0f;
					else
						break;
					SetState(PickachuState::Pickachu_PowerHiting);
				}
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
		SetVelocity({ 50.0f, 50.0f });
		SetSize({ 32.0f, 32.0f});
		SetState(BallState::Ball_Idle);
	}

	virtual ~Ball() = default;

	void Update(float deltaTime)
	{
		SetVelocity({ GetVelocity().x, GetVelocity().y - (GRAVITY / 10.0f * deltaTime) });

		switch (GetState())
		{
		case BallState::Ball_Idle:
		{
			float currentAnimationIndex = GetAnimationIndex();
			SetAnimationIndex(currentAnimationIndex + 2.0f * deltaTime);
			if (GetAnimationIndex() > 4.0f)
				SetAnimationIndex(0.0f);
			break;
		}
		case BallState::Ball_PowerHiting:
		{
			float currentAnimationIndex = GetAnimationIndex();
			SetAnimationIndex(currentAnimationIndex + 2.0f * deltaTime);
			if (GetAnimationIndex() > 3.0f)
				SetAnimationIndex(0.0f);
			break;
		}
			
		default:
			break;
		}
		
	
		if (GetPosition().y < 16.0f * 4.0f)
		{
			SetPosition({ GetPosition().x, 16.0f * 4.0f });
			SetVelocity({ GetVelocity().x, -GetVelocity().y });
		}
		else if (GetPosition().y > 16.0f * 19.0f)
		{
			SetPosition({ GetPosition().x, 16.0f * 19.0f });
			SetVelocity({ GetVelocity().x, -GetVelocity().y });
		}

		if (GetPosition().x < 16.0f * 1.0f  )
		{
			SetPosition({ 16.0f * 1.0f, GetPosition().y });
			SetVelocity({ -GetVelocity().x, GetVelocity().y });
		}
		else if (GetPosition().x > 16.0f * 25.0f)
		{
			SetPosition({ 16.0f * 25.0f, GetPosition().y });
			SetVelocity({ -GetVelocity().x, GetVelocity().y });
		}
		SetPosition(GetPosition() + GetVelocity() * deltaTime);
		if(GetState() == BallState::Ball_Idle)
			SetVelocity(glm::clamp(GetVelocity(), { -100.0f, -100.0f }, { 100.0f, 100.0f }));
	}
};
