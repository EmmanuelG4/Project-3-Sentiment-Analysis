#ifndef DSSTRING_H
#define DSSTRING_H

#include <iostream>

class DSString{
    private:
        char *data;

        size_t len;

    public:
        DSString();

        DSString(const DSString &);

        DSString &operator=(const DSString &);

        ~DSString();

        size_t length() const;

        char &operator[](size_t);

        DSString operator+(const DSString &) const;

        DSString operator==(const DSString &) const;

        DSString operator<(const DSString &) const;

        DSString substring(size_t start, size_t numChars) const;

        DSString toLower() const;

        const char *c_str() const;

        friend std::ostream &operator<<(std::ostream &, const DSString &);
};

#endif
