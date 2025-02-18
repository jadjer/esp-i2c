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

#pragma once

#include <driver/i2c_types.h>
#include <i2c/Device.hpp>

/**
 * @namespace i2c
 */
namespace i2c {

/**
 * @class Master
 * @brief I2C master device implementation
 */
class Master {
public:
  using Pin = std::uint8_t;
  using Port = std::uint8_t;
  using BusHandle = Device::BusHandle;
  using DeviceAddress = Device::DeviceAddress;

public:
  Master(Pin sdaPin, Pin sclPin, Port port = 0);
  ~Master();

public:
  /**
   * Create new slave device
   * @param address Device address
   * @return Pointer of new device object
   */
  DevicePtr createDevice(Master::DeviceAddress address);

private:
  Master::BusHandle m_busHandle = nullptr;
};

}// namespace i2c
