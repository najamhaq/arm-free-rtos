//
// Created by najamhaq on 2026-01-22.
//

#ifndef MICROBIT_FREERTOS_APP_INCLUDE_LOCK_H
#define MICROBIT_FREERTOS_APP_INCLUDE_LOCK_H

#pragma once
#include "FreeRTOS.h"
#include "semphr.h"

class Lock {
public:
  explicit Lock(SemaphoreHandle_t m) : m_(m) { xSemaphoreTake(m_, portMAX_DELAY); }
  ~Lock() { xSemaphoreGive(m_); }
  Lock(const Lock&) = delete;
  Lock& operator=(const Lock&) = delete;

private:
  SemaphoreHandle_t m_;
};

#endif // MICROBIT_FREERTOS_APP_INCLUDE_LOCK_H
