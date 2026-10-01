#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

void check(bool good) {
    if (!good) throw std::logic_error("check failed");
}
void digit(int value) {
    if (value < 0 || value > 9) throw std::invalid_argument("digit");
}
class Encoder {
public:
    virtual ~Encoder() = default;
    virtual std::string write(int value) const = 0;
};
class Decoder {
public:
    virtual ~Decoder() = default;
    virtual int read(const std::string& wire) const = 0;
};
class PlainEncoder final : public Encoder {
public:
    std::string write(int value) const override {
        digit(value);
        return std::to_string(value);
    }
};
class PlainDecoder final : public Decoder {
public:
    int read(const std::string& wire) const override {
        if (wire.size() != 1 || wire[0] < '0' || wire[0] > '9')
            throw std::invalid_argument("plain");
        return wire[0] - '0';
    }
};
class FramedEncoder final : public Encoder {
public:
    std::string write(int value) const override {
        digit(value);
        return "[" + std::to_string(value) + "]";
    }
};
class FramedDecoder final : public Decoder {
public:
    int read(const std::string& wire) const override {
        if (wire.size() != 3 || wire.front() != '[' || wire.back() != ']')
            throw std::invalid_argument("frame");
        return PlainDecoder{}.read(wire.substr(1, 1));
    }
};
class CodecFactory {
public:
    virtual ~CodecFactory() = default;
    // Create both members of one family.
    virtual std::unique_ptr<Encoder>
    encoder() const = 0;
    virtual std::unique_ptr<Decoder>
    decoder() const = 0;
};
class PlainFactory final : public CodecFactory {
public:
    std::unique_ptr<Encoder>
    encoder() const override {
        return std::make_unique<PlainEncoder>();
    }
    std::unique_ptr<Decoder>
    decoder() const override {
        return std::make_unique<PlainDecoder>();
    }
};
class FramedFactory final : public CodecFactory {
public:
    std::unique_ptr<Encoder>
    encoder() const override {
        return std::make_unique<FramedEncoder>();
    }
    std::unique_ptr<Decoder>
    decoder() const override {
        return std::make_unique<FramedDecoder>();
    }
};
std::string round_trip(const CodecFactory& factory, int value) {
    // Borrow factory; own both products.
    auto encoder = factory.encoder();
    auto decoder = factory.decoder();
    if (!encoder || !decoder)
        throw std::logic_error("null codec");
    const auto wire = encoder->write(value);
    check(decoder->read(wire) == value);
    return wire;
}

class TaggedEncoder final : public Encoder {
public:
    std::string write(int value) const override {
        digit(value);
        return "d=" + std::to_string(value);
    }
};
class TaggedDecoder final : public Decoder {
public:
    int read(const std::string& wire) const override {
        if (wire.size() != 3 || wire.substr(0, 2) != "d=")
            throw std::invalid_argument("tag");
        return PlainDecoder{}.read(wire.substr(2, 1));
    }
};
class TaggedFactory final : public CodecFactory {
public:
    std::unique_ptr<Encoder> encoder() const override {
        return std::make_unique<TaggedEncoder>();
    }
    std::unique_ptr<Decoder> decoder() const override {
        return std::make_unique<TaggedDecoder>();
    }
};

// Return true only for the requested exception type.
#include <utility>
template<class E, class F> bool dpx_rejects(F&& action) {
  try { std::forward<F>(action)(); }
  catch (const E&) { return true; }
  return false;
}

int main() {
std::cout << std::boolalpha << ([]{
  TaggedFactory f;
  return round_trip(f,9);
})() << '\n';
}
