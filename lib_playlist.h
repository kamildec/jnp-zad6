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

class Song : public File {
private:
    std::string artist;
    std::string title;

public:
    void play() {}
};

class Movie : public File {
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


class PlayList {
public:
    void add(File *file) {}

    void add(PlayList *playList) {}

    void add(File *file, position_t position) {}

    void remove() {}

    void remove(position_t position) {}

    void setMode(Mode *mode) {}

    void play() {};
};


class Player {
public:
    File *openFile(File file) {
        return nullptr;
    }

    PlayList *createPlaylist(const char *name) {
        return nullptr;
    }
};

#endif //LIB_PLAYLIST_H
