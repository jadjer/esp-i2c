// Copyright 2025 Pavel Suprunov
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "i2c/Master.hpp"

#include <driver/i2c_master.h>

#include <esp_log.h>
#include <memory>
#include <utility>

namespace i2c {

namespace {

auto const TAG = "I2C Master";

using Result = esp_err_t;
using Configuration = i2c_master_bus_config_t;

} // namespace

auto Master::create(Master::Pin const sdaPin, Master::Pin const sclPin, Master::Port const port) -> std::expected<Master::Pointer, Master::Error> {
  Configuration const configuration = {
      .i2c_port = port,
      .sda_io_num = static_cast<gpio_num_t>(sdaPin),
      .scl_io_num = static_cast<gpio_num_t>(sclPin),
      .clk_source = I2C_CLK_SRC_DEFAULT,
      .glitch_ignore_cnt = 15,
      .intr_priority = 0,
      .flags = {
          .enable_internal_pullup = true,
      },
  };

  BusHandle busHandle{nullptr};

  Result const errorCode = i2c_new_master_bus(&configuration, &busHandle);
  if (errorCode != ESP_OK) {
    ESP_LOGE(TAG, "Bus initialization failed");
    return std::unexpected(Master::Error::CREATE_MASTER_ERROR);
  }

  return Master::Pointer(new (std::nothrow) Master(busHandle));
}

Master::Master(Master::BusHandle const busHandle) noexcept : m_busHandle(busHandle) {}

Master::~Master() noexcept { closeHandle(); }

auto Master::createDevice(Device::Address const deviceAddress) -> std::expected<Device::Pointer, Device::Error> {
  if (not m_busHandle) {
    return std::unexpected(Device::Error::CREATE_DEVICE_ERROR);
  }

  return Device::create(m_busHandle, deviceAddress);
}

auto Master::closeHandle() -> void {
  if (m_busHandle) {
    i2c_del_master_bus(m_busHandle);
  }
}

} // namespace i2c
