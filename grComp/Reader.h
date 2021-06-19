#pragma once

#include <iostream>
#include <istream>
#include <string>
#include <filesystem>
#include <memory>

namespace fs = std::filesystem;

namespace grComp{
    class Reader{
        private:
        std::unique_ptr<std::istream> data;
        fs::path filepath;

        public:
        Reader(fs::path filepath);
        Reader(std::unique_ptr<std::istream> stream);

        inline int get() { return data->get(); }
        inline fs::path get_filename() { return filepath; }
        inline unsigned long long get_line() { return 0; }
    };
}