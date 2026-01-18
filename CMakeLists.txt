cmake_minimum_required(VERSION 4.1)
project(microbit_freertos LANGUAGES C CXX ASM)

# ----------------------------
# Options / Target names
# ----------------------------
set(TARGET_NAME firmware CACHE STRING "Firmware target name (base name, without extension)")
set(ELF_TARGET "${TARGET_NAME}.elf")

option(BUILD_FIRMWARE "Build embedded firmware (ARM cross-compile)" ${CMAKE_CROSSCOMPILING})
option(BUILD_TESTS    "Build host unit tests" ON)

# Optional: compile unit tests with ARM compiler (compile-only; no link/run)
option(BUILD_TESTS_ARM_COMPILE_ONLY "Compile unit tests with ARM compiler (no link/run)" OFF)

# ----------------------------
# Common flags (ARM / nRF52833)
# ----------------------------
set(MB2_CPU_FLAGS
        -mcpu=cortex-m4
        -mthumb
        -mfpu=fpv4-sp-d16
        -mfloat-abi=hard
)

function(mb2_apply_firmware_flags tgt)
    target_compile_options(${tgt} PRIVATE
            ${MB2_CPU_FLAGS}
            $<$<CONFIG:Debug>:-Og -g3>
            $<$<CONFIG:Release>:-O2>

            -ffreestanding
            -fdata-sections -ffunction-sections
            -Wall -Wextra
    )

    target_compile_options(${tgt} PRIVATE
            $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions -fno-rtti>
    )

    target_compile_definitions(${tgt} PRIVATE
            $<$<CONFIG:Release>:NDEBUG>
    )

    target_link_options(${tgt} PRIVATE
            ${MB2_CPU_FLAGS}
            -nostdlib
            -Wl,--gc-sections
            -Wl,-Map=${CMAKE_BINARY_DIR}/${TARGET_NAME}.map
            -T ${CMAKE_SOURCE_DIR}/linker.ld

            # Release ELF is stripped (keeps Debug ELF debuggable)
            $<$<CONFIG:Release>:-Wl,--strip-debug>
    )
endfunction()

# =====================================================================
# Firmware (ARM)
# =====================================================================
if(BUILD_FIRMWARE)

    add_executable(${ELF_TARGET}
            startup.s
            linker.ld

            # ---- Platform / bare-metal sources (today) ----
            main.cpp

            # ---- Future RTOS structure (add later) ----
            # platform/...
            # rtos/...
            # app/...
    )

    target_include_directories(${ELF_TARGET} PRIVATE
            include
            # platform/include
            # rtos/include
            # app/include
    )

    mb2_apply_firmware_flags(${ELF_TARGET})

    # ----------------------------
    # Artifact generation (HEX)
    # ----------------------------
    # We generate HEX after building the ELF. This avoids fragile OUTPUT rules
    # with generator-expressions (especially in multi-config generators).
    add_custom_command(TARGET ${ELF_TARGET} POST_BUILD
            COMMAND ${CMAKE_OBJCOPY} -O ihex
            $<TARGET_FILE:${ELF_TARGET}>
            ${CMAKE_BINARY_DIR}/${TARGET_NAME}.hex
            COMMENT "Generating ${TARGET_NAME}.hex"
            VERBATIM
    )

    add_custom_target(hex
            DEPENDS ${ELF_TARGET}
            COMMENT "Build firmware and generate .hex"
    )

    # ----------------------------
    # Deploy (pyOCD)
    # ----------------------------
    add_custom_target(deploy
            COMMAND pyocd flash --target nrf52833 --erase sector $<TARGET_FILE:${ELF_TARGET}>
            COMMAND pyocd reset --target nrf52833
            DEPENDS ${ELF_TARGET}
            USES_TERMINAL
            VERBATIM
    )

    # ----------------------------
    # Debug server (pyOCD gdbserver)
    # ----------------------------
    add_custom_target(debugserver
            COMMAND pyocd gdbserver
            --target nrf52833
            --port 3333
            --telnet-port 4444
            --persist
            -O frequency=200000
            -O connect_mode=under-reset
            -O cmsis_dap.limit_packets=true
            USES_TERMINAL
            VERBATIM
    )

endif()

# =====================================================================
# Unit tests (Host)
# =====================================================================
if(BUILD_TESTS AND NOT CMAKE_CROSSCOMPILING)

    include(CTest)
    enable_testing()

    add_library(unity
            third_party/unity/src/unity.c
    )
    target_include_directories(unity PUBLIC
            third_party/unity/src
    )

    add_executable(test_queue
            tests/test_queue.cpp
            src/queue.cpp
            src/message.cpp

            include/irq.h
            src/irq_host.c   # NOTE: fixed filename (you had irq_host..c)
    )

    target_include_directories(test_queue PRIVATE
            include
            third_party/unity/src
    )

    target_link_libraries(test_queue PRIVATE unity)
    target_compile_options(test_queue PRIVATE -g -O0)
    target_compile_definitions(test_queue PRIVATE UNIT_TESTING=1)

    add_test(NAME test_queue COMMAND test_queue)

endif()

# =====================================================================
# Unit tests (ARM toolchain) - compile-only (optional)
# =====================================================================
if(BUILD_TESTS_ARM_COMPILE_ONLY AND CMAKE_CROSSCOMPILING)

    add_library(unity_arm_obj OBJECT
            third_party/unity/src/unity.c
    )
    target_include_directories(unity_arm_obj PUBLIC
            third_party/unity/src
    )
    target_compile_options(unity_arm_obj PRIVATE
            ${MB2_CPU_FLAGS}
            -ffreestanding
            -Wall -Wextra
    )

    add_library(test_queue_arm_obj OBJECT
            tests/test_queue.cpp
    )
    target_include_directories(test_queue_arm_obj PRIVATE
            include
            third_party/unity/src
    )
    target_compile_options(test_queue_arm_obj PRIVATE
            ${MB2_CPU_FLAGS}
            -ffreestanding
            -Wall -Wextra
    )

    add_custom_target(arm_tests_compile
            DEPENDS unity_arm_obj test_queue_arm_obj
            COMMENT "Compile unit tests with ARM toolchain (no link/run)"
    )

endif()
