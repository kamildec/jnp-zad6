#ifndef PLAYLIST_PLAYABLE_H
#define PLAYLIST_PLAYABLE_H
#include "file.h"

class Playable {
public:
    virtual bool canBeAdded(unsigned int id) {
        (void)id;
        return true;
    }
    virtual void play() = 0;
};

class ManagePlay {
    const static int CODE = 13;

    static std::string uncodingROT13 (std::string str) {
        std::string res;
        for (char &i : str) {
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
    static void printMovie(const std::string &type, const std::string &metadata,
                           std::string &content) {

        std::cout << type << metadata << uncodingROT13(content) << "\n";
    }

    static void printSong(const std::string &type, const std::string &metadata) {
        std::cout << type << metadata << "\n";
    }

    static void printPlaylist(const std::string &type, const std::string &name) {
        std::cout << type << " [" << name << "]\n";
    }
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
        ManagePlay::printSong("Song", " [" + artist + ", " +
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
        ManagePlay::printMovie("Movie", " [" + title + ", " +
                                        year + "]: ", content);
    }
};

#endif //PLAYLIST_PLAYABLE_H
