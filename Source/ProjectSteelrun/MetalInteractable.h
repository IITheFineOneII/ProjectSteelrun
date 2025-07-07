// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MetalInteractable.generated.h"


UINTERFACE(MinimalAPI)
class UMetalInteractable : public UInterface
{
	GENERATED_BODY()
};
class PROJECTSTEELRUN_API IMetalInteractable
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Metal")
    bool IsMetal() const;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Metal")
    bool IsTarget() const;
};
