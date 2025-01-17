#ifndef CBTC_BENCODE_DECODE_STR_HPP
#define CBTC_BENCODE_DECODE_STR_HPP

#include "node.hpp"
#include <string>

/*
  Byte Strings
  Byte strings are encoded as follows:
  <string length encoded in base ten ASCII>:<string data>
  Note that there is no constant beginning delimiter, and no ending delimiter.

  Example: 4: spam represents the string "spam"
  Example: 0: represents the empty string ""
*/

namespace CBTC {
  
  namespace Bencode {

    class BString : public BNode {
      public:
        explicit BString(std::string value);
        ~BString() = default;
        const std::string & getValue() const;
        void print(std::ostream& os, size_t depth = 0) const override;
        std::ostream & encode(std::ostream & os) const override;

      private:
        std::string value;
    };

  } // namespace Bencode

} // namespace CBTC

#endif // CBTC_BENCODE_DECODE_STR_HPP