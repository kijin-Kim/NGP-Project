# 멀티플레이어 피카츄 배구 게임 (NGP-Project)

이 프로젝트는 2020년 2학기 네트워크 게임 프로그래밍 수업에서 진행한 팀 프로젝트입니다. Winsock과 OpenGL API를 직접 사용하여 2v2 실시간 온라인 피카츄 배구 게임을 C++로 구현했습니다. 네트워크 동기화, 그래픽스 렌더링 파이프라인, 게임 로직 등 멀티플레이어 게임의 핵심 요소를 직접 구현하며 학습하는 것을 목표로 했습니다.


## 주요 기능

* **실시간 2v2 멀티플레이:** Winsock(TCP) 기반 최대 4인 동시 접속 및 플레이 지원
* **핵심 게임 로직:** 캐릭터 이동, 점프, 기본 공격, 파워 히트(스파이크), 공 물리(중력 적용), 충돌 처리(AABB) 구현
* **게임 상태 관리:** 로그인 - 로비(채팅 기능 포함) - 인게임 상태 전환 관리
* **2D 그래픽스 렌더링:** OpenGL Core Profile 기반 렌더링

## 기술 스택

* **언어:** C++
* **네트워킹:** Winsock (TCP/IP)
* **그래픽스:** OpenGL 4.x (Core Profile), GLEW, GLFW
* **라이브러리:**
    * GLM (수학)
    * FreeType (폰트 렌더링)
    * JsonCpp (Texture Atlas 정보 파싱)
    * stb_image (이미지 로딩)
* **서버 기술:** Windows Threads, Critical Sections (동기화)
* **개발 환경:** Windows, Visual Studio

## 아키텍처 개요

### 클라이언트-서버 구조

* TCP/IP 소켓 통신을 기반으로 하는 클라이언트-서버 모델입니다.
* **서버 권위(Server-Authoritative)** 구조: 모든 게임 로직 연산(이동, 충돌, 점수 등)은 서버에서 수행되며, 클라이언트는 서버로부터 받은 상태 정보를 기반으로 렌더링합니다.

### 서버 (`Server/`)

* **멀티쓰레딩:** Listening 쓰레드가 클라이언트 연결을 수락하고, 각 클라이언트마다 별도의 Communication 쓰레드를 생성하여 통신을 담당합니다 ([`Server/src/main.cpp`](https://github.com/kijin-Kim/NGP-Project/blob/master/Server/src/main.cpp)).
* **중앙 처리 및 상태 관리:** 서버의 메인 쓰레드가 전역 큐(`g_DataQueue`)를 통해 모든 클라이언트의 요청을 순차적으로 처리합니다. State 패턴을 사용하여 Login, Lobby, Game 상태별 로직을 관리합니다 ([`Server/src/State.h`](https://github.com/kijin-Kim/NGP-Project/blob/master/Server/src/State.h), [`Server/src/main.cpp`](https://github.com/kijin-Kim/NGP-Project/blob/master/Server/src/main.cpp)).
* **동기화:** Critical Section과 Event 객체를 사용하여 쓰레드 간 공유 자원 접근 및 작업 흐름을 제어합니다.

### 클라이언트 (`Client/`)

* **상태 관리:** 서버와 동일하게 Login, Lobby, Game 상태를 가지며, 각 상태에 맞는 UI 렌더링 및 서버 통신 로직을 수행합니다 ([`Client/src/Script/PikachuVolleyBall.cpp`](https://github.com/kijin-Kim/NGP-Project/blob/master/Client/src/Script/PikachuVolleyBall.cpp)).
* **렌더링 파이프라인:** OpenGL API를 직접 사용하여 2D 렌더링 시스템을 구축했습니다.
    * `Renderer` 클래스: 셰이더 관리, 버텍스 버퍼 설정, 드로우 콜 등 핵심 렌더링 로직 담당 ([`Client/src/Client/Renderer.cpp`](https://github.com/kijin-Kim/NGP-Project/blob/master/Client/src/Client/Renderer.cpp)).
    * `TextureManager`: Texture Atlas(Json) 및 이미지 파일 로딩/관리 ([`Client/src/Client/TextureManager.h`](https://github.com/kijin-Kim/NGP-Project/blob/master/Client/src/Client/TextureManager.h)).
    * `FontData`: FreeType 라이브러리를 이용한 폰트 렌더링 지원 ([`Client/src/Client/FontData.h`](https://github.com/kijin-Kim/NGP-Project/blob/master/Client/src/Client/FontData.h)).

### 네트워크 (`Network/`)

* Winsock API를 캡슐화한 `Network` 클래스 제공 ([`Network/src/Network/Network.h`](https://github.com/kijin-Kim/NGP-Project/blob/master/Network/src/Network/Network.h)).
* `#pragma pack(1)`을 사용하여 클라이언트-서버 간 통신에 사용될 패킷 구조체 정의 ([`Network/src/Network/Data.h`](https://github.com/kijin-Kim/NGP-Project/blob/master/Network/src/Network/Data.h)).

## 시작하기 (Getting Started)

### 준비물

* Windows 운영체제
* Visual Studio

### 빌드 방법

1.  이 저장소를 클론합니다: `git clone https://github.com/kijin-Kim/NGP-Project.git`
2.  Visual Studio에서 솔루션 파일을 엽니다.
3.  솔루션 탐색기에서 `Server` 프로젝트와 `Client` 프로젝트를 각각 빌드합니다 (Debug 또는 Release 모드 선택).

### 실행 방법

1.  빌드된 `Server.exe` 파일을 먼저 실행합니다. 서버가 클라이언트 접속을 대기합니다.
2.  빌드된 `Client.exe` 파일을 실행합니다. 필요한 경우 서버 IP 주소를 인자로 전달할 수 있습니다. (로컬 테스트 시: `Client.exe 127.0.0.1`)
3.  최대 4개의 클라이언트까지 접속하여 로그인 후 로비에서 대기합니다. 4명이 모두 접속하면 자동으로 게임이 시작됩니다.

## 알려진 이슈 / 개선점 (Optional)

* 현재 구현은 네트워크 지연 시간 보상(Client Side Prediction, Interpolation 등)을 고려하지 않아, 네트워크 환경에 따라 입력 반응성이 떨어질 수 있습니다.
* 오류 처리 및 예외 처리가 부족할 수 있습니다.
