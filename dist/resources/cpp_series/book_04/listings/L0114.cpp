#include "frame.hpp"
#include "check.hpp"
#include <iostream>
#include <span>
#include <string>

int main() {
    try {
        std::size_t trials = 0;
        for (std::size_t size = 0; size <= 64; ++size) {
            std::string original(size, '\0');
            for (std::size_t i = 0; i < size; ++i)
                original[i] = static_cast<char>((i * 37 + size) & 255);
            auto wire = harbor::encode_frame(original);
            // Exercise every pair of chunk boundaries for each bounded payload.
            for (std::size_t a = 0; a <= wire.size(); ++a) {
                for (std::size_t b = a; b <= wire.size(); ++b) {
                    harbor::FrameDecoder decoder;
                    CHECK(decoder.feed(std::span(wire).first(a)) == a);
                    CHECK(decoder.feed(std::span(wire).subspan(a, b - a)) == b - a);
                    CHECK(decoder.feed(std::span(wire).subspan(b)) == wire.size() - b);
                    CHECK(decoder.finish()); CHECK(decoder.payload() == original);
                    ++trials;
                }
            }
        }
        // A manually specified independent wire value, not round-trip alone.
        const std::byte bytes[]{std::byte{0}, std::byte{0}, std::byte{0}, std::byte{1}, std::byte{255}};
        harbor::FrameDecoder one;
        CHECK(one.feed(bytes) == 5);
        CHECK(one.payload().size() == 1);
        CHECK(static_cast<unsigned char>(one.payload()[0]) == 255);
        std::cout << "PASS chunk_histories=" << trials << '\n';
        return 0;
    } catch (const std::exception& e) { std::cerr << e.what() << '\n'; return 1; }
}
