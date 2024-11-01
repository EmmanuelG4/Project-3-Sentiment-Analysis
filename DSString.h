#ifndef DSSTRING_H
#define DSSTRING_H

#include <iostream>
#include <cstring>

class DSString{
    private:
        char *data;

        size_t len;

    public:
        static const size_t npos = -1;

        DSString(){
            data = nullptr;
            len = 0;
        }

        DSString(const char *);

        DSString(const DSString &other) : len(other.len) {
            data = new char[len + 1];
            std::strcpy(data, other.data);
        }

        DSString &operator=(const DSString &);

        ~DSString();

        size_t length() const;

        char &operator[](size_t);

        DSString operator+(const DSString &) const;

        bool operator==(const DSString &) const;

        bool operator<(const DSString &) const;

        DSString substring(size_t start, size_t numChars) const;

        DSString toLower() const;

        const char *c_str() const;

        friend std::ostream &operator<<(std::ostream &, const DSString &);
};

#endif
