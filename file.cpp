#include <cstring>
#include <iostream>
#include <memory>
#include <string>
#include <sstream>
#include <iterator>
#include <vector>

#include "file.hpp"

File::File(const std::string t_filePath) {
    size_t position = t_filePath.length() - 1;
    while (t_filePath.at(position) != '/') {
        --position;
        if(position == 0) { break; }
    }
    m_fileName = t_filePath.substr(position + 1, t_filePath.length() - position - 1);
    m_filePath = t_filePath;
    open();
}

File::~File() {
    try {
        m_fileStream.close();
    } catch(const std::ifstream::failure& e) {
        throw std::runtime_error("Exception while closing file " + m_filePath + " : " + e.what() + "\n");
    }
    m_filePath.clear();
    m_fileName.clear();
}

void File::open() {
    try {
        m_fileStream.open(m_filePath);
    } catch(const std::ifstream::failure& e) {
        throw std::runtime_error("Exception while opening file " + m_filePath + " : " + e.what() + "\n");
    }
}

std::string& File::getFileName(void) {
    return m_fileName;
}

char File::getNextChar(void) {
    char value = EOF;
    if (!m_fileStream.eof()) {
        try {
            m_fileStream.get(value);
        } catch(const std::ifstream::failure& e) {
            throw std::runtime_error("Exception while reading file " + m_filePath + " : " + e.what() + "\n");
        }
    }
    return value;
}

bool File::isEof(void) {
    return !m_fileStream.good() || m_fileStream.eof(); 
}
