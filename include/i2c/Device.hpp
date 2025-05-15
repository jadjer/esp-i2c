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

#include <cstddef>
#include <cstdint>
#include <driver/i2c_types.h>
#include <expected>
#include <memory>
#include <vector>

/**
 * @namespace i2c
 */
namespace i2c {

/**
 * @class Device
 * @brief Slave device implementation
 */
class Device {
public:
  enum class Error : std::uint8_t {
    DEVICE_NOT_FOUND,
    CREATE_DEVICE_ERROR,
  };

public:
  using Bus = i2c_master_bus_handle_t;
  using Byte = std::uint8_t;
  using Size = std::size_t;
  using Bytes = std::vector<Byte>;
  using Address = std::uint16_t;
  using Pointer = std::unique_ptr<Device>;
  using Register = Byte;

private:
  using DeviceHandle = i2c_master_dev_handle_t;

public:
  static auto create(Bus bus, Address address) -> std::expected<Pointer, Error>;

private:
  explicit Device(DeviceHandle deviceHandle) noexcept;

public:
  ~Device() noexcept;

public:
  /**
   * Read bytes from device
   * @param registerAddress Register address
   * @param packageSize Package size for read
   * @return Vector of bytes
   */
  [[maybe_unused]] auto read(Register registerAddress, Size packageSize = 1) -> Bytes;
  /**
   * Write bytes to device
   * @param registerAddress Register address
   * @param bytes Data package
   */
  [[maybe_unused]] auto write(Register registerAddress, Bytes const &bytes) -> void;

private:
  auto closeHandle() -> void;

private:
  DeviceHandle const m_deviceHandle;
};

} // namespace i2c
