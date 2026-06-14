# AGENTS.md

## Build & Run

Compile with g++ (requires C++11+):

```bash
g++ math_coach_sim.cpp ui_renderer.cpp animation.cpp -o math_coach_sim -finput-charset=UTF-8
```

There is no separate build system (no Makefile, CMake, or CI).

Run from the repo root — the program loads `questions.json` via a relative path (`loadQuestions("questions.json")`).

## Platform Notes

- Primary target is **Windows** (uses `system("CLS")`, `_sleep()`, `SetConsoleCP`). The `ui_renderer.cpp` has `#ifdef _WIN32` guards; Linux gets `system("clear")` and `std::this_thread::sleep_for`.
- UTF-8 + wide chars throughout (`wchar_t`, `wprintf`, `wstring`). The `-finput-charset=UTF-8` flag is needed to compile Chinese string literals correctly.
- ANSI color codes are used directly via escape sequences. On Windows, the code enables `ENABLE_VIRTUAL_TERMINAL_PROCESSING` at runtime.

## Architecture

- **`math_coach_sim.cpp`** — main entry point and game logic (~430 lines). Contains game loop, input handling, question loading, and rating updates.
- **`ui_renderer.cpp` / `ui_renderer.h`** — `ui` namespace with all display functions (text boxes, student cards, question display, victory screen).
- **`animation.cpp` / `animation.h`** — `anim` namespace with character animation effects for success/failure feedback. Uses typewriter effect, progress bars, and screen effects.
- **`questions.json`** — question bank. Hand-written JSON with Chinese text. Parsed by a custom mini-JSON parser (no library dependency). Supports `answer_type`: `"int"`, `"int_pair"`, `"string_multi"`.

The codebase has **zero external dependencies** — everything is standard library + custom code.

## Conventions

- All user-facing text is in **Chinese** (Simplified).
- Student data stored in global arrays: `dai[1000]`, `ji[1000]`, `shu[1000]`, `zu[1000]` (代数/几何/数论/组合). Indices 1–4 are the four students.
- Rating range is 1–4 (差/中/良/优), clamped by `clamp_rating()`.
- Difficulty is 1–3, mapped to 0-based index for the `questions[4][3]` array.
- The `ui` namespace is the rendering layer; keep UI changes in `ui_renderer.cpp`, game logic in `math_coach_sim.cpp`.

## Adding Questions

Edit `questions.json`. Each object needs: `category` (代数/几何/数论/组合), `difficulty` (1–3), `intro`, `question`, `answer_type`, `answer`, and `reward` (1 or 2). The parser is line-oriented and doesn't support nested JSON or escaped characters beyond `\n`, `\"`, `\\`.
