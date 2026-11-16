#include "GPPDemoHUDViewModel.h"

#include "GPPDemoRunSubsystem.h"

void UGPPDemoHUDViewModel::RefreshFromRun(const UGPPDemoRunSubsystem* Run)
{
    if (!Run || !Run->HasActiveRun())
    {
        UE_MVVM_SET_PROPERTY_VALUE(LightState, FText::FromString(TEXT("No active run")));
        UE_MVVM_SET_PROPERTY_VALUE(Objective, FText::FromString(TEXT("Start a new game")));
        return;
    }

    switch (Run->GetLightStage())
    {
        case EGPPDemoLightStage::Owned:
            UE_MVVM_SET_PROPERTY_VALUE(LightState, FText::FromString(TEXT("Personal Light: available")));
            UE_MVVM_SET_PROPERTY_VALUE(Objective, FText::FromString(TEXT("Explore and restore the beacon")));
            break;
        case EGPPDemoLightStage::Donated:
            UE_MVVM_SET_PROPERTY_VALUE(LightState, FText::FromString(TEXT("Personal Light: donated")));
            UE_MVVM_SET_PROPERTY_VALUE(Objective, FText::FromString(TEXT("Find the separate recovery fragment")));
            break;
        case EGPPDemoLightStage::Recovered:
            UE_MVVM_SET_PROPERTY_VALUE(LightState, FText::FromString(TEXT("Personal Light: recovered")));
            UE_MVVM_SET_PROPERTY_VALUE(Objective, FText::FromString(TEXT("Continue exploring; beacon remains powered")));
            break;
    }
}

void UGPPDemoHUDViewModel::ReportSaveState(const FText& Text)
{
    UE_MVVM_SET_PROPERTY_VALUE(SaveStatus, Text);
}
