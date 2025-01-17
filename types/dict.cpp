#include "dict.hpp"



namespace CBTC {

  namespace Bencode {
    
    BDictionary::~BDictionary() {
      for (const auto & ptr : value) {
        delete ptr.second;
      }
    }
    
    void BDictionary::print(std::ostream& os, size_t depth) const {
      for (const auto& item : value) {
        os << std::string(depth, ' ') << '[' << item.first << ']' << std::endl;
        item.second->print(os, depth + 2);
      }
      return;
    }

    const std::map<std::string, BNode *> & BDictionary::getValue() const {
      return value;
    }

    void BDictionary::addItem(std::string key, BNode * val) {
      value[std::move(key)] = std::move(val);
      return;
    }
    
    BNode * BDictionary::search(const std::string & key) const {
      if (value.empty()) return nullptr;

      auto it = value.find(key);

      if (it != value.end()) {
        return it->second;

      } else {
        for (const auto & ptr : value) {
          if (ptr.second->type == BType::BTypeDict) {
            BDictionary * tmp = static_cast<BDictionary *>(ptr.second);
            BNode * ans = tmp->search(key);
            if (ans) return ans;
          }
        }
      }
      
      return nullptr;
    }

    std::ostream & BDictionary::encode(std::ostream & os) const {
      os << 'd';

      for (const auto & ptr : value) {
        os << ptr.first.size() << ':' << ptr.first;
        ptr.second->encode(os);
      }

      os << 'e';

      return os;
    }

  } // namespace Bencode

} // namespace CBTC