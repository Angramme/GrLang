#pragma once

#include <vector>
#include <string>
#include <filesystem>

#include "tl/expected.hpp"

namespace grComp{
    namespace Error{
        enum class CompileStep {
            reader,
            lexer,
            parser,
            generator,
        };
        struct CError
		{
			CompileStep compile_step;
			unsigned long long line;
			// std::vector<std::string> inside; // in function main in function f2 in method class1::f3 ...
            std::filesystem::path file;
			std::string err_message;
			std::string hint_message;

            std::string to_string() const;
            inline static std::string to_string(const CError& err) { return err.to_string(); };
		};

        template <typename T>
        using ex = tl::expected<T, CError>;

        using unex = tl::unexpected<CError>;

        std::string CompileStepToString(grComp::Error::CompileStep cs);

        // compile_error ParseErr(const std::string& msg, ){

        // }
    }
}