#ifndef PLAYLIST_FILE_H
#define PLAYLIST_FILE_H

#define SEPARATOR '|'
#define SEPARATOR2 ':'

class File {
private:
    std::string type;
    std::string content;
    std::unordered_map<std::string, std::string> metadata;

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

        static const std::regex content("(\\d|\\s|[A-Za-z]|\\.|\\,|!|\\?|'|:|;|-)*");

        if (!regex_match(str, content))
            throw CorruptContentException();

        this->content = str;
    }

    //Zwraca wartość wskazanej metadanej.
    std::string getValue(const std::string &name) const {
        auto result = metadata.find(name);
        if (result != metadata.end())
            return result->second;
        else
            throw PlayableException();
    }

    std::string getType() const {
        return this->type;
    }

    std::string getContent() const {
        return this->content;
    }
};

#endif //PLAYLIST_FILE_H
