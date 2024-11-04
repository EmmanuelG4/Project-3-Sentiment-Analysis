#include <cstring>
#include <cctype>
#include "DSString.h"

// constructor for the DSString class used to convert a string into a DSString object. 
DSString::DSString(const char *str){
    if(str){
        len = strlen(str);
        data = new char[len + 1]; // allocates the memory necessary for the converted string. 
        strcpy(data, str);
    }
    else{ // If the str variable is null, the default values are used. 
        data = nullptr;
        len = 0;
    }
}

// overloads the = operator to allow for assignment/creation of a deep copy of a DSString object using the = operator. 
DSString &DSString::operator=(const DSString &other){
    if(this == &other){ // checks if the other object is being assigned to itself.
        return *this;
    }

    delete[] data; // deletes the data for the current DSString object.

    len = other.len;
    data = new char[len + 1]; // allocates the memory necessary for the new DSString object.
    strcpy(data, other.data);

    return *this;
}

// defines the destructor for the DSString class.
DSString::~DSString(){ 
    delete[] data;
}

// function used to remove any extra whitespaces at the beginning and end of a DSString object.
DSString DSString::trim() const { 
    size_t first = find_first_not_of(' ');
    if (first == npos) {
        return "";  
    }
    size_t last = find_last_not_of(' ');
    return substring(first, last - first + 1);
}

// function starts at the beginning of a DSString object and iteraters using a for loop until it finds a character that does not
// match the given variable c. 
size_t DSString::find_first_not_of(char c) const {
    for (size_t i = 0; i < length(); ++i) {
        if (c_str()[i] != c) {
            return i;
        }
    }
    return npos;
}

// function starts at the end of a DSString object and iterates backwards through the object until it finds a character that does
// match the given variable c. 
size_t DSString::find_last_not_of(char c) const {
    for (size_t i = length(); i > 0; --i) {
        if (c_str()[i - 1] != c) {
            return i - 1;
        }
    }
    return npos;
}

// boolean function that checks if a DSString object is empty. 
bool DSString::empty() const { 
    return length() == 0;
}

// returns the length of a DSString object. 
size_t DSString::length() const{ 
    return len;
}

// overloads the [] operator in order to allow for specific characters within a DSString object to be accessed.
char &DSString::operator[](size_t index){ 
    return data[index];
}

// overloads the + operator to allow for 2 DSString objects to be combined. 
DSString DSString::operator+(const DSString &other) const{
    DSString newStr;
    newStr.len = len + other.len;
    newStr.data = new char[newStr.len + 1];

    strcpy(newStr.data, data);
    strcat(newStr.data, other.data);

    return newStr;
}

// overloads the == operator to allow for comparison of two DSString objects using the == operator. 
bool DSString::operator==(const DSString &other) const{
    return strcmp(data, other.data) == 0;
}

// overloads the < operator to allow for comparison of two DSString objects using the < operator.
bool DSString::operator<(const DSString &other) const{
    return strcmp(data, other.data) < 0;
}

// DSString function that returns a substring of a DSString object.
DSString DSString::substring(size_t start, size_t numChars) const{
    if(start >= len){ // validates that the parameters are within the bounds of the DSString object.
        return DSString("");
    }

    size_t actualLength = (start + numChars > len) ? len - start : numChars;
    char *subStr = new char[actualLength + 1]; // allocates the space necessary for the substring.

    strncpy(subStr, data + start, actualLength);
    subStr[actualLength] = '\0'; // adds a null terminator at the end of the substring to make sure it is a valid string.

    DSString result(subStr);
    delete[] subStr; // deletes the temporary instance of the substring to prevent memory leaks. 

    return result;
}

// function used to create a copy of a DSString object in lowercase. 
DSString DSString::toLower() const{
    DSString lowerStr;
    lowerStr.len = len;
    lowerStr.data = new char[len + 1]; // allocates space for the lowercase string.

    for(size_t i = 0; i < len; i++){
        lowerStr.data[i] = tolower(data[i]);
    }
    lowerStr.data[len] = '\0'; // adds a null terminator at the end of the lowercase string to make sure it is a valid string.

    return lowerStr;
}

// char function that returns the C-style data value of a DSString object.
const char *DSString::c_str() const{
    return data;
}

// overloads the << operator to allow for a DSString object to be output using output streams.
std::ostream &operator<<(std::ostream &os, const DSString &str){
    os << str.data;
    return os;
}
