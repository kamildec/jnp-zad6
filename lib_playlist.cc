#include "lib_playlist.h"

unsigned int Playlist::lastId = 0;

std::shared_ptr<SequenceMode> createSequenceMode() {
    return std::make_shared<SequenceMode>();
}

std::shared_ptr<ShuffleMode> createShuffleMode(unsigned long seed) {
    return std::make_shared<ShuffleMode>(seed);
}

std::shared_ptr<OddEvenMode> createOddEvenMode() {
    return std::make_shared<OddEvenMode>();
}