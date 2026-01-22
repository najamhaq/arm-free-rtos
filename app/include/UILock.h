//
// Created by najamhaq on 2026-01-22.
//

#ifndef MICROBIT_FREERTOS_APP_INCLUDE_UILOCK_H
#define MICROBIT_FREERTOS_APP_INCLUDE_UILOCK_H

#pragma once
#include "FreeRTOS.h"
#include "semphr.h"

extern SemaphoreHandle_t g_ledMutex;

#endif // MICROBIT_FREERTOS_APP_INCLUDE_UILOCK_H
