#ifndef LIB_PLAYLIST_H
#define LIB_PLAYLIST_H

#include <random>
#include <unordered_map>
#include <utility>
#include <deque>
#include <algorithm>
#include <regex>

using position_t = uint32_t;
#define SEPARATOR '|'
#define SEPARATOR2 ':'

class PlayerException : public std::exception {};

class YearIsNotNumberException : public PlayerException {
public:
    [[nodiscard]] const char *what() const noexcept override {
        return "year is not number";
    }
};

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
    [[nodiscard]] const char *what() const noexcept override {
        return "unsupported type";
    }
};

class File {
private:
    std::string type;
    std::string content;
    std::unordered_map<std::string, std::string> metadata;
    const char* file;

    bool isNumber(std::string str) {
        for (auto i = str.begin(); i != str.end(); ++i)
            if (!std::isdigit(*i))
                return false;

        return !str.empty();
    }

public:
    // Wyodrębnia kolejne informacje z treści pliku.
    // Rzuca wyjątki, gdy odkryje, że plik lub jego metadane są uszkodzone.
    explicit File(const char *file) {
        this->file = file;

        std::string str(file);
        std::string toMap;
        size_t pos = str.find(SEPARATOR);
        if (pos == std::string::npos)
            throw CorruptFileException();
        toMap = str.substr(0, pos);
        this->type = toMap;
        str = str.substr(pos + 1, std::string::npos);

        pos = str.find(SEPARATOR);
        std::string key, value;
        while (pos != std::string::npos) {
            toMap = str.substr(0, pos);
            str = str.substr(pos + 1, std::string::npos);
            pos = toMap.find(SEPARATOR2);
            if (pos == std::string::npos)
                throw CorruptMetadataException();

            key = toMap.substr(0, pos);
            value = toMap.substr(pos + 1, std::string::npos);
            if (key == "year" && !isNumber(value))
                throw YearIsNotNumberException();
            metadata.insert({key, value});

            pos = str.find(SEPARATOR);
        }

        static const std::regex content("(\\d|\\s|[A-Za-z]|\\.|\\,|!|\\?|'|:|;|-)+");

        if (!regex_match(str, content))
            throw CorruptContentException();

        this->content = str;
    }

    std::string getValue(const std::string &name) const {
        auto result = metadata.find(name);
        if (result != metadata.end())
            return result->second;
        else
            return std::string();
    }

    std::string getType() const {
        return this->type;
    }

    std::string getContent() const {
        return this->content;
    }

    /*const char *getFile() {
        return file;
    }*/
};

class Printer {
    const static int CODE = 13;

    static std::string uncodingROT13 (std::string str) {
        std::string res;
        for (char & i : str) {
            if ((i >= 'a' && i <= 'm') || (i >= 'A' && i <='M'))
                res.append(1,i += CODE);
            else if ((i >= 'n' && i <= 'z') || (i >= 'N' && i <= 'Z'))
                res.append(1, i -= CODE);
            else
                res.append(1, i);
        }

        return res;
    }

public:
    static void printMovie(std::string type, std::string metadata,
            std::string &content) {

        std::cout << type << metadata << uncodingROT13(content) << "\n";
    }

    static void printSong(std::string type, std::string metadata) {
        std::cout << type << metadata << "\n";
    }

    static void printPlaylist(std::string type, std::string name) {
        std::cout << type << " [" << name << "]\n";
    }
};


class Playable {
public:
    virtual void play() = 0;
};

class Song : virtual public Playable {
private:
    std::string artist;
    std::string title;
    std::string content;
public:

    explicit Song(File *file) try : artist(file->getValue("artist")), title(file->getValue("title")),
                            content(file->getContent()) {}
                            catch (std::exception &e) {throw PlayableException();}

    void play() override {
        Printer::printSong("Song", " [" + artist + ", " +
            title + "]: " + content);
    }
};

class Movie : virtual public Playable {
private:
    std::string title;
    std::string year;
    std::string content;

public:
    explicit Movie(File *file) try : title(file->getValue("title")), year(file->getValue("year")),
                            content(file->getContent()) {}
                            catch (std::exception &e) {throw PlayableException();}

    void play() override {
        Printer::printMovie("Movie", " [" + title + ", " +
                  year + "]: ", content);
    }
};

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

        for (unsigned int i : seq) {
            deque[i]->play();
        }
    }
};

class SequenceMode : public Mode {
public:
    void play(const std::deque<std::shared_ptr<Playable>> &deque) override {
        for (const auto& it : deque) {
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


std::shared_ptr<SequenceMode> createSequenceMode() {
    return std::make_shared<SequenceMode>();
}

std::shared_ptr<ShuffleMode> createShuffleMode(unsigned long seed) {
    return std::make_shared<ShuffleMode>(seed);
}

std::shared_ptr<OddEvenMode> createOddEvenMode() {
    return std::make_shared<OddEvenMode>();
}

class Playlist : public Playable {
private:
    std::string name;
    std::deque<std::shared_ptr<Playable>> playlist;
    std::shared_ptr<Mode> mode;

public:
    explicit Playlist(const char* name) : name(name), mode(createSequenceMode()) {}

    void add(const std::shared_ptr<Playable> &playable) {
        playlist.push_back(playable);
    }

    void add(const std::shared_ptr<Playable> &file, position_t position) {
        if (position > playlist.size())
            throw OutOfRangePositionException();
        playlist.insert(playlist.begin() + position, file);
    }

    void remove() {
        playlist.pop_back();
    }

    void remove(position_t position) {
        if (playlist.empty() || position > playlist.size() - 1)
            throw OutOfRangePositionException();
        playlist.erase(playlist.begin() + position);
    }

    void setMode(const std::shared_ptr<Mode> &play_mode) {
        this->mode = play_mode;
    }

    void play() override {
        Printer::printPlaylist("Playlist", name);
        mode->play(playlist);
    };
};

class Player {
public:
    static std::shared_ptr<Playable> openFile(File file) {

        std::string type = file.getType();
        if (type == "audio") {
            std::shared_ptr<Song> song = std::make_shared<Song>(&file);
            return song;
        }
        else if (type == "video") {
            std::shared_ptr<Movie> movie = std::make_shared<Movie>(&file);
            return movie;
        }
        else {
            throw UnsupportedTypeException();
        }
    }

    static std::shared_ptr<Playlist> createPlaylist(const char *name) {
        std::shared_ptr<Playlist> playlist = std::make_shared<Playlist>(name);
        return playlist;
    }
};

#endif //LIB_PLAYLIST_H
