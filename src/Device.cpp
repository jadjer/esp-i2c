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

namespace i2c {

Device::Device(Device::BusHandle const busHandle, Device::DeviceAddress const deviceAddress) {
  i2c_device_config_t const deviceConfiguration = {
      .dev_addr_length = I2C_ADDR_BIT_LEN_7,
      .device_address = deviceAddress,
      .scl_speed_hz = 100000,
      .scl_wait_us = 1,
      .flags = {
          .disable_ack_check = false,
      },
  };

  ESP_ERROR_CHECK(i2c_master_bus_add_device(busHandle, &deviceConfiguration, &m_deviceHandle));
}

Device::~Device() {
  ESP_ERROR_CHECK(i2c_master_bus_rm_device(m_deviceHandle));
}

void Device::read() {
  ESP_ERROR_CHECK(i2c_master_transmit_receive(m_deviceHandle, nullptr, 0, nullptr, 0, -1));
}

void Device::write() {
  ESP_ERROR_CHECK(i2c_master_transmit(m_deviceHandle, nullptr, 0, -1));
  ESP_ERROR_CHECK(i2c_master_receive(m_deviceHandle, nullptr, 0, -1));
}

}// namespace i2c
