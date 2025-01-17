#ifndef CBTC_BENCODE_DECODE_INT_HPP
#define CBTC_BENCODE_DECODE_INT_HPP

#include "node.hpp"
#include <cstdint>

/*
  Integers
  Integers are encoded as follows: i<integer encoded in base ten ASCII>e
  The initial i and trailing e are beginning and ending delimiters.

  Example: i3e represents the integer "3"
  Example: i-3e represents the integer "-3"
  i-0e is invalid. All encodings with a leading zero, such as i03e,
  are invalid, other than i0e, which of course corresponds to the integer "0".

  NOTE: The maximum number of bit of this integer is unspecified,
  but to handle it as a signed 64bit integer is mandatory to handle 
  "large files" aka .torrent for more that 4Gbyte.
*/

namespace CBTC {

  namespace Bencode {

    class BInteger : public BNode {
      public:
        explicit BInteger(int64_t value);
        ~BInteger() = default;
        const int64_t & getValue() const;
        void print(std::ostream& os, size_t depth = 0) const override;
        std::ostream & encode(std::ostream & os) const override;

      private:
        int64_t value;
    };

  } // namespace Bencode

} // namespace CBTC

#endif // CBTC_BENCODE_DECODE_INT_HPP