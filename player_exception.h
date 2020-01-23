#ifndef PLAYLIST_PLAYER_EXCEPTION_H
#define PLAYLIST_PLAYER_EXCEPTION_H

//Plik zawierający wyjątki, których korzeniem jest PlayerException.
class PlayerException : public std::exception {};

class OutOfRangePositionException : public PlayerException {
public:
    [[nodiscard]] const char *what() const noexcept override {
        return "position is out of playlist's range";
    }
};

class PlayableException : public PlayerException {
public:
    [[nodiscard]] const char *what() const noexcept override {
        return "Object is not an instance of Playable class!";
    }
};

class CorruptMetadataException : public PlayerException {
public:
    [[nodiscard]] const char *what() const noexcept override {
        return "corrupt metadata";
    }
};

class CorruptContentException : public PlayerException {
public:
    [[nodiscard]] const char *what() const noexcept override {
        return "corrupt content";
    }
};

class CorruptFileException : public PlayerException {
public:
    [[nodiscard]] const char *what() const noexcept override {
        return "corrupt file";
    }
};

class UnsupportedTypeException : public PlayerException {
public:
    [[nodiscard]] const char *what() const noexcept override {
        return "unsupported type";
    }
};

class CycleException : public PlayerException {
public:
    [[nodiscard]] const char *what() const noexcept override {
        return "cycle in playlist";
    }
};

class YearIsNotNumberException : public PlayerException {
public:
    [[nodiscard]] const char *what() const noexcept override {
        return "year is not number";
    }
};

#endif //PLAYLIST_PLAYER_EXCEPTION_H
