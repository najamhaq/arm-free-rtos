# FreeRTOS on ARM Cortex-M4F (micro:bit v2 / nRF52833)

## Overview

This repository documents an **end-to-end FreeRTOS bring-up and application project** on the **BBC micro:bit v2**, targeting the **Nordic nRF52833 (ARM Cortex-M4F)** microcontroller.

The goal is to demonstrate **professional-grade embedded firmware skills**:

* Bare-metal board bring-up
* Deterministic interrupt control
* RTOS integration and configuration
* ISR-safe communication primitives
* Clean separation between platform, RTOS, and application layers

The micro:bit is used strictly as a **carrier board**. All work targets the MCU directly.

---

## Hardware

* **Board:** BBC micro:bit v2
* **SoC:** Nordic nRF52833
* **CPU:** ARM Cortex-M4F (ARMv7E-M)
* **Clock:** HFCLK @ 64 MHz
* **Debug / Flash:** DAPLink, pyOCD + GDB

---

## Project Roadmap (Incremental, Commit-Driven)

### Phase 1 — Tooling & Build Foundation

* Command-line build only (no IDE dependency)
* Cross-compiled ARM firmware
* Native **host builds for unit testing**
* Clear separation of host vs target code

### Phase 2 — Unified CMake Build System

* Single CMake project supporting:

    * Host-based unit tests
    * ARM firmware builds
    * Debug targets
    * Flash / deploy targets

### Phase 3 — FreeRTOS Integration

* FreeRTOS kernel integrated from source
* Cortex-M4F port
* SysTick configuration
* Heap implementation selection
* RTOS-aware interrupt priority configuration

### Phase 4 — Application Layer

* Task-based application design
* Deterministic scheduling
* ISR → task communication
* Real-time timing guarantees

---

## Design Philosophy

### Bare-Metal First, RTOS Second

The project starts **without an RTOS** and adds FreeRTOS only after:

* Startup code is understood
* Interrupts are controlled explicitly
* Memory layout is verified

This mirrors how **production firmware teams** introduce RTOSes in safety-critical systems.

---

## Build Requirements

### Host Tools

* CMake ≥ 3.22
* Ninja (recommended)
* GCC or Clang (for host unit tests)
* Git (with submodule support)

### ARM Toolchain

* GNU Arm Embedded Toolchain (`arm-none-eabi-gcc`)
* pyOCD
* GDB (`arm-none-eabi-gdb`)

### Unit Testing Framework

* **Unity** (ThrowTheSwitch)

    * Added as a **git submodule** under `third_party/unity`
    * Used for fast, deterministic **host-based unit testing** of embedded logic

> After cloning the repository, initialize dependencies with:
>
> ```bash
> git submodule update --init --recursive
> ```

---

## Build Instructions (Command Line)

### Configure

```bash
cmake -S . -B build -G Ninja -DCMAKE_TOOLCHAIN_FILE=arm-gcc-toolchain.cmake
```

### Build Firmware

```bash
cmake --build build
```

### Flash to micro:bit

```bash
cmake --build build --target deploy
```

### Debug

Terminal A:

```bash
cmake --build build --target debugserver
```

Terminal B:

```bash
arm-none-eabi-gdb build/firmware.elf
```

---

## Project Structure

```
.
├── CMakeLists.txt
├── arm-gcc-toolchain.cmake
├── platform/
├── rtos/
├── app/
├── tests/
├── third_party/unity/
├── startup.s
├── linker.ld
└── README.md
```

---

## What This Repository Demonstrates

* ARM Cortex-M reset and startup flow
* NVIC and interrupt priority management
* FreeRTOS kernel bring-up
* ISR-safe queue and notification usage
* Deterministic task scheduling
* Host-based unit testing for embedded logic
* Professional CMake-based firmware builds

---

## License

Educational and demonstrational use only.
