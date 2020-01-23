#ifndef __LIB_PLAYLIST_H__
#define __LIB_PLAYLIST_H__

#include <iostream>
#include <random>
#include <unordered_map>
#include <utility>
#include <deque>
#include <algorithm>
#include <regex>
#include "player_exception.h"
#include "playable.h"
#include "mode.h"

//Funkcje tworzące kolejne tryby odtwarzania.
std::shared_ptr<SequenceMode> createSequenceMode();

std::shared_ptr<ShuffleMode> createShuffleMode(unsigned long seed);

std::shared_ptr<OddEvenMode> createOddEvenMode();

class Playlist : public Playable {
private:
    std::string name;
    std::deque<std::shared_ptr<Playable>> playlist;
    std::shared_ptr<Mode> mode;
    unsigned int id;

public:
    //Ostatnie nadane id.
    static unsigned int lastId;

    explicit Playlist(const char* name) : name(name), mode(createSequenceMode()), id(lastId++) {}

    void add(const std::shared_ptr<Playable> &playable) {
        if (playable->canBeAdded(id))
            playlist.push_back(playable);
        else
            throw CycleException();
    }

    void add(const std::shared_ptr<Playable> &playable, position_t position) {
        if (position > playlist.size())
            throw OutOfRangePositionException();
        if (playable->canBeAdded(id))
            playlist.insert(playlist.begin() + position, playable);
        else
            throw CycleException();
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
        ManagePlay::printPlaylist("Playlist", name);
        mode->play(playlist);
    };

    //Sprawdza DFS-em, czy dodanie playlisty nie spowoduje powstania cyklu.
    bool canBeAdded(unsigned int id) override {
        if (id == this->id)
            return false;

        for (const auto &it : playlist)
            if (!it->canBeAdded(id))
                return false;

        return true;
    }
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

#endif //__LIB_PLAYLIST_H__
