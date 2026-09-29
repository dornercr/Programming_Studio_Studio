#pragma once
#include <cstdint>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>
namespace harbor {
inline constexpr std::size_t max_frame = 4096;
inline std::string frame(std::string_view payload) {
    if (payload.size() > max_frame) throw std::length_error("oversize frame");
    auto n = static_cast<std::uint32_t>(payload.size());
    std::string out(4, '\0');
    for (int i=0; i<4; ++i) out[static_cast<std::size_t>(i)] =
        static_cast<char>((n >> (24 - 8*i)) & 255U);
    out.append(payload);
    return out;
}
// One decoder instance per connection. It accepts one message, not a stream
// of unlimited messages. An error poisons the connection: close it.
class Decoder {
    std::string bytes_;
    std::size_t expected_ = 0;
    bool header_ = false;
    bool failed_ = false;
public:
    void feed(std::string_view chunk) {
        if (failed_) throw std::runtime_error("decoder already failed");
        if (chunk.size() > max_frame + 4 - bytes_.size()) {
            failed_ = true; throw std::length_error("frame buffer limit");
        }
        bytes_.append(chunk);
        if (!header_ && bytes_.size() >= 4) {
            for (std::size_t i=0; i<4; ++i)
                expected_ = (expected_ << 8) | static_cast<unsigned char>(bytes_[i]);
            header_ = true;
            if (expected_ > max_frame) {
                failed_ = true; throw std::length_error("declared frame limit");
            }
        }
        if (header_ && bytes_.size() > expected_ + 4) {
            failed_ = true; throw std::runtime_error("trailing frame bytes");
        }
    }
    bool ready() const { return !failed_ && header_ && bytes_.size() == expected_ + 4; }
    std::string_view payload() const {
        if (!ready()) throw std::runtime_error("incomplete frame");
        return std::string_view(bytes_).substr(4);
    }
    void finish() const { if (!ready()) throw std::runtime_error("truncated frame"); }
};
}
