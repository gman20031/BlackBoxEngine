#include <Actors/ComponentFactory.h>

#include "Card.h"

using namespace BlackBoxEngine;

static std::tuple
<
    ComponentFactory::Registrar<CardLetteringComponent>
> GameComponentRegistrar;