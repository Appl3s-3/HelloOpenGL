#if !defined(LUNA_DEFINES_HPP)
#define LUNA_DEFINES_HPP

#include <string>

#define LUNA_FATAL_ERROR "Luna Fatal: "
#define LUNA_FILE_ERROR "Luna File: "
#define LUNA_MEMORY_ERROR "Luna Memory: "
#define LUNA_CREATION_ERROR "Luna Creation: "
#define LUNA_BUFFER_ERROR "Luna Buffer: "
#define LUNA_FONT_ERROR "Luna Font: "
#define LUNA_DRAW_ERROR "Luna Draw: "

#define LUNA_WARNING "Luna Warning: "

#define LUNA_RANDOM_SEED 10378

namespace luna {
    namespace err {
        std::string const FATAL = "Luna Fatal: ";
        std::string const FILE = "Luna File: ";
        std::string const MEMORY = "Luna Memory: ";
        std::string const CREATION = "Luna Creation: ";
        std::string const BUFFER = "Luna Buffer: ";
        std::string const FONT = "Luna Font: ";
        std::string const DRAW = "Luna Draw: ";
    };

    size_t const RANDOM_SEED = 10378;

    enum class SourceText {
        string = 0,
        file = 1
    };
    
    enum class Alignment {
        top_left = 0,    top,    top_right,
                left, centre,        right,
         bottom_left, bottom, bottom_right
    };
};


#endif // LUNA_DEFINES_HPP
