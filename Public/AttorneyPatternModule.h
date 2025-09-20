#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

/**
 * A set of macro helpers to use attorney pattern without
 * much of a boilerplate code.
 */
class FAttorneyPatternModuleModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};