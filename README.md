# Mini-Shell-Project

이 프로젝트는 C 언어를 기반으로 커맨드 라인 쉘을 직접 구현하고, 시스템 프로그래밍의 핵심 개념을 학습하는 스터디형 협업 프로젝트입니다.

---

## 🛠 Build 및 실행

```bash
make clean
make
./build/shell
```

---

## 📦 기능
✅ 완성
- 기본 명령어
  - 내부 명령어: `cd`, `pwd`, `exit`
  - 외부 명령어: `ls`, `date`, `cat`
- 명령어 파싱
- 단일 명령 실행
- `|` 파이프 처리
- `<`, `>` 입출력 리디렉션
- `&` 백그라운드 실행`

⏳ 미완성
- 설정파일(`.myshellrc`) 처리
- SIGINT 핸들링
- 히스토리 저장

---

## 📁 프로젝트 구조

```bash
myshell/
├── include/        # 헤더 파일 (.h)
│   ├── parser.h
│   └── executor.h
├── src/            # 소스 코드 (.c)
│   ├── main.c
│   ├── parser.c
│   └── executor.c
├── build/          # 빌드된 결과물 (.o, shell 실행 파일)
├── Makefile        # 빌드 자동화
└── README.md
```

---

## 🤝 GitHub 협업 방식

- 기능 단위 브랜치 (`feat/parser`, `feat/executor` 등) 생성
- PR → 리뷰 → 병합
- 커밋 메시지는 `feat:`, `fix:`, `docs:` 등 [Conventional Commits] 스타일 사용
- `build/`, `*.o` 파일 등은 `.gitignore`로 관리

---