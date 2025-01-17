#include "int.hpp"


namespace CBTC {

  namespace Bencode {

    BInteger::BInteger(int64_t val) : value(val) {}

    const int64_t & BInteger::getValue() const {return value;}

    void BInteger::print(std::ostream& os, size_t depth) const {
      os << std::string(depth, ' ') << value << std::endl;
      return;
    }

    std::ostream & BInteger::encode(std::ostream & os) const {
      os << 'i' << std::to_string(value) << 'e';
      return os;
    }

  } // namespace Bencode

} // namespace CBTC