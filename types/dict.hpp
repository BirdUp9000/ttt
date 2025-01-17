#ifndef CBTC_BENCODE_DECODE_DICT_HPP
#define CBTC_BENCODE_DECODE_DICT_HPP

#include "node.hpp"
#include <string>
#include <map>

/*
  Dictionaries
  Dictionaries are encoded as follows: d<bencoded string><bencoded element>e
  The initial d and trailing e are the beginning and ending delimiters.
  Note that the keys must be bencoded strings. The values may be any bencoded
  type, including integers, strings, lists, and other dictionaries. Keys must
  be strings and appear in sorted order (sorted as raw strings, not alphanumerics).
  The strings should be compared using a binary comparison, not a culture-specific
  "natural" comparison.

  Example: d3:cow3:moo4:spam4:eggse represents the dictionary
  { "cow" => "moo", "spam" => "eggs" }

  Example: d4:spaml1:a1:bee represents the dictionary
  { "spam" => [ "a", "b" ] }

  Example: de represents an empty dictionary {}"
*/

namespace CBTC {

  namespace Bencode {

    class BDictionary : public BNode {
      public:
        ~BDictionary();
        void print(std::ostream& os, size_t depth = 0) const override;
        const std::map<std::string, BNode *> & getValue() const;
        void addItem(std::string key, BNode * val);
        BNode * search(const std::string & key) const;
        std::ostream & encode(std::ostream & os) const override;

      private:
        std::map<std::string, BNode *> value;
    };

  } // namespace Bencode

} // namespace CBTC

#endif // CBTC_BENCODE_DECODE_DICT_HPP