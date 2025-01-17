#include "torrentFileInfo.hpp"

namespace CBTC {

  namespace Bencode {

    TorrentFileInfo::TorrentFileInfo() :
      isEmpty(true),
      creationDate(0),
      length(0),
      piece_length(0)
    {}

  } // namespace Bencode

} // namespace CBTC

