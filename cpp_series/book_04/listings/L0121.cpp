#pragma once
#include <array>
#include <cstddef>
#include <climits>
#include <cstdint>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace harbor {
static_assert(CHAR_BIT == 8, "Wire protocol requires eight-bit bytes");
inline constexpr std::size_t max_payload = 4096;
inline std::size_t decode_length(std::span<const std::byte, 4> h) {
    std::uint32_t n = 0;
    for (auto b : h) n = (n << 8) | std::to_integer<unsigned>(b);
    if (n > max_payload) throw std::length_error("frame exceeds 4096 bytes");
    return n;
}
inline std::vector<std::byte> encode_frame(std::string_view payload) {
    if (payload.size() > max_payload) throw std::length_error("payload too large");
    const auto n = static_cast<std::uint32_t>(payload.size());
    std::vector<std::byte> bytes(4 + payload.size());
    for (unsigned i = 0; i != 4; ++i)
        bytes[i] = static_cast<std::byte>((n >> (24 - 8 * i)) & 255u);
    for (std::size_t i = 0; i != payload.size(); ++i)
        bytes[i + 4] = static_cast<std::byte>(static_cast<unsigned char>(payload[i]));
    return bytes;
}
// One frame per decoder. feed returns how many bytes it consumed, leaving
// any next frame to the caller. No unbounded input buffer is retained.
class FrameDecoder {
    std::array<std::byte, 4> header_{};
    std::size_t header_used_ = 0;
    std::size_t needed_ = 0;
    bool complete_ = false;
    bool failed_ = false;
    std::string payload_;
public:
    std::size_t feed(std::span<const std::byte> input) {
        if (failed_) throw std::logic_error("decoder is failed");
        std::size_t consumed = 0;
        try {
            while (consumed < input.size() && !complete_) {
                if (header_used_ < 4) {
                    header_[header_used_++] = input[consumed++];
                    if (header_used_ == 4) {
                        needed_ = decode_length(header_);
                        payload_.reserve(needed_);
                        complete_ = needed_ == 0;
                    }
                } else {
                    payload_.push_back(static_cast<char>(
                        std::to_integer<unsigned char>(input[consumed++])));
                    complete_ = payload_.size() == needed_;
                }
            }
        } catch (...) { failed_ = true; throw; }
        return consumed;
    }
    bool complete() const noexcept { return complete_; }
    bool finish() const {
        if (failed_) throw std::logic_error("decoder is failed");
        if (!complete_ && header_used_ != 0)
            throw std::runtime_error("truncated frame");
        return complete_; // false is clean EOF before a new header
    }
    const std::string& payload() const {
        if (!complete_ || failed_) throw std::logic_error("frame not complete");
        return payload_; // borrowed until this decoder is changed/destroyed
    }
};
} // namespace harbor
