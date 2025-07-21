#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SteelrunObjectInterface.generated.h"

UINTERFACE(MinimalAPI)
class USteelrunObjectInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECTSTEELRUN_API ISteelrunObjectInterface
{
	GENERATED_BODY()

public:
	/** Returns whether this object is metal */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Metal")
	bool IsMetal() const;

	/** Returns whether this object is currently targeted */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Metal")
	bool IsTarget() const;

	/** Sets the target state (typically toggles the internal bool) */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Metal")
	void ToggleTarget();

};