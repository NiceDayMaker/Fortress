# Fortress Console Game (C언어 포트리스)

포트리스(Fortress) 스타일의 콘솔 기반 2D 포격 게임입니다.  
Windows 콘솔에서 ASCII 그래픽과 miniaudio를 활용해 구현되었으며  2인용 턴제 방식으로 플레이할 수 있습니다.

---

## 게임 스크린샷
![게임 타이틀 화면](/Resources/title.png)

![게임 플레이 화면](/Resources/playing.png)

## 프로젝트 구조
```bash
.
├── main.c              # 게임 엔트리 포인트
├── game.c/h            # GameManager: 전체 상태 및 턴 관리
├── player.c/h          # Player 구조체 및 조작, 발사 로직
├── projectile.c/h      # 포탄 궤적 및 충돌 처리
├── terrain.c/h         # 지형 생성 및 파괴 처리
├── render.c/h          # 콘솔 렌더링 시스템
├── input.c/h           # 입력 처리 시스템
├── utils.c/h           # 유틸리티 함수 (커서 이동, ansi 색상 출력, FPS 제어 등)
├── sound.c/h           # 사운드 재생 (miniaudio 사용)
├── miniaudio.h         # MiniAudio 라이브러리
└── Resources/
    ├── asciix4.txt     # 시작 화면 ASCII 아트
    └── *.wav           # 사운드 효과 (move, set, shoot 등)
```

## 빌드
MinGW gcc 빌드
```bash
gcc -fdiagnostics-color=always -std=c17 -O2 -g main.c game.c utils.c render.c terrain.c player.c projectile.c input.c sound.c -o Forstrss.exe -ldsound -lwinmm
```

## 실행
ANSI/VT100 Escape Code를 지원하는 콘솔 필요 (Windows 10 이상 권장)
- ex)
    - Windows Console
    - Windows PowerShell

```bash
./Fortress.exe
```

