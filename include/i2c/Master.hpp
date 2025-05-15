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
#include <driver/i2c_master.h>
#include <expected>
#include <i2c/Device.hpp>
#include <memory>
#include <string>

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
  enum class Error : std::uint8_t {
    CREATE_MASTER_ERROR,
  };

public:
  using Pin = std::uint8_t;
  using Port = std::uint8_t;
  using Pointer = std::unique_ptr<Master>;

private:
  using BusHandle = i2c_master_bus_handle_t;

public:
  static auto create(Pin sdaPin, Pin sclPin, Port port) -> std::expected<Pointer, Error>;

private:
  explicit Master(BusHandle busHandle) noexcept;

public:
  ~Master() noexcept;

public:
  /**
   * Create new slave device
   * @param address Device address
   * @return Pointer of new device object
   */
  auto createDevice(Device::Address deviceAddress) -> std::expected<Device::Pointer, Device::Error>;

private:
  auto closeHandle() -> void;

private:
  BusHandle const m_busHandle;
};

} // namespace i2c
