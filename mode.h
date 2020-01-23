#ifndef PLAYLIST_MODE_H
#define PLAYLIST_MODE_H

using position_t = uint32_t;

class Mode {
public:
    virtual void play(const std::deque<std::shared_ptr<Playable>> &deque) = 0;
};


class ShuffleMode : public Mode {
    std::default_random_engine   engine;

public:
    explicit ShuffleMode(unsigned long seed) : engine(seed) {}

    // Tworzy pseudolosową permutację  - kolejność odtwarzania składanki.
    void play(const std::deque<std::shared_ptr<Playable>> &deque) override {
        std::vector<position_t> seq(deque.size());
        std::iota(seq.begin(), seq.end(), 0);
        std::shuffle(seq.begin(), seq.end(), engine);

        for (position_t i : seq) {
            deque[i]->play();
        }
    }
};

class SequenceMode : public Mode {
public:
    void play(const std::deque<std::shared_ptr<Playable>> &deque) override {
        for (const auto &it : deque) {
            it->play();
        }
    }
};

class OddEvenMode : public Mode {
public:
    void play(const std::deque<std::shared_ptr<Playable>> &deque) override {
        size_t i = 1;
        while (i < deque.size()) {
            deque[i]->play();
            i += 2;
        }
        i = 0;
        while (i < deque.size()) {
            deque[i]->play();
            i += 2;
        }
    }
};


#endif //PLAYLIST_MODE_H
