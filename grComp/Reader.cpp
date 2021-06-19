#include "Reader.h"

#include <fstream>

using namespace grComp;
namespace fs = std::filesystem;


Reader::Reader(fs::path filepath)
    : filepath(filepath), data(std::make_unique<std::ifstream>(filepath.c_str()))
{}
Reader::Reader(std::unique_ptr<std::istream> stream)
    : filepath("[from stream]"), data(std::move(stream))
{}
