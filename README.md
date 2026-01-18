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

This repository is intentionally built **in stages**, each corresponding to a clean commit boundary.

### Phase 1 — Tooling & Build Foundation

* Command-line build only (no IDE dependency)
* Cross-compiled ARM firmware
* Native **host builds for unit testing**
* Clear separation of host vs target code

### Phase 2 — Unified CMake Build System

Single CMake project capable of:

* Building **host-based unit tests**
* Building **ARM firmware** (Debug / Release)
* Launching **GDB debug sessions**
* Flashing firmware to the micro:bit

### Phase 3 — FreeRTOS Integration

* FreeRTOS kernel integrated from source
* Cortex-M4F port
* SysTick configuration
* Heap implementation selection
* RTOS-aware interrupt priority configuration

### Phase 4 — Application Layer

* Task-based application design
* Deterministic scheduling
* ISR → task communication (queues / notifications)
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
* GCC / Clang (for host unit tests)

### ARM Toolchain

* GNU Arm Embedded Toolchain (`arm-none-eabi-gcc`)
* pyOCD
* GDB (arm-none-eabi-gdb)

---

## Build Instructions (Command Line)

### 1. Configure

```bash
cmake -S . -B build \
  -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=arm-gcc-toolchain.cmake
```

### 2. Build Firmware

```bash
cmake --build build
```

### 3. Flash to micro:bit

```bash
pyocd flash build/firmware.hex
```

### 4. Debug

```bash
pyocd gdbserver --target nrf52833
```

In another terminal:

```bash
arm-none-eabi-gdb build/firmware.elf
```
