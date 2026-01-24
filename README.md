# FreeRTOS on ARM Cortex-M4F  
### BBC micro:bit v2 (Nordic nRF52833)

[![CI](https://github.com/najamhaq/arm-free-rtos/actions/workflows/ci.yaml/badge.svg)](
https://github.com/najamhaq/arm-free-rtos/actions/workflows/ci.yaml
)

## Overview

This repository documents an **end-to-end FreeRTOS bring-up and application project**
on the **BBC micro:bit v2**, targeting the **Nordic nRF52833 (ARM Cortex-M4F)** MCU.

The goal is to demonstrate **professional-grade embedded firmware development**
practices, including:

- Bare-metal board bring-up
- Deterministic interrupt control
- RTOS integration and configuration
- ISR-safe communication primitives
- Clean separation between platform, RTOS, and application layers

The micro:bit is used strictly as a **carrier board**.  
All development targets the MCU directly — no vendor SDK abstractions.

---

## Hardware

- **Board:** BBC micro:bit v2  
- **SoC:** Nordic nRF52833  
- **CPU:** ARM Cortex-M4F (ARMv7E-M)  
- **Clock:** HFCLK @ 64 MHz  
- **Debug / Flash:** DAPLink, pyOCD + GDB  

---

## Project Roadmap (Incremental, Commit-Driven)

### Phase 1 — Tooling & Build Foundation
- Command-line build only (no IDE dependency)
- Cross-compiled ARM firmware
- Native **host builds for unit testing**
- Clear separation between host and target code

### Phase 2 — Unified CMake Build System
- Single CMake project supporting:
  - Host-based unit tests
  - ARM firmware builds
  - Flash / deploy targets
  - Debug server targets

### Phase 3 — FreeRTOS Integration
- FreeRTOS kernel integrated from source
- Cortex-M4F port
- SysTick configuration
- Heap implementation selection
- RTOS-aware interrupt priority configuration

### Phase 4 — Application Layer
- Task-based application design
- Deterministic scheduling
- ISR → task communication
- Real-time timing guarantees

---

## Design Philosophy

### Bare-Metal First, RTOS Second

The project starts **without an RTOS** and introduces FreeRTOS only after:

- Startup code is understood
- Interrupts are explicitly controlled
- Memory layout and linker behavior are verified

This mirrors how **production firmware teams** introduce RTOSes in
safety-critical and resource-constrained systems.

---

## Build Requirements

### Host Tools

- CMake ≥ 3.22  
- Ninja (recommended)  
- GCC or Clang (for host unit tests)  
- Git (with submodule support)  
- clang-format  

### ARM Toolchain

- GNU Arm Embedded Toolchain (`arm-none-eabi-gcc`)
- pyOCD
- GDB (`arm-none-eabi-gdb`)

### Unit Testing Framework

- **Unity** (ThrowTheSwitch)
  - Added as a **git submodule** under `third_party/unity`
  - Used for fast, deterministic **host-based unit testing** of embedded logic

---

## Initial Setup

```bash
git config --global core.autocrlf true   # optional (Windows users)
./dev setup
./dev configure
./dev build
./dev test
```

> Formatting and CI checks rely on `clang-format` being installed and available on PATH.

---

## Development Commands (`./dev`)

| Command | Description |
|------|-------------|
| `./dev setup` | Install / verify development prerequisites |
| `./dev configure` | Configure CMake build |
| `./dev build` | Build firmware |
| `./dev deploy` | Flash firmware to micro:bit |
| `./dev debugserver` | Start pyOCD GDB server |
| `./dev test` | Run host-based unit tests |
| `./dev format` | Apply clang-format |
| `./dev format-check` | CI formatting check |

---

## Build & Debug (Firmware)

```bash
./dev configure
./dev build
./dev debugserver
```

In a separate terminal:

```bash
arm-none-eabi-gdb build/firmware.elf
```

---

## Host-Based Unit Tests

```bash
cmake -S . -B build-host -G Ninja \
  -DBUILD_FIRMWARE=OFF \
  -DBUILD_TESTS=ON

cmake --build build-host
ctest --test-dir build-host --output-on-failure
```

---

## Project Structure

```
.
├── CMakeLists.txt
├── arm-gcc-toolchain.cmake
├── dev
├── scripts/
│   ├── dev-setup.sh
│   ├── dev-test.sh
│   ├── dev-format.sh
│   └── ci-check-format.sh
├── src/
    ├── app/
    │   └── main.cpp
    ├── drivers/        # Hardware drivers (buttons, LED matrix, UART, etc.)
    ├── modules/        # Pure logic modules (hardware-independent) testable on host
    ├── platform/       # Platform-specific code (SoC, registers, pin mapping)
    │   ├── startup.s
    │   ├── linker.ld
    └── rtos/           # RTOS integration and configuration
├── tests/
├── third_party/
│   └── unity/
└── README.md
```

---

## What This Repository Demonstrates

- ARM Cortex-M reset and startup flow
- NVIC and interrupt priority management
- FreeRTOS kernel bring-up
- ISR-safe queues and notifications
- Deterministic task scheduling
- Host-based unit testing for embedded code
- Professional CMake-based firmware workflows

---

## License

Educational and demonstrational use only.
