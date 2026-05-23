# Pikachu Volleyball - Multiplayer Game

> C++ / OpenGL / Winsock 으로 엔진 없이 구현한 2v2 멀티플레이 피카츄 배구 게임
---

## 플레이 영상

https://github.com/user-attachments/assets/7f509ddb-ff0e-4dba-a384-b0a7caeb8bc2

## 채팅

https://github.com/user-attachments/assets/8bc3fdb8-d012-4162-8c72-037a08152230



## Summary

게임 엔진을 사용하지 않고 **OpenGL과 Winsock으로 클라이언트와 서버를 모두 구현한 멀티플레이 게임**입니다.  
2D 렌더링 파이프라인, TCP 네트워크 통신, 멀티스레드 동기화, 게임 상태 관리까지 전 영역을 설계했습니다.

---

## 핵심 기능

### 1. OpenGL 기반 2D 렌더링 시스템 구현

엔진의 추상화 없이 **OpenGL 4.0 코어 프로파일을 사용해 2D 렌더러를 처음부터 구현**했습니다.

**구현 내용**
- VAO/VBO/EBO 버퍼 관리 및 Shader 파이프라인 구축
- Quad 단위 렌더링 추상화 (`Renderer::DrawQuad`)
- **텍스처 아틀라스 시스템** - JSON 메타데이터를 파싱해 단일 텍스처에서 여러 스프라이트 추출
- **FreeType 기반 폰트 렌더링** - 한글 포함 비트맵 폰트 캐싱
- 직교 투영(Orthographic Projection)으로 픽셀 단위 2D 좌표계 구성

**관련 코드**
- [`Client/src/Client/Renderer.cpp`](Client/src/Client/Renderer.cpp)
- [`Client/src/Client/TextureManager.h`](Client/src/Client/TextureManager.h)

---

### 2. 클라이언트-서버 네트워크 아키텍처 (Winsock2 + TCP)

**서버를 구현하고**, 클라이언트와 통신할 패킷 구조를 설계했습니다.

**구현 내용**
- TCP 소켓 통신 (`Network` 싱글톤으로 송수신 추상화)
- **상태별 패킷 구조 설계** - Login / Lobby / Game 각각의 패킷을 `IData` 인터페이스로 추상화
- `#pragma pack(1)` 으로 패킷 메모리 정렬 명시 → 네트워크 전송 시 패딩 이슈 제거
- 클라이언트 → 서버: 입력값(UserInput) 전송
- 서버 → 클라이언트: 계산된 오브젝트 위치 + 애니메이션 상태 전송

**설계 포인트**
- 서버를 권위 서버(Authoritative Server)로 두어 모든 게임 로직 계산을 서버에서 수행
- 클라이언트는 입력 전송 + 결과 렌더링만 담당 → 치팅 방지 + 동기화 단순화

**관련 코드 및 문서**
- [NGP 프로젝트 추진 계획서.pdf](https://github.com/user-attachments/files/28174538/NGP.pdf)
- [`Network/src/Network/Data.h`](https://github.com/kijin-Kim/NGP-Project/blob/6f86fbdfbdbe67556b95b006a836c403f3e86bd0/Network/src/Network/Data.h#L61) - 패킷 구조 정의
- [`Server/src/main.cpp`](https://github.com/kijin-Kim/NGP-Project/blob/6f86fbdfbdbe67556b95b006a836c403f3e86bd0/Server/src/main.cpp#L51) - 서버 메인 루프

---

### 3. 멀티스레드 서버 구조 (Critical Section + Event)

최대 4명의 동시 접속을 처리하기 위해 **스레드 기반 서버**를 구현했습니다.

**스레드 구조**
- **Listening Thread**: 신규 클라이언트 접속 대기 (4명 차면 종료)
- **Communication Thread** (클라이언트당 1개): 데이터 송수신 전담
- **Main Thread**: 16ms 틱(60FPS) 단위로 게임 로직 처리

**동기화 처리**
- `CRITICAL_SECTION`으로 공유 큐(`g_DataQueue`)와 상태 배열(`g_ClientStates`) 보호
- `Event` 객체로 데이터 처리 완료 시점을 통신 스레드에 통지 (`SetEvent` / `WaitForSingleObject`)
- 클라이언트 상태(Login/Lobby/Game)에 따라 패킷 크기를 동적으로 결정

**관련 코드**
- [`Server/src/main.cpp - ListeningThreadProc`](https://github.com/kijin-Kim/NGP-Project/blob/6f86fbdfbdbe67556b95b006a836c403f3e86bd0/Server/src/main.cpp#L147),
- [`Server/src/main.cpp - CommunicationThreadProc`](https://github.com/kijin-Kim/NGP-Project/blob/6f86fbdfbdbe67556b95b006a836c403f3e86bd0/Server/src/main.cpp#L179)

---

### 4. State 패턴 기반 게임 흐름 관리

게임의 3가지 단계를 **State 패턴**으로 분리해 클라이언트와 서버 양쪽에 일관되게 적용했습니다.

**상태 구조**
```
Login (닉네임 입력)  →  Lobby (채팅 / 매칭 대기)  →  Game (인게임 플레이)
```

**구현 내용**
- 추상 클래스 `State`에 `SendData()`, `ReceiveData()`, `Render()` 정의
- 각 상태(`LoginState`, `LobbyState`, `GameState`)가 자신의 패킷 처리와 렌더링을 캡슐화
- 상태 전환 시 `Game::SetGameState()` 한 줄로 전체 흐름 교체

**얻은 이점**
- 신규 상태 추가 시 기존 코드 수정 최소화

**관련 코드**
- [`Client/src/Client/State.h`](https://github.com/kijin-Kim/NGP-Project/blob/6f86fbdfbdbe67556b95b006a836c403f3e86bd0/Server/src/State.h#L10)

---
