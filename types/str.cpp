#include "str.hpp"



namespace CBTC {

  namespace Bencode {

    BString::BString(std::string val) : value(val) {}

    const std::string & BString::getValue() const {return value;}

    void BString::print(std::ostream& os, size_t depth) const {
      if (value.size() >= 100) {
        os << std::string(depth, ' ') << "[...]" << std::endl;
      } else {
        os << std::string(depth, ' ') << value << std::endl;
      }
      return;
    }

    std::ostream & BString::encode(std::ostream & os) const {
      os << value.size() << ':' << value;
      return os;
    }

  } // namespace Bencode

} // namespace CBTC
