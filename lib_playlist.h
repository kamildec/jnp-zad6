#ifndef LIB_PLAYLIST_H
#define LIB_PLAYLIST_H

#include <bits/stdc++.h>
#include <unordered_map>
#include <utility>

using position_t = uint32_t;
#define SEPARATOR '|'
#define SEPARATOR2 ':'

class PlayerException : public std::exception {
private:
    //std::string m_error;
public:
    /*explicit PlayerException(const char *error) : m_error(error) {}

    explicit PlayerException(const std::string &error) : m_error(error) {}

    [[nodiscard]] const char *what() const noexcept override {
        return m_error.c_str();
    }*/
};

class CorruptedMetadata : public PlayerException {
public:
    [[nodiscard]] const char *what() const noexcept override {
        return "Corrupted metadata in file!";
    }
};

class PlayableException : public PlayerException {
public:
    [[nodiscard]] const char *what() const noexcept override {
        return "Object is not an instance of Playable class!";
    }
};

class File {
private:
    std::unordered_map<std::string, std::string> metadata;
    const char* file;
public:
    explicit File(const char *file) {
        this->file = file;

        std::string str(file);
        std::string toMap;
        size_t pos = str.find(SEPARATOR);
        toMap = str.substr(0, pos);
        metadata.insert({"type", toMap});
        str = str.substr(pos + 1, std::string::npos);

        pos = str.find(SEPARATOR);
        std::string key, value;
        while (pos != std::string::npos) {
            toMap = str.substr(0, pos);
            str = str.substr(pos + 1, std::string::npos);
            pos = toMap.find(SEPARATOR2);
            if (pos == std::string::npos)
                throw CorruptedMetadata();

            key = toMap.substr(0, pos);
            value = toMap.substr(pos + 1, std::string::npos);
            metadata.insert({key, value});

            pos = str.find(SEPARATOR);
        }

        metadata.insert({"content", str});
    }

    //virtual ~File() = default;

    std::string getValue(const std::string &name) const {
        auto result = metadata.find(name);
        if (result != metadata.end())
            return result->second;
        else
            return std::string();
    }

    const char *getFile() {
        return file;
    }
};

class Playable {
public:
    virtual void play() {}
};

class Song : public File, virtual public Playable {
private:
    std::string artist;
    std::string title;

public:
    Song(const char *file) : File(file) {}

    void play() override {
        std::cout << "Song" << " [" << getValue("artist") << ", " <<
            getValue("title") << "]: " << getValue("content") << std::endl;
    }
};

class Movie : public File, virtual public Playable {
private:
    std::string title;
    std::string year;

public:
    Movie(const char *file) : File(file) {}

    void play() override {
        std::cout << "Movie" << " [" << getValue("title") << ", " <<
                  getValue("year") << "]: " << getValue("content") << std::endl;
    }
};

class Mode {
public:
    virtual void play(std::deque<Playable*> deque) {}
};


class ShuflleMode : public Mode {
    std::default_random_engine engine;
    unsigned long seed;

public:
    explicit ShuflleMode(unsigned long seed) : seed(seed), engine(seed) {}

    void play(std::deque<Playable*> deque) override {
        std::uniform_int_distribution<position_t> distr(0, deque.size());
        std::vector<position_t> seq(deque.size());
        std::iota(seq.begin(), seq.end(), 0);
        std::shuffle(seq.begin(), seq.end(), engine);

        for (unsigned int i : seq) {
            deque[i]->play();
        }
    }
};

class SequenceMode : public Mode {
public:
    void play(std::deque<Playable*> deque) override {
        for (auto it : deque) {
            it->play();
        }
    }
};

class OddEvenMode : public Mode {
public:
    void play(std::deque<Playable*> deque) override {
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


SequenceMode *createSequenceMode() {
    SequenceMode *sequenceMode = new SequenceMode;
    return sequenceMode;
}

ShuflleMode *createShuffleMode(unsigned long seed) {
    ShuflleMode *shuflleMode = new ShuflleMode(seed);
    return shuflleMode;
}

OddEvenMode *createOddEvenMode() {
    OddEvenMode *oddEvenMode = new OddEvenMode();
    return oddEvenMode;
}

//TODO zagnieżdzanie playlist
class Playlist : public Playable {
private:
    std::string name;
    std::deque<Playable*> playlist;
    Mode *mode;

public:

    explicit Playlist(const char* name) : name(name), mode(createSequenceMode()) {}

    void add(Playable *playable) {
        playlist.push_back(playable);
    }

    //TODO co jak pozycja wyjebana w kosmos?
    void add(Playable *file, position_t position) {
        playlist.insert(playlist.begin() + position, file);
    }

    void remove() {
        playlist.pop_back();
    }

    void remove(position_t position) {
        playlist.erase(playlist.begin() + position);
    }

    void setMode(Mode *mode) {
        this->mode = mode;
    }

    void play() {
        mode->play(playlist);
    };
};


class Player {
public:
    Playable *openFile(File file) {

        std::string type = file.getValue("type");
        if (type == "audio") {
            Song *song = new Song(file.getFile());
            return song;
        }
        else if (type == "video") {
            Movie *movie = new Movie(file.getFile());
            return movie;
        }
        else {
            throw PlayerException();
        }
    }

    Playlist *createPlaylist(const char *name) {
        Playlist *playlist = new Playlist(name);
        return playlist;
    }
};

#endif //LIB_PLAYLIST_H
