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

#include <memory>

namespace i2c {

namespace {

using Error = esp_err_t;

} // namespace

auto Master::createMaster(Master::Pin const sdaPin, Master::Pin const sclPin, Master::Port const port) -> Master::MasterPointer {
  i2c_master_bus_config_t const masterConfiguration = {
      .i2c_port = port,
      .sda_io_num = static_cast<gpio_num_t>(sdaPin),
      .scl_io_num = static_cast<gpio_num_t>(sclPin),
      .clk_source = I2C_CLK_SRC_DEFAULT,
      .glitch_ignore_cnt = 15,
      .intr_priority = 0,
      .flags =
          {
              .enable_internal_pullup = true,
          },
  };

  BusHandle busHandle = nullptr;

  Error const errorCode = i2c_new_master_bus(&masterConfiguration, &busHandle);
  if (errorCode != ESP_OK) {
    return nullptr;
  }

  return Master::MasterPointer(new Master(busHandle));
}

Master::Master(Master::BusHandle busHandle) : busHandle(busHandle) {}

Master::~Master() { i2c_del_master_bus(busHandle); }

auto Master::createDevice(DeviceAddress const address) -> Master::DevicePointer {
  auto const isDeviceFound = i2c_master_probe(busHandle, address, -1);
  if (not isDeviceFound) {
    return nullptr;
  }

  return std::make_unique<Device>(busHandle, address);
}

} // namespace i2c
