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

#include <cstdint>
#include <driver/i2c_types.h>

namespace i2c {

class Device {
public:
  using BusHandle = i2c_master_bus_handle_t;
  using DeviceHandle = i2c_master_dev_handle_t;
  using DeviceAddress = std::uint16_t;

public:
  Device(Device::BusHandle busHandle, Device::DeviceAddress deviceAddress);
  ~Device();

public:
  void read();
  void write();

private:
  Device::DeviceHandle m_deviceHandle = nullptr;
};

}// namespace i2c

#include <memory>

using DevicePtr = std::unique_ptr<i2c::Device>;
