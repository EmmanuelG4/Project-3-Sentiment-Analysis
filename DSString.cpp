#include <cstring>
#include <cctype>
#include "DSString.h"

DSString::DSString(const char *str){
    if(str){
        len = strlen(str);
        data = new char[len + 1];
        strcpy(data, str);
    }
    else{
        data = nullptr;
        len = 0;
    }
}

DSString &DSString::operator=(const DSString &other){
    if(this == &other){
        return *this;
    }

    delete[] data;

    len = other.len;
    data = new char[len + 1];
    strcpy(data, other.data);

    return *this;
}

DSString::~DSString(){
    delete[] data;
}

size_t DSString::length() const{
    return len;
}

char &DSString::operator[](size_t index){
    return data[index];
}

DSString DSString::operator+(const DSString &other) const{
    DSString newStr;
    newStr.len = len + other.len;
    newStr.data = new char[newStr.len + 1];

    strcpy(newStr.data, data);
    strcpy(newStr.data, other.data);

    return newStr;
}

bool DSString::operator==(const DSString &other) const{
    return strcmp(data, other.data) == 0;
}

bool DSString::operator<(const DSString &other) const{
    return strcmp(data, other.data) < 0;
}

DSString DSString::substring(size_t start, size_t numChars) const{
    if(start >= len){
        return DSString("");
    }

    size_t actualLength = (start + numChars > len) ? len - start : numChars;
    char *subStr = new char[actualLength + 1];

    strncpy(subStr, data + start, actualLength);
    subStr[actualLength] = '\0';

    DSString result(subStr);
    delete[] subStr;

    return result;
}

DSString DSString::toLower() const{
    DSString lowerStr;
    lowerStr.len = len;
    lowerStr.data = new char[len + 1];

    for(size_t i = 0; i < len; i++){
        lowerStr.data[i] = tolower(data[i]);
    }
    lowerStr.data[len] = '\0';

    return lowerStr;
}

const char *DSString::c_str() const{
    return data;
}

std::ostream &operator<<(std::ostream &os, const DSString &str){
    os << str.data;
    return os;
}
