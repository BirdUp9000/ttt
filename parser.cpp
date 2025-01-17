#include "parser.hpp"



namespace CBTC {

  namespace Bencode {

    Parser::Parser(std::istream & input_stream) : root(nullptr), is(input_stream) {
      root = decode();
      return;
    } 

    Parser::~Parser() {
      delete root;
    }

    bool Parser::isEmpty() const {
      return root == nullptr;
    }

    BNode * Parser::parseInt() {
      std::string integer;
      char c = is.get();

      while (is.get(c) && c != END_CHAR) {
        if (!isdigit(c)) throw std::runtime_error("Invalid character in 'INT' block.");
        integer += c;
      }

      if (integer.empty()) {
        throw std::runtime_error("Empty integer.");
      }

      if (c != END_CHAR && is.eof()) {
        throw std::runtime_error("Unexpected end of file in 'INT' block.");
      }

      BNode * new_node = new BInteger(std::stoll(integer));
      new_node->type = BType::BTypeInt;

      return new_node;
    }

    BNode * Parser::parseStr() {
      std::string integer;
      char c = is.get();
      integer += c;

      while ((c = is.get()) && STR_CHAR(c)) {
        integer += c;
      }

      if (integer.empty()) {
        throw std::runtime_error("Empty integer.");
      }

      if (c != ':') {
        throw std::runtime_error("String missing ':'.");
      }

      uint64_t length = std::stoll(integer);
      std::string str(length, '\0');
      is.read(&str[0], length);
    
      BNode * new_node = new BString(str);
      new_node->type = BType::BTypeStr;

      return new_node;
    }

    BNode * Parser::parseList() {
      is.get();
      BList * new_node = new BList;

      while (is.peek() != END_CHAR) {
        if (is.eof()) {
          throw std::runtime_error("Unexpected end of file in 'LIST' block.");
        }
        new_node->addItem(decode());
      }

      is.get();
      new_node->type = BType::BTypeList;
      return new_node;
    }

    BNode * Parser::parseDict() {
      is.get();
      BDictionary * new_node = new BDictionary;

      while (is.peek() != END_CHAR) {
        if (is.eof()) {
          throw std::runtime_error("Unexpected end of file in 'DICT' block.");
        }

        BNode * ptr = decode();
        if (ptr->type != BType::BTypeStr) {
          throw std::runtime_error("Key in dictionary must be a string.");
        }

        BString * key_ptr = static_cast<BString *>(ptr);
        std::string key = key_ptr->getValue();
        delete key_ptr;

        BNode * val = decode();
        new_node->addItem(key, val);
      }

      is.get();
      new_node->type = BType::BTypeDict;
      return new_node;
    }

    BNode * Parser::decode() {
      if (!is.good()) {
        throw std::runtime_error("Error reading from the stream.");
      }

      BNode * new_node = nullptr;
      char c = is.peek();

      switch (c) {
        case INT_CHAR : {
          new_node = parseInt();
          break;
        }

        case LIST_CHAR : {
          new_node = parseList();
          break;
        }

        case DICT_CHAR : {
          new_node = parseDict();
          break;
        }

        default : {
          if STR_CHAR(c) {
            new_node = parseStr();
            break;
          }

          else {
            std::cout << "Symbol: " << c << std::endl;
            throw std::runtime_error("Unexpected character while parsing");
          }
        }
      }
      return new_node;
    }

    std::ostream & Parser::DBGprint(std::ostream & os, size_t depth) const {
      if (!root) throw std::runtime_error("Parse file before printing.");

      os << "[.torrent]" << std::endl;
      root->print(os, depth + 1);
      
      return os;
    }

    BNode * Parser::search(const std::string & key) const {
      if (root->type != BType::BTypeDict || !root) return nullptr;
      if (key.empty()) return root;

      BDictionary * tmp = static_cast<BDictionary *>(root);
      return tmp->search(key);
    }

    std::ostream & Parser::encode(std::ostream & os, const BNode * ptr) {
      ptr->encode(os);
      return os;
    }

    const TorrentFileInfo Parser::getClientInfo() const {
      TorrentFileInfo result;

      if (isEmpty()) return result;

      // Handle "announce"
      BNode* announceNode = search("announce");
      if (announceNode && announceNode->type == BType::BTypeStr) {
        result.announce = std::move(static_cast<BString*>(announceNode)->getValue());
        result.isEmpty = false;
      }

      // Handle "announce-list"
      BNode* announceListNode = search("announce-list");
      if (announceListNode && announceListNode->type == BType::BTypeList) {
      std::vector<BNode *> nodes = static_cast<BList*>(announceListNode)->getValue();

        for (const auto& ptr : nodes) {
          if (ptr->type != BType::BTypeList) {
            throw std::runtime_error("Announce-list contains other types than list.");
          }

          std::vector<BNode *> subNodes = static_cast<BList*>(ptr)->getValue();
          for (const auto& subPtr : subNodes) {
            if (subPtr->type != BType::BTypeStr) {
              throw std::runtime_error("Sub element of announce-list is not a string.");
            }
            result.announceList.push_back(std::move(static_cast<BString*>(subPtr)->getValue()));
          }
        }
      }

      // Handle "comment"
      BNode* commentNode = search("comment");
      if (commentNode && commentNode->type == BType::BTypeStr) {
        result.comment = std::move(static_cast<BString*>(commentNode)->getValue());
      }

      // Handle "created by"
      BNode* createdByNode = search("created by");
      if (createdByNode && createdByNode->type == BType::BTypeStr) {
        result.createdBy = std::move(static_cast<BString*>(createdByNode)->getValue());
      }

      // Handle "creation date"
      BNode* creationDateNode = search("creation date");
      if (creationDateNode && creationDateNode->type == BType::BTypeInt) {
        result.creationDate = std::move(static_cast<BInteger*>(creationDateNode)->getValue());
      }

      // Handle "encoding"
      BNode* encodingNode = search("encoding");
      if (encodingNode && encodingNode->type == BType::BTypeStr) {
        result.encoding = std::move(static_cast<BString*>(encodingNode)->getValue());
      }

      // Handle "info"
      std::ostringstream oss;
      BNode* infoNode = search("info");
      encode(oss, infoNode);
      result.src_info = std::move(oss.str());

      // Handle "length"
      BNode* lengthNode = search("length");
      if (lengthNode && lengthNode->type == BType::BTypeInt) {
        result.length = std::move(static_cast<BInteger*>(lengthNode)->getValue());
      }

      // Handle "name"
      BNode* nameNode = search("name");
      if (nameNode && nameNode->type == BType::BTypeStr) {
        result.name = std::move(static_cast<BString*>(nameNode)->getValue());
      }

      // Handle "piece length"
      BNode* pieceLengthNode = search("piece length");
      if (pieceLengthNode && pieceLengthNode->type == BType::BTypeInt) {
        result.piece_length = std::move(static_cast<BInteger*>(pieceLengthNode)->getValue());
      }

      // Handle "publisher"
      BNode* publisherNode = search("publisher");
      if (publisherNode && publisherNode->type == BType::BTypeStr) {
        result.publisher = std::move(static_cast<BString*>(publisherNode)->getValue());
      }

      // Handle "publisher-url"
      BNode* publisherUrlNode = search("publisher-url");
      if (publisherUrlNode && publisherUrlNode->type == BType::BTypeStr) {
        result.publisher_url = std::move(static_cast<BString*>(publisherUrlNode)->getValue());
      }

      return result;
    }

  } // namespace Bencode 

} // namespace CBTC
