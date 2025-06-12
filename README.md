# Mini Shell Project

이 프로젝트는 C 언어를 기반으로 커맨드 라인 쉘을 직접 구현된 것을 바탕으로
C++로 리팩토링하는 프로젝트입니다.

> for 박주영

---

## 🛠 Build 및 실행

```bash
make clean
make
./build/shell
```

---

## 📦 기능
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

## 해야 할 일

### 🟦 1주차 목표: 기본 구조 + 내부 명령어까지 리팩토링
- Shell 클래스로 메인 루프 재구성
- Parser를 std::string, std::vector 기반으로 재작성
- Command 인터페이스 설계
- 내부 명령어(cd, pwd, exit)를 InternalCommand로 분리하여 실행되도록 구현

#### 🧩 구성 예시

- Shell, Parser, ICommand, InternalCommand 클래스 생성
- 기존 C 코드(main.c, parser.c 등) 일부를 점진적으로 C++로 변환
- make 빌드 및 테스트 입력 (< test.txt)으로 동작 확인

---

### 🟩 2주차 목표: 나머지 기능 전체 리팩토링
- ExternalCommand 구현 (fork + execvp)
- RedirectHandler, PipeHandler 설계 및 적용
- `&` 백그라운드 실행 처리
- 전체 실행 흐름을 클래스 기반으로 정리 (Executor 등)
- .myshellrc 읽기 / 히스토리 저장은 선택 구현

#### 🧩 구성 예시

- 파이프(|) 처리 → PipeHandler 클래스
- 리디렉션(<, >) 처리 → RedirectHandler 클래스
- Command의 서브클래스로 외부 명령 실행
- 전체적으로 Shell.run() 한 줄로 실행 흐름이 연결되도록 설계

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