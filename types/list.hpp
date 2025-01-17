#ifndef CBTC_BENCODE_DECODE_LIST_HPP
#define CBTC_BENCODE_DECODE_LIST_HPP

#include "node.hpp"
#include <vector>

/*
  Lists
  Lists are encoded as follows: l<bencoded values>e
  The initial l and trailing e are beginning and ending delimiters.
  Lists may contain any bencoded type, including integers, strings,
  dictionaries, and even lists within other lists.

  Example: l4:spam4:eggse represents the list of two strings:
  [ "spam", "eggs" ]

  Example: le represents an empty list: []
*/

namespace CBTC {

  namespace Bencode {

    class BList : public BNode {
      public:
        ~BList() override;
        void print(std::ostream& os, size_t depth = 0) const override;
        const std::vector<BNode *> & getValue() const;
        void addItem(BNode * val);
        std::ostream & encode(std::ostream & os) const override;

      private:
        std::vector<BNode *> value;
    };

  } // namespace Bencode

} // namespace CBTC

#endif // CBTC_BENCODE_DECODE_LIST_HPP