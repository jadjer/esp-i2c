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

#include "i2c/Device.hpp"

#include <array>
#include <driver/i2c_master.h>
#include <esp_log.h>
#include <utility>

namespace i2c {

namespace {

auto const TAG = "I2C Device";
auto const I2C_TIMEOUT_MS = 1000;

using Result = esp_err_t;
using Configuration = i2c_device_config_t;

} // namespace

auto Device::create(Bus const bus, Address const address) -> std::expected<Device::Pointer, Device::Error> {
  Result const deviceFoundResult = i2c_master_probe(bus, address, -1);
  if (deviceFoundResult != ESP_OK) {
    ESP_LOGE(TAG, "Device %d not found", address);
    return std::unexpected(Device::Error::DEVICE_NOT_FOUND);
  }

  Configuration const configuration = {
      .device_address = address,
      .scl_speed_hz = 100000,
      .flags =
          {
              .disable_ack_check = true,
          },
  };

  DeviceHandle deviceHandle{nullptr};

  Result const result = i2c_master_bus_add_device(bus, &configuration, &deviceHandle);
  if (result != ESP_OK) {
    return std::unexpected(Device::Error::CREATE_DEVICE_ERROR);
  }

  return Device::Pointer(new (std::nothrow) Device(deviceHandle));
}

Device::Device(Device::DeviceHandle deviceHandle_) noexcept : m_deviceHandle(deviceHandle_) {}

Device::~Device() noexcept { closeHandle(); }

auto Device::read(Device::Register const registerAddress, Device::Size const packageSize) -> Device::Bytes {
  std::array<Device::Byte, 32> buffer{};

  Result const result = i2c_master_transmit_receive(m_deviceHandle, &registerAddress, 1, buffer.data(), packageSize, I2C_TIMEOUT_MS);
  if (result == ESP_OK) {
    return {buffer.begin(), buffer.begin() + packageSize};
  }
  if (result == ESP_ERR_INVALID_ARG) {
    ESP_LOGE(TAG, "I2C master transmit parameter invalid");
  }
  if (result == ESP_ERR_TIMEOUT) {
    ESP_LOGE(TAG, "Operation timeout(larger than xfer_timeout_ms) because the bus is busy or hardware crash");
  }

  return {};
}

auto Device::write(Device::Register const registerAddress, Device::Bytes const &bytes) -> void {
  Device::Bytes registerWithData{};

  registerWithData.push_back(registerAddress);
  registerWithData.insert(registerWithData.end(), bytes.begin(), bytes.end());

  Result const result = i2c_master_transmit(m_deviceHandle, registerWithData.data(), registerWithData.size(), I2C_TIMEOUT_MS);
  if (result == ESP_ERR_INVALID_ARG) {
    ESP_LOGE(TAG, "I2C master transmit parameter invalid");
  }
  if (result == ESP_ERR_TIMEOUT) {
    ESP_LOGE(TAG, "Operation timeout(larger than xfer_timeout_ms) because the bus is busy or hardware crash.");
  }
}

auto Device::closeHandle() -> void {
  if (m_deviceHandle) {
    i2c_master_bus_rm_device(m_deviceHandle);
  }
}

} // namespace i2c
