#ifndef BTDEBUGSTRING_H_INCLUDED
#define BTDEBUGSTRING_H_INCLUDED


#include "../BaseBloc.h"

#include <string>


namespace BehaviourTree
{
    /*Return the type parsed into a std string. Here " ??? "*/
    template<general::type type>
    std::string typeToStdString() { return " ??? "; }

    /*return "Bloc Composite: "*/
    template<>
    std::string typeToStdString<general::type::COMPOSITE>() { return "Bloc Composite: "; }

    /*return "Bloc Decorator: "*/
    template<>
    std::string typeToStdString<general::type::DECORATOR>() { return "Bloc Decorator: "; }

    /*return "Bloc Action: "*/
    template<>
    std::string typeToStdString<general::type::ACTION>() { return "Bloc Action: "; }


    /*Return the type parsed into a std string. If the type isn't known, return " ??? "*/
    std::string typeToStdString(general::type type)
    {
        switch (type)
        {
        case general::type::COMPOSITE:
            return typeToStdString<general::type::COMPOSITE>();

        case general::type::DECORATOR:
            return typeToStdString<general::type::DECORATOR>();

        case general::type::ACTION:
            return typeToStdString<general::type::ACTION>();

        default:
            return " ??? ";
        }
    }


    /*
    Output into a string every bloc you pass by parameter. 

    It's the toStdString methods of each Blocs that are called and concatened together
    in the same order you passed in the arguments
    */
    template<class Bloc = BaseBloc*, class ... Blocs>
    std::string outputBTToStdString(Blocs ... arguments)
    {
        std::string result;

        Bloc array[] = { arguments... };

        for (auto iter = std::begin(array); iter != std::end(array); ++iter)
        {
            if (*iter)
            {
                result += (*iter)->toStdString();
            }
        }

        return result;
    }
}

#endif //!BTDEBUGSTRING_H_INCLUDED