cmake_minimum_required(VERSION 3.31)
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
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

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
            -T ${SRC_DIR}/platform/linker.ld

            # Release ELF is stripped (keeps Debug ELF debuggable)
            $<$<CONFIG:Release>:-Wl,--strip-debug>
    )
endfunction()


# ====================
# formating
# ====================
find_program(CLANG_FORMAT clang-format)

add_custom_target(format
        COMMAND ${CLANG_FORMAT} -i
        ${PROJECT_SOURCE_DIR}/app/*.cpp
        ${PROJECT_SOURCE_DIR}/app/*.h
        ${PROJECT_SOURCE_DIR}/platform/*.cpp
        ${PROJECT_SOURCE_DIR}/platform/*.h
        COMMENT "Running clang-format"
)

add_custom_target(format-check
        COMMAND ${CLANG_FORMAT} --dry-run --Werror
        ${PROJECT_SOURCE_DIR}/app/*.cpp
        ${PROJECT_SOURCE_DIR}/app/*.h
        ${PROJECT_SOURCE_DIR}/platform/*.cpp
        ${PROJECT_SOURCE_DIR}/platform/*.h
        COMMENT "Checking clang-format (fails on diff)"
)



# =====================================================================
# Firmware (ARM)
# =====================================================================
if(BUILD_FIRMWARE)

    add_executable(${ELF_TARGET}
            ${SRC_DIR}/platform/startup.s
            ${SRC_DIR}/platform/linker.ld
            ${SRC_DIR}/platform/handlers.c
            ${SRC_DIR}/platform/led_matrix.cpp
            ${SRC_DIR}/platform/mem.c
            ${SRC_DIR}/platform/button.cpp
            ${SRC_DIR}/drivers/include/button.h

            # --- actual application ---
            ${SRC_DIR}/app/main.cpp
            ${SRC_DIR}/app/include/app_queues.h
            ${SRC_DIR}/app/app_queues.cpp
            ${SRC_DIR}/app/button_task.cpp
            ${SRC_DIR}/app/input_controller.cpp
            ${SRC_DIR}/app/include/lock.h
            ${SRC_DIR}/app/include/ui_lock.h
            ${SRC_DIR}/app/ui_lock.cpp

            ${SRC_DIR}/modules/debouncer.h
            ${SRC_DIR}/modules/debouncer.cpp

            # --- Your RTOS integration ---
            ${SRC_DIR}/rtos/hooks.c
            ${SRC_DIR}/rtos/heap_4.c

            # --- FreeRTOS Kernel (from submodule) ---
            third_party/freertos-kernel/list.c
            third_party/freertos-kernel/queue.c
            third_party/freertos-kernel/tasks.c
            third_party/freertos-kernel/timers.c
            third_party/freertos-kernel/event_groups.c
            third_party/freertos-kernel/stream_buffer.c

            # --- Cortex-M4F port ---
            third_party/freertos-kernel/portable/GCC/ARM_CM4F/port.c
            src/include/input_type.h
            src/drivers/button.cpp
            src/platform/include/button_hw.h
    )

    target_include_directories(${ELF_TARGET} PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${SRC_DIR}/include
            ${SRC_DIR}/platform/include
            ${SRC_DIR}/rtos/include
            ${SRC_DIR}/app/include
            ${SRC_DIR}/drivers/include
            ${SRC_DIR}/modules
            third_party/freertos-kernel/include
            third_party/freertos-kernel/portable/GCC/ARM_CM4F
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
    # Use the Windows Python launcher so deploy/debug works from PowerShell, Git Bash, MSYS2, CLion
    find_program(PY_LAUNCHER py REQUIRED)
    add_custom_target(deploy
            COMMAND ${PY_LAUNCHER} -m pyocd flash --target nrf52833 --erase sector $<TARGET_FILE:${ELF_TARGET}>
            COMMAND ${PY_LAUNCHER} -m pyocd reset --target nrf52833
            DEPENDS ${ELF_TARGET}
            USES_TERMINAL
            VERBATIM
    )

    # ----------------------------
    # Debug server (pyOCD gdbserver)
    # ----------------------------
    add_custom_target(debugserver
            COMMAND ${PY_LAUNCHER} -m pyocd gdbserver
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
    add_executable(unit_tests
            tests/test_main.c
    )


    target_link_libraries(unit_tests PRIVATE unity)
    target_compile_options(unit_tests PRIVATE -g -O0)
    target_compile_definitions(unit_tests PRIVATE UNIT_TESTING=1)

    add_test(NAME unit_tests COMMAND unit_tests)

    add_executable(button_test
            tests/button_test.cpp
            ${SRC_DIR}/modules/debouncer.h
            ${SRC_DIR}/modules/debouncer.cpp
    )
    target_include_directories(button_test PRIVATE
            ${SRC_DIR}/include
            ${SRC_DIR}/modules
            ${SRC_DIR}/drivers/include
            third_party/unity/src
    )
    target_include_directories(unit_tests PRIVATE
            ${SRC_DIR}/include
            ${SRC_DIR}/modules
            ${SRC_DIR}/drivers/include
            third_party/unity/src
    )

    target_link_libraries(button_test PRIVATE unity)
    target_compile_options(button_test PRIVATE -g -O0)
    target_compile_definitions(button_test PRIVATE UNIT_TESTING=1)
    add_test(NAME button_test COMMAND button_test)
    set_tests_properties(button_test PROPERTIES LABELS "unit;button")
endif()

# =====================================================================
# Unit tests (ARM toolchain) - compile-only (optional)
# =====================================================================
if(BUILD_TESTS_ARM_COMPILE_ONLY AND CMAKE_CROSSCOMPILING)

    add_library(unity_arm_obj OBJECT
            third_party/unity/${SRC_DIR}/unity.c
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
