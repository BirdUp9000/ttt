#ifndef CBTC_BENCODE_PARSER_HPP
#define CBTC_BENCODE_PARSER_HPP

#include "types/node.hpp"
#include "types/int.hpp"
#include "types/str.hpp"
#include "types/list.hpp"
#include "types/dict.hpp"
#include "torrentFileInfo.hpp"

#include <arpa/inet.h>
#include <istream>
#include <sstream>
#include <cstring>

#define INT_CHAR      'i'
#define STR_CHAR(c)   (isdigit(c))
#define LIST_CHAR     'l'
#define DICT_CHAR     'd'
#define END_CHAR      'e'



namespace CBTC {

  namespace Bencode {

    class Parser {
      public:
        Parser(std::istream & input_stream);
        Parser(Parser &&) = delete;
        Parser & operator = (const Parser) = delete;
        Parser(const Parser &) = delete;
        void operator = (const Parser &) = delete;
        ~Parser();

        bool isEmpty() const;
        std::ostream & DBGprint(std::ostream & os, size_t depth = 1) const;
        const TorrentFileInfo getClientInfo() const;
        BNode * search(const std::string & key) const;
        static std::ostream & encode(std::ostream & os, const BNode * ptr);

      private:
        BNode * root;
        std::istream & is;
        BNode * decode();
        BNode * parseInt();
        BNode * parseStr();
        BNode * parseList();
        BNode * parseDict();
    };

  }; // namespace Bencode

} // namespace CBTC

#endif // CBTC_BENCODE_PARSER_HPP