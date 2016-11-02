#ifndef BTDEBUGSTRING_H_INCLUDED
#define BTDEBUGSTRING_H_INCLUDED


#include "../BaseBloc.h"

#include <string>


namespace BehaviourTree
{
    template<general::type type>
    std::string typeToStdString() { return " ??? "; }

    template<>
    std::string typeToStdString<general::type::COMPOSITE>() { return "Bloc Composite: "; }

    template<>
    std::string typeToStdString<general::type::DECORATOR>() { return "Bloc Decorator: "; }

    template<>
    std::string typeToStdString<general::type::ACTION>() { return "Bloc Action: "; }



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