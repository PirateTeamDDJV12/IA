#ifndef PLANIFICATEUR_HEADER
#define PLANIFICATEUR_HEADER
#include "Mission.h"

class Planificateur
{
    void create(Mission::MissionType mType, unsigned int from, unsigned int to);
};

#endif // PLANIFICATEUR_HEADER