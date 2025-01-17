#include "list.hpp"



namespace CBTC {

  namespace Bencode {

    BList::~BList() {
      for (const auto & ptr : value) {
        delete ptr;
      }
    }

    void BList::print(std::ostream& os, size_t depth) const {
      for (const auto& item : value) {
        item->print(os, depth + 2);
      }
      return;
    }

    const std::vector<BNode *> & BList::getValue() const {return value;}

    void BList::addItem(BNode * val) {
      value.push_back(std::move(val));
      return;
    }

    std::ostream & BList::encode(std::ostream & os) const {
      os << 'l';

      for (const auto & ptr : value) {
        ptr->encode(os);
      }

      os << 'e';

      return os;
    }

  } // namespace Bencode

} // namespace CBTC