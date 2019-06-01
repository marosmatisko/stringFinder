#include "console.hpp"
#include "filesystem.hpp"


StringFinder::FileSystem::FileSystem(const std::string t_system_path) {
    m_system_path = fs::u8path(t_system_path);
    if (pathIsValid()) {
        Console::printDebugInfo({"Path correct!"});
        traversePath();
    } else {
        throw std::invalid_argument("Received path which does not exist or is unavailable!");
    }
}

StringFinder::FileSystem::~FileSystem() {}

bool StringFinder::FileSystem::pathIsValid(void) const {
    return fs::exists(m_system_path);
}

void StringFinder::FileSystem::traversePath(void) {
    m_traverse_complete = false;
    processPath(m_system_path);
    m_traverse_complete = true;
}

void StringFinder::FileSystem::processDirectory(const fs::path& t_directory_path) {
    Console::printDebugInfo({"Processing folder: ", t_directory_path.u8string()});
    for (const auto& entry : fs::directory_iterator(t_directory_path)) {
        processPath(entry.path());
    }
}

void StringFinder::FileSystem::processFile(const fs::path& t_file_path) {
    try{
        auto file = std::make_shared<File>(t_file_path.u8string());
        m_files.push(file);
    } catch(const std::bad_alloc& e) {
        throw std::runtime_error(e.what());
    }
    Console::printDebugInfo({"File found! Adding to processing file: ", t_file_path});
}

void StringFinder::FileSystem::processPath(const fs::path& t_path) {
    if (fs::is_directory(t_path)) {
        processDirectory(t_path);
    } else if (fs::is_regular_file(t_path)) {
        processFile(t_path);
    }
}

bool StringFinder::FileSystem::traversalComplete(void) const {
    return m_traverse_complete;
}

StringFinder::FileQueue& StringFinder::FileSystem::getFiles(void) {
    return m_files;
}
