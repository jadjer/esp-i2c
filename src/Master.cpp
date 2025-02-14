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

namespace i2c {

Master::Master(Master::Pin const sdaPin, Master::Pin const sclPin, Master::Port const port) {
  i2c_master_bus_config_t const masterConfiguration = {
      .i2c_port = port,
      .sda_io_num = static_cast<gpio_num_t>(sdaPin),
      .scl_io_num = static_cast<gpio_num_t>(sclPin),
      .clk_source = I2C_CLK_SRC_DEFAULT,
      .glitch_ignore_cnt = 7,
      .intr_priority = 0,
      .trans_queue_depth = 100,
      .flags = {
          .enable_internal_pullup = false,
          .allow_pd = false,
      },
  };

  ESP_ERROR_CHECK(i2c_new_master_bus(&masterConfiguration, &m_busHandle));
}

Master::~Master() {
  ESP_ERROR_CHECK(i2c_del_master_bus(m_busHandle));
}

DevicePtr Master::createDevice(DeviceAddress const address) {
  auto const isDeviceFound = i2c_master_probe(m_busHandle, address, -1);
  if (isDeviceFound) {
    return std::make_unique<Device>(m_busHandle, address);
  }

  return nullptr;
}

}// namespace i2c
