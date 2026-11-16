#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "GPPDemoHUDViewModel.generated.h"

class UGPPDemoRunSubsystem;

// Read-only UI projection from the existing T07 progression authority.
UCLASS(BlueprintType)
class UGPPDemoHUDViewModel final : public UMVVMViewModelBase
{
    GENERATED_BODY()

public:
    void RefreshFromRun(const UGPPDemoRunSubsystem* Run);
    void ReportSaveState(const FText& Text);

private:
    UPROPERTY(BlueprintReadOnly, FieldNotify, meta=(AllowPrivateAccess="true"))
    FText LightState;

    UPROPERTY(BlueprintReadOnly, FieldNotify, meta=(AllowPrivateAccess="true"))
    FText Objective;

    UPROPERTY(BlueprintReadOnly, FieldNotify, meta=(AllowPrivateAccess="true"))
    FText SaveStatus;
};
