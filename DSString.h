#ifndef DSSTRING_H
#define DSSTRING_H

#include <iostream>
#include <cstring>

class DSString{
    private:
        char *data;

        size_t len;

    public:
        static const size_t npos = -1; // declares the npos variable and defines its value as -1.

        DSString(){ // declares and defines the default constructor for the DSString class.
            data = nullptr;
            len = 0;
        }

        DSString(const char *); // declares a constructor used to convert a cstring.

        DSString(const DSString &other) : len(other.len) { // declares and defines a copy constructor for the DSString class.
            data = new char[len + 1];
            strcpy(data, other.data);
        }

        DSString &operator=(const DSString &); // declares an overload function for the = operator. 

        ~DSString(); // declares the DSString class destructor.

        bool empty() const; // declares the empty function to return a bool value.

        DSString trim() const; // declares the trim function to return an object of the DSString class.

        size_t find_first_not_of(char c) const; // declares the find_first_not_of function.

        size_t find_last_not_of(char c) const; // declares the find_last_not_of function.

        size_t length() const; // declares the length function.

        char &operator[](size_t); // declares an overload function for the [] operator.

        DSString operator+(const DSString &) const; // declares an overload function for the + operator.

        bool operator==(const DSString &) const; // decalres an overload function for the == operator.

        bool operator<(const DSString &) const; // declares an overload function for the < operator. 

        DSString substring(size_t start, size_t numChars) const; // declares the substring function for the DSString class.

        DSString toLower() const;  // declares the toLower function to return an object of the DSString class.

        const char *c_str() const; // declares the c_str function

        friend std::ostream &operator<<(std::ostream &, const DSString &); // declares an overload function for the << operator.
};

#endif
