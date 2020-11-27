#pragma once
#include <cstdint>

//////////////////////// GLOBAL ENUM, DEFINES //////////////////////////////

#define MAX_GAME_SCORE 5

enum LoginResult
{
	None = 0, Failed, Succeded
};

enum PickachuState
{
	Pickachu_Idle = 0, Pickachu_Jumping, Pickachu_Walking, Pickachu_PowerHiting
};

enum BallState
{
	Ball_Idle = 4, Ball_PowerHiting
};


#pragma pack(push)
#pragma pack(1)

struct IData  //데이터들을 하나의 컨테이너에 넣기 위한 인터페이스 클래스
{
	int8_t ID = -1;        //클라이언트를 구분하기 위한 id
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////INGAME//////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct UserInput
{
	int32_t Key; //클라이언트의 입력 값
	uint8_t Action; //클라이언트의 입력 값 상태 PRESSED, RELEASED, REPEATED
};

struct ClientToServerInGame : public IData //서버가 게임에서 Client에게 넘겨줄(Client가 렌더링할때 필요한) 데이터를 계산하기 위해 필요한 데이터
{
	UserInput Input;
};


struct float2
{
	float X = 0;
	float Y = 0;
};

struct Animation
{
	uint8_t State;  // 특정 오브젝트의 상태(예) 점프, 슬라이딩, Idle 등)
	uint8_t AnimationIndex; //특정 상태에서 출력해야되는 2D 스프라이트의 인덱스
};

struct ServerToClientInGame : public IData //Client가 게임을 렌더링할 때 필요한 계산된 데이터
{
	float2 ObjectPositions[5]; //게임내의 모든 오브젝트의 위치(플레이어 4명, 공 1개)
	uint8_t Scores[2];     //게임내의 팀의 점수(순서대로 좌측 팀, 우측 팀)
	Animation AnimationData[5];  //애니메이션 데이터
	bool bLeftTeamWon = false; // 게임이 끝났을 때 (어느 한 쪽팀이 목표점수에 도달하였을 때), 확인하는 Boolean, True 왼쪽팀 승리 False 오른쪽팀 승리
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////INLOBBY/////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


struct ClientToServerInLobby : public IData  //서버가 로비에서 Client에게 넘겨줄(Client가 렌더링할때 필요한) 데이터를 계산하기 위해 필요한 데이터
{
	wchar_t Chat[256] = {};
};

struct ChatLine
{
	wchar_t Line[50] = {};
};

struct ServerToClientInLobby : public IData //Client가 로비를 렌더링할 때 필요한 계산된 데이터
{
	ChatLine Chats[16] = {};
	bool bShouldStartMatch = false;
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////INLOGIN//////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


struct ClientToServerInLogin : public IData //클라이언트가 서버에게 보낼 닉네임
{
	wchar_t NickName[20] = {};
};
struct ServerToClientInLogin : public IData //서버가 클라이언트에게 보내는 로그인 성공여부(중복 시, 실패)
{
	LoginResult Result = None;
};


#pragma pack(pop)