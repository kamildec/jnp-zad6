#ifndef LIB_PLAYLIST_H
#define LIB_PLAYLIST_H

#include <bits/stdc++.h>

using position_t = uint32_t;

class PlayerException : std::exception {
private:
    std::string m_error;
public:
    explicit PlayerException(const char *error) : m_error(error) {}

    explicit PlayerException(const std::string &error) : m_error(error) {}

    [[nodiscard]] const char *what() const noexcept override {
        return m_error.c_str();
    }
};

class File {
private:
    std::string description;

public:
    explicit File(const char *file) {}
};

class Playable {
    virtual void play() {}
};

class Song : public File, public Playable {
private:
    std::string artist;
    std::string title;

public:
    void play() {}
};

class Movie : public File, public Playable {
private:
    std::string title;
    std::string year;

public:
    void play() {}
};

class Mode {

};

Mode *createSequenceMode() {
    return nullptr;
}

Mode *createShuffleMode(unsigned long seed) {
    return nullptr;
}

Mode *createOddEvenMode() {
    return nullptr;
}


class Playlist : public Playable {
public:
    void add(Playable *playable) {}

    //void add(Playlist *playList) {}

    void add(Playable *file, position_t position) {}

    void remove() {}

    void remove(position_t position) {}

    void setMode(Mode *mode) {}

    void play() {};
};


class Player {
public:
    Playable *openFile(File file) {
        return nullptr;
    }

    Playlist *createPlaylist(const char *name) {
        return nullptr;
    }
};

#endif //LIB_PLAYLIST_H
