#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <stdexcept>

namespace ioniq5_ecan {

enum class ByteOrder { LittleEndian, BigEndian };

template <std::size_t N>
void set_signal(std::array<uint8_t, N>& data, unsigned start_bit, unsigned bit_length, uint64_t raw,
                ByteOrder order) {
  if (bit_length == 0U || bit_length > 64U) {
    throw std::invalid_argument("invalid CAN signal length");
  }

  if (order == ByteOrder::LittleEndian) {
    if (start_bit + bit_length > N * 8U) {
      throw std::out_of_range("CAN signal exceeds frame");
    }
    for (unsigned i = 0; i < bit_length; ++i) {
      const unsigned dst = start_bit + i;
      const uint8_t mask = static_cast<uint8_t>(1U << (dst % 8U));
      data[dst / 8U] = static_cast<uint8_t>((data[dst / 8U] & static_cast<uint8_t>(~mask)) |
                                            (((raw >> i) & 1U) ? mask : 0U));
    }
    return;
  }

  int dbc_bit = static_cast<int>(start_bit);
  for (unsigned i = 0; i < bit_length; ++i) {
    if (dbc_bit < 0 || static_cast<unsigned>(dbc_bit) >= N * 8U) {
      throw std::out_of_range("Motorola CAN signal exceeds frame");
    }
    const unsigned dst = static_cast<unsigned>(dbc_bit);
    const uint8_t mask = static_cast<uint8_t>(1U << (dst % 8U));
    const unsigned src = bit_length - 1U - i;
    data[dst / 8U] = static_cast<uint8_t>((data[dst / 8U] & static_cast<uint8_t>(~mask)) |
                                          (((raw >> src) & 1U) ? mask : 0U));
    dbc_bit = (dbc_bit % 8 == 0) ? dbc_bit + 15 : dbc_bit - 1;
  }
}

template <std::size_t N>
uint64_t get_signal(const std::array<uint8_t, N>& data, unsigned start_bit, unsigned bit_length,
                    ByteOrder order) {
  if (bit_length == 0U || bit_length > 64U) {
    throw std::invalid_argument("invalid CAN signal length");
  }

  uint64_t raw = 0;
  if (order == ByteOrder::LittleEndian) {
    if (start_bit + bit_length > N * 8U) {
      throw std::out_of_range("CAN signal exceeds frame");
    }
    for (unsigned i = 0; i < bit_length; ++i) {
      const unsigned src = start_bit + i;
      raw |= static_cast<uint64_t>((data[src / 8U] >> (src % 8U)) & 1U) << i;
    }
    return raw;
  }

  int dbc_bit = static_cast<int>(start_bit);
  for (unsigned i = 0; i < bit_length; ++i) {
    if (dbc_bit < 0 || static_cast<unsigned>(dbc_bit) >= N * 8U) {
      throw std::out_of_range("Motorola CAN signal exceeds frame");
    }
    raw <<= 1U;
    raw |=
      (data[static_cast<unsigned>(dbc_bit) / 8U] >> (static_cast<unsigned>(dbc_bit) % 8U)) & 1U;
    dbc_bit = (dbc_bit % 8 == 0) ? dbc_bit + 15 : dbc_bit - 1;
  }
  return raw;
}

inline int64_t sign_extend(uint64_t value, unsigned bit_length) {
  if (bit_length == 0U || bit_length > 64U) {
    throw std::invalid_argument("invalid signed signal length");
  }
  if (bit_length == 64U) {
    return static_cast<int64_t>(value);
  }
  const uint64_t sign = uint64_t{1} << (bit_length - 1U);
  return static_cast<int64_t>((value ^ sign) - sign);
}

}  // namespace ioniq5_ecan
