#ifndef CBTC_BENCODE_NODE_HPP
#define CBTC_BENCODE_NODE_HPP

#include <iostream>

namespace CBTC {

  namespace Bencode {

    enum BType {
      BTypeInt,
      BTypeStr,
      BTypeList,
      BTypeDict,
    };

    struct BNode {
      BNode() = default;
      BNode(const BNode&) = delete;
      BNode(BNode&&) = delete;
      BNode& operator=(const BNode&) = delete;
      BNode& operator=(BNode&&) = delete;
      virtual ~BNode() = 0;

      virtual void print(std::ostream& os, size_t depth = 0) const = 0;
      virtual std::ostream & encode(std::ostream & os) const = 0;
      BType type;
    };

  } // namespace Bencode

} // namespace CBTC

#endif // CBTC_BENCODE_NODE_HPP