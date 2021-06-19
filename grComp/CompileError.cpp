#include "CompileError.h"

namespace fs = std::filesystem;

std::string grComp::Error::CError::to_string() const {
    return 
        "Compile Error ["+ grComp::Error::CompileStepToString(compile_step) +"] occurred on line " + std::to_string(line) +
        " in file " + file.filename().string() + " ( "+ fs::absolute(file).string() +" ) " +
        "\nError Message: \n" + err_message +
        (hint_message.length() > 0 ? 
            ("\nHint message: \n" + hint_message)
            : ""
        );
}


std::string grComp::Error::CompileStepToString(grComp::Error::CompileStep cs){
    using namespace grComp::Error;
    switch (cs)
    {
    case CompileStep::generator: return "generator"; 
    case CompileStep::lexer: return "lexer"; 
    case CompileStep::parser: return "parser"; 
    case CompileStep::reader: return "reader";
    default: return "[corrupted compile step value]";
    }
}