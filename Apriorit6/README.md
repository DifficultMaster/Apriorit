# Apriorit6

This project is a C++23 (CMake 3.20+) implementation of four RAII/smart-pointer tasks.

## Build

```bash
cmake -S . -B out/build
cmake --build out/build
```

## Run demo

```bash
./out/build/Apriorit6
```

## Where to look

### Task 1
1. Написати власний guard, керуючий дескриптором, отриманим від Win32 API CreateFile (або open, якщо у вас Linux).

- Guard declaration: `task1/include/guards/file_handle_guard.hpp`
- Guard implementation: `task1/src/guards/file_handle_guard.cpp`
- Demo usage: `src/main.cpp` (`DemoTask1`)

### Task 2
2. Написати custom deleter для std::unique_ptr трьома різними способами, закриваючий socket.

- Deleters (lambda, functor, std::function): `task2/include/deleters/socket_deleters.hpp`
- Demo usage: `src/main.cpp` (`DemoTask2`)

### Task 3
3. Пофіксити (якщо треба) наступні сніпети коду, описати що може піти не так.

- File reading snippet (before/after): `task3/src/snippet1.cpp`
- Texture rendering snippet (before/after): `task3/src/snippet2.cpp`
- Database access snippet (before/after): `task3/src/snippet3.cpp`

### Task 4
4. Розробити спрощену реалізацію std::shared_ptr, без std::weak_ptr.

- SharedPtr implementation: `task4/include/shared_ptr.hpp`
- Demo usage: `src/main.cpp` (`DemoTask4`)

## Implementation status check

- Task 1: implemented and compiled.
- Task 2: implemented in three styles and compiled.
- Task 3: fixed snippets are present with explanations in code.
- Task 4: required interface methods are present and compiled.
