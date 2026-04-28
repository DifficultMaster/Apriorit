# Apriorit3 — Singly Linked List Exercise (C++23, CMake, MSVC v145 (VS 2026))

This repository contains a small C++23 data-structure exercise focused on implementing a custom forward singly linked list.

It currently builds one console application:
- **Apriorit7** — runs a simple manual demo from `src/main.cpp`.

The list implementation is located in `include/apriorit7/core/linked_list.hpp`.

## Table of Contents
- [Overview](#overview)
- [System Requirements](#system-requirements)
- [Build](#build)
- [Run](#run)
- [How It Works](#how-it-works)
- [Public API Snapshot](#public-api-snapshot)
- [Error Handling](#error-handling)
- [Troubleshooting](#troubleshooting)
- [Future Improvements](#future-improvements)

## Overview

The project demonstrates:
- Template container design in modern C++ (`template<typename T>`).
- Ownership and memory safety via `std::unique_ptr` for node chaining.
- Move-only list semantics (copy disabled, move enabled).
- Forward-iterator support for mutable and const iteration.
- Core list operations (`push_front`, `pop_front`, `insert_after`, `erase_after`, `remove_if`, `reverse`, `resize`, `split_when`, `splice_after`).

## System Requirements

- **CMake** 3.20+
- **C++23** compiler
- Ninja generator (recommended)

## Build

From the repository root:

```bash
cmake -S . -B out/build
cmake --build out/build
```

Generated executable:
- `Apriorit7`

## Run

### 1) Build target

```bash
cmake -S . -B out/build -G Ninja
cmake --build out/build
```

### 2) Run demo

```bash
./out/build/Apriorit7
```

The program prints visible `[PASS]` / `[FAIL]` checks for several list operations.

## How It Works

### Container model
1. `LinkedList<T>` stores nodes as `std::unique_ptr<Node>`.
2. The head pointer owns the full chain.
3. Node ownership is transferred via move operations.

### Iterator model
1. Concrete iterator structs are internal.
2. Public aliases `iterator` and `const_iterator` are exposed.
3. `begin/end/cbegin/cend` provide forward traversal.

### Current demo flow (`src/main.cpp`)
1. Create an empty list.
2. Push and pop front element.
3. Move-construct and move-assign lists.
4. Clear list and print pass/fail checks.
5. Split list at a specific value and print pass/fail checks.

## Public API Snapshot

Key methods in `LinkedList<T>`:
- Element access: `front()`
- Iterators: `begin()`, `end()`, `cbegin()`, `cend()`
- Capacity: `empty()`, `size()`, `max_size()`
- Modifiers: `clear()`, `push_front()`, `pop_front()`, `insert_after()`, `insert_range_after()`, `erase_after()`, `resize()`, `swap()`
- Operations: `splice_after()`, `split_when()`, `remove()`, `remove_if()`, `reverse()`
- Non-members: `operator==`, `operator<=>`

## Error Handling

- Memory management is handled by `std::unique_ptr`, avoiding manual `delete`.
- Invalid iterator positions in insertion/erase helpers are handled defensively (returning end iterator behavior).
- Demo validation is intentionally simple and output-based (`[PASS]` / `[FAIL]`).

## Troubleshooting

- **No console output**: ensure you run `Apriorit7` executable, not only build.
- **Compiler errors about ranges/three-way compare**: verify C++23 is enabled.
- **Include errors for list header**: verify include path points to `include/` and include file is `apriorit7/core/linked_list.hpp`.

## Future Improvements

- Add proper unit tests for all list operations and edge cases.
- Add benchmark comparison versus `std::forward_list` for educational profiling.
- Add more complex operations and demo scenarios to test them.
