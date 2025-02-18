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

#include <driver/i2c_master.h>
#include <esp_log.h>

namespace i2c {

auto const TAG = "I2C";

auto const I2C_TIMEOUT_MS = 1000;

Device::Device(Device::BusHandle const busHandle, Device::DeviceAddress const deviceAddress) {
  i2c_device_config_t const deviceConfiguration = {
      .device_address = deviceAddress,
      .scl_speed_hz = 100000,
      .flags = {
          .disable_ack_check = true,
      }
  };

  ESP_ERROR_CHECK(i2c_master_bus_add_device(busHandle, &deviceConfiguration, &m_deviceHandle));
}

Device::~Device() {
  ESP_ERROR_CHECK(i2c_master_bus_rm_device(m_deviceHandle));
}

Device::Bytes Device::read(Device::RegisterAddress registerAddress, Device::Size packageSize) {
  Device::Byte buffer[32] = {};

  ESP_ERROR_CHECK(i2c_master_transmit_receive(m_deviceHandle, &registerAddress, 1, buffer, packageSize, I2C_TIMEOUT_MS));

  return {buffer, buffer + packageSize};
}

void Device::write(Device::RegisterAddress registerAddress, Device::Bytes bytes) {
  Device::Bytes registerWithData{};

  registerWithData.push_back(registerAddress);
  registerWithData.insert(registerWithData.end(), bytes.begin(), bytes.end());

  ESP_ERROR_CHECK(i2c_master_transmit(m_deviceHandle, registerWithData.data(), registerWithData.size(), I2C_TIMEOUT_MS));
}

}// namespace i2c
