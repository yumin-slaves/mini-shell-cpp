# Mini Shell Project

이 프로젝트는 C 언어를 기반으로 커맨드 라인 쉘을 직접 구현된 것을 바탕으로
C++로 리팩토링하는 프로젝트입니다.

---

## 🛠 Build 및 실행

```bash
make clean
make
./build/shell
```

---

## 팀원

## 팀원

- [박주영](https://github.com/juyounginpark) - C++ 리팩토링 담당


## 📦 기능
> 아래 기능은 원래 C로 구현되었으며, 현재는 C++로 리팩토링 중입니다.
- 기본 명령어
  - 내부 명령어: `cd`, `pwd`, `exit`
  - 외부 명령어: `ls`, `date`, `cat`
- 명령어 파싱
- 단일 명령 실행
- `|` 파이프 처리
- `<`, `>` 입출력 리디렉션
- `&` 백그라운드 실행`
- 설정파일(`.myshellrc`) 처리
- SIGINT 핸들링
- 히스토리 저장

---

## ✅ C++ 리팩토링 일정 & PR 계획

### 🧩 PR #1: 내부 명령어 리팩토링 (6/18 ~ 6/19)

브랜치 이름: `refactor/internal-command`

- [x] `Shell`, `Parser`, `Executor` 클래스 기본 구조 생성
- [x] `ICommand` 인터페이스 설계
- [x] `CdCommand`, `PwdCommand`, `ExitCommand` 구현
- [x] 명령어 파싱 → 커맨드 객체 분리 구조
- [x] 메인 루프에서 커맨드 실행 흐름 연결
- [ ] 테스트: 내부 명령어 동작 확인

📌 **PR 제출일 목표: 6/19(수) 오후 6시 이전**

---

### 🧩 PR #2: 외부 명령어 실행 구조 (6/20 ~ 6/21)

브랜치 이름: `refactor/external-command`

- [x] `ExternalCommand` 클래스 설계 및 구현
- [x] `fork` + `execvp` 기반 실행 처리
- [x] 내부/외부 명령어 분기 로직 작성
- [x] `Shell`에서 execute 흐름 통합
- [ ] 테스트: `ls`, `date`, `cat` 정상 실행

📌 **PR 제출일 목표: 6/21(금) 오후 6시 이전**

---

### 🧩 PR #3: 리디렉션 & 파이프 구조 설계 (6/22 ~ 6/23)

브랜치 이름: `refactor/redirection-pipe`

- [x] 명령어 파싱 확장: `<`, `>`, `|` 토큰 인식
- [x] 리디렉션/파이프 정보를 Command 객체에 포함
- [x] 실행 구조에 리디렉션/파이프 슬롯 추가 (기능 구현은 선택)
- [ ] 테스트: 구조만 반영된 명령어 실행 흐름 확인

📌 **PR 제출일 목표: 6/23(일) 오후 6시 이전**

---

### 🧩 PR #4: 설정파일 & 부가기능 (6/24 ~ 6/25)

브랜치 이름: `feature/rc-history-signal`

- [x] `.myshellrc` 파일 파싱 및 적용
- [x] `.myshell_history` 기록 기능 추가
- [x] 환경변수 처리 (`getenv`, `export`, `unset`)
- [x] `SIGINT` 핸들링 (`Ctrl+C` 처리)
- [ ] 테스트: `.rc`, `.history` 정상 작동 여부 확인

📌 **PR 제출일 목표: 6/25(수) 오후 6시 이전**

---

## 📁 프로젝트 구조

```bash
myshell/
├── include/        # 헤더 파일 (.h)
│   ├── executor.h
│   ├── parser.h
│   └── redirect.h
├── src/            # 소스 코드 (.c)
│   ├── main.c
│   ├── parser.c
│   ├── redirect.c
│   └── executor.c
├── build/
├── Makefile        # 빌드 파일
└── README.md
```

---

## 🤝 GitHub 협업 방식

- 기능 단위 브랜치 (`feat/parser`, `feat/executor` 등) 생성
- PR → 리뷰 → 병합
- 커밋 메시지는 `feat:`, `fix:`, `docs:` 등 [Conventional Commits] 스타일 사용

---

## 🎨 코드 스타일 가이드

> 일관된 스타일을 유지하여 코드 가독성과 협업 효율을 높입니다.

- 클래스 이름: `PascalCase` (`Shell`, `InternalCommand`)
- 함수 이름: `snake_case` (`run_shell()`, `parse_command()`)
- 변수 이름: `snake_case` (`command_list`, `input_line`)
- 파일 이름: `snake_case` (`parser.cpp`, `redirect_handler.h`)

---