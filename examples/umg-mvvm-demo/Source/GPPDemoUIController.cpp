#include "GPPDemoUIController.h"

#include "GPPDemoHUDViewModel.h"
#include "GPPDemoRunSubsystem.h"
#include "GPPLightCharacter.h"

#include "Blueprint/UserWidget.h"
#include "Components/InputComponent.h"
#include "EnhancedActionKeyMapping.h"
#include "EnhancedPlayerInput.h"
#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "GameFramework/GameUserSettings.h"
#include "GameFramework/Pawn.h"
#include "InputAction.h"
#include "InputCoreTypes.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Misc/ConfigCacheIni.h"
#include "Sound/SoundClass.h"
#include "Sound/SoundMix.h"

DEFINE_LOG_CATEGORY_STATIC(LogGPPDemoUI, Log, All);

namespace GPPDemoUIConfig
{
    static const TCHAR* Section = TEXT("GameProductionPlaybook.UserPreferences");
    static const TCHAR* VolumeKey = TEXT("MasterVolume");
    static const TCHAR* SubtitlesKey = TEXT("Subtitles");
}

void AGPPDemoUIController::BeginPlay()
{
    Super::BeginPlay();

    HUDModel = NewObject<UGPPDemoHUDViewModel>(this);
    if (UGameInstance* GI = GetGameInstance())
    {
        RunSubsystem = GI->GetSubsystem<UGPPDemoRunSubsystem>();
    }
    if (!RunSubsystem)
    {
        UE_LOG(LogGPPDemoUI, Error, TEXT("Real T07 RunSubsystem is required."));
        return;
    }

    RunSubsystem->OnProgressChanged.AddDynamic(this, &AGPPDemoUIController::HandleProgressChanged);
    RunSubsystem->OnSaveCompleted.AddDynamic(this, &AGPPDemoUIController::HandleSaveCompleted);
    HUDModel->RefreshFromRun(RunSubsystem);

    if (HUDWidgetClass)
    {
        HUDWidget = CreateWidget<UUserWidget>(this, HUDWidgetClass);
        if (HUDWidget)
        {
            HUDWidget->AddToViewport(0);
        }
    }
    else
    {
        UE_LOG(LogGPPDemoUI, Error, TEXT("Missing actual HUD Widget Blueprint."));
    }

    float Volume = 1.0f;
    bool bSubtitles = true;
    if (GConfig)
    {
        GConfig->GetFloat(GPPDemoUIConfig::Section, GPPDemoUIConfig::VolumeKey,
                          Volume, GGameUserSettingsIni);
        GConfig->GetBool(GPPDemoUIConfig::Section, GPPDemoUIConfig::SubtitlesKey,
                         bSubtitles, GGameUserSettingsIni);
    }
    ApplyMasterVolume(Volume, false);
    UGameplayStatics::SetSubtitlesEnabled(bSubtitles);

    if (!RunSubsystem->HasActiveRun())
    {
        ShowMainMenu();
    }
}

void AGPPDemoUIController::EndPlay(const EEndPlayReason::Type Reason)
{
    if (RunSubsystem)
    {
        RunSubsystem->OnProgressChanged.RemoveDynamic(this, &AGPPDemoUIController::HandleProgressChanged);
        RunSubsystem->OnSaveCompleted.RemoveDynamic(this, &AGPPDemoUIController::HandleSaveCompleted);
    }
    Super::EndPlay(Reason);
}

void AGPPDemoUIController::SetupInputComponent()
{
    Super::SetupInputComponent();
    if (InputComponent)
    {
        FInputKeyBinding& Escape =
            InputComponent->BindKey(EKeys::Escape, IE_Pressed, this, &AGPPDemoUIController::TogglePause);
        Escape.bExecuteWhenPaused = true;
    }
}

void AGPPDemoUIController::HandleProgressChanged()
{
    if (HUDModel)
    {
        HUDModel->RefreshFromRun(RunSubsystem);
    }
}

void AGPPDemoUIController::HandleSaveCompleted(bool bSucceeded)
{
    if (HUDModel)
    {
        HUDModel->ReportSaveState(FText::FromString(
            bSucceeded ? TEXT("Save verified") : TEXT("Save failed — inspect previous slot")));
    }
    if (SavingWidget)
    {
        SavingWidget->RemoveFromParent();
        SavingWidget = nullptr;
    }
}

bool AGPPDemoUIController::ShowScreen(EGPPDemoScreen Next, TSubclassOf<UUserWidget> Class)
{
    if (!Class)
    {
        UE_LOG(LogGPPDemoUI, Error, TEXT("Screen %d requires a real assigned Widget Blueprint."),
            static_cast<int32>(Next));
        return false;
    }

    UUserWidget* NewScreen = CreateWidget<UUserWidget>(this, Class);
    if (!NewScreen)
    {
        return false;
    }

    if (ScreenWidget)
    {
        ScreenWidget->RemoveFromParent();
    }
    ScreenWidget = NewScreen;
    Screen = Next;
    NewScreen->AddToViewport(20);

    if (AGPPLightCharacter* Character = Cast<AGPPLightCharacter>(GetPawn()))
    {
        Character->ResetSprint();
    }

    UGameplayStatics::SetGamePaused(this, true);
    FInputModeUIOnly InputMode;
    InputMode.SetWidgetToFocus(NewScreen->TakeWidget());
    SetInputMode(InputMode);
    bShowMouseCursor = true;
    return true;
}

bool AGPPDemoUIController::ShowMainMenu()
{
    return ShowScreen(EGPPDemoScreen::Main, MainMenuWidgetClass);
}

bool AGPPDemoUIController::ShowPauseMenu()
{
    return RunSubsystem && RunSubsystem->HasActiveRun()
        ? ShowScreen(EGPPDemoScreen::Pause, PauseWidgetClass) : false;
}

void AGPPDemoUIController::HideMenus()
{
    if (ScreenWidget)
    {
        ScreenWidget->RemoveFromParent();
        ScreenWidget = nullptr;
    }
    Screen = EGPPDemoScreen::None;
    UGameplayStatics::SetGamePaused(this, false);
    SetInputMode(FInputModeGameOnly());
    bShowMouseCursor = false;
}

bool AGPPDemoUIController::StartNewGame()
{
    if (Screen != EGPPDemoScreen::Main || !RunSubsystem || RunSubsystem->HasActiveRun()
        || !GetWorld() || !GetPawn())
    {
        return false;
    }

    // Start at the actual possessed pawn's position in the currently loaded level.
    if (!RunSubsystem->StartNewRun(GetWorld()->GetFName(),
                                   FName(TEXT("GPP_Start")), GetPawn()->GetActorLocation()))
    {
        return false;
    }

    HideMenus();
    return true;
}

bool AGPPDemoUIController::ReturnToGame()
{
    if (!RunSubsystem || !RunSubsystem->HasActiveRun())
    {
        return false;
    }
    HideMenus();
    return true;
}

void AGPPDemoUIController::TogglePause()
{
    if (!RunSubsystem || !RunSubsystem->HasActiveRun())
    {
        return;
    }

    if (Screen == EGPPDemoScreen::None)
    {
        ShowPauseMenu();
    }
    else if (Screen == EGPPDemoScreen::Pause)
    {
        ReturnToGame();
    }
}

bool AGPPDemoUIController::ShowOptions()
{
    ReturnScreen = RunSubsystem && RunSubsystem->HasActiveRun()
        ? EGPPDemoScreen::Pause : EGPPDemoScreen::Main;
    return ShowScreen(EGPPDemoScreen::Options, OptionsWidgetClass);
}

bool AGPPDemoUIController::ShowControls()
{
    ReturnScreen = RunSubsystem && RunSubsystem->HasActiveRun()
        ? EGPPDemoScreen::Pause : EGPPDemoScreen::Main;
    return ShowScreen(EGPPDemoScreen::Controls, ControlsWidgetClass);
}

bool AGPPDemoUIController::BackToPreviousMenu()
{
    return ReturnScreen == EGPPDemoScreen::Pause ? ShowPauseMenu() : ShowMainMenu();
}

bool AGPPDemoUIController::RequestSave()
{
    if (!RunSubsystem || !RunSubsystem->HasActiveRun())
    {
        return false;
    }
    if (!RunSubsystem->RequestAsyncSave())
    {
        if (HUDModel)
        {
            HUDModel->ReportSaveState(FText::FromString(TEXT("Save rejected")));
        }
        return false;
    }

    if (HUDModel)
    {
        HUDModel->ReportSaveState(FText::FromString(TEXT("Saving...")));
    }

    if (SavingOverlayClass && !SavingWidget)
    {
        SavingWidget = CreateWidget<UUserWidget>(this, SavingOverlayClass);
        if (SavingWidget)
        {
            SavingWidget->AddToViewport(100);
        }
    }
    return true;
}

bool AGPPDemoUIController::SetGraphicsQuality(int32 Level)
{
    if (!GEngine || Level < 0 || Level > 4)
    {
        return false;
    }
    UGameUserSettings* S = GEngine->GetGameUserSettings();
    if (!S) { return false; }
    S->SetOverallScalabilityLevel(Level);
    S->ApplySettings(false);
    S->SaveSettings();
    return true;
}

bool AGPPDemoUIController::SetVSyncEnabled(bool bEnabled)
{
    if (!GEngine) { return false; }
    UGameUserSettings* S = GEngine->GetGameUserSettings();
    if (!S) { return false; }
    S->SetVSyncEnabled(bEnabled);
    S->ApplySettings(false);
    S->SaveSettings();
    return true;
}

bool AGPPDemoUIController::ApplyMasterVolume(float Volume, bool bPersist)
{
    if (!MasterSoundMix || !MasterSoundClass || !FMath::IsFinite(Volume))
    {
        UE_LOG(LogGPPDemoUI, Error, TEXT("Real SoundMix and SoundClass must be assigned."));
        return false;
    }
    const float Clamped = FMath::Clamp(Volume, 0.0f, 1.0f);
    // A slider may call this function repeatedly. Push the mix once per
    // controller lifetime instead of accumulating repeated modifier pushes.
    if (!bSoundMixActivated)
    {
        UGameplayStatics::PushSoundMixModifier(this, MasterSoundMix);
        bSoundMixActivated = true;
    }
    UGameplayStatics::SetSoundMixClassOverride(
        this, MasterSoundMix, MasterSoundClass, Clamped, 1.0f, 0.1f, true);

    if (bPersist && GConfig)
    {
        GConfig->SetFloat(GPPDemoUIConfig::Section, GPPDemoUIConfig::VolumeKey,
                          Clamped, GGameUserSettingsIni);
        GConfig->Flush(false, GGameUserSettingsIni);
    }
    return true;
}

bool AGPPDemoUIController::SetMasterVolume(float Volume)
{
    return ApplyMasterVolume(Volume, true);
}

bool AGPPDemoUIController::SetSubtitlesEnabled(bool bEnabled)
{
    UGameplayStatics::SetSubtitlesEnabled(bEnabled);
    if (GConfig)
    {
        GConfig->SetBool(GPPDemoUIConfig::Section, GPPDemoUIConfig::SubtitlesKey,
                         bEnabled, GGameUserSettingsIni);
        GConfig->Flush(false, GGameUserSettingsIni);
    }
    return true;
}

TArray<FText> AGPPDemoUIController::GetActiveControlBindings() const
{
    TArray<FText> Result;
    const UEnhancedPlayerInput* Input = Cast<UEnhancedPlayerInput>(PlayerInput);
    if (!Input) { return Result; }

    for (const FEnhancedActionKeyMapping& Mapping : Input->GetEnhancedActionMappingsView())
    {
        if (Mapping.Action)
        {
            Result.Add(FText::FromString(
                Mapping.Action->GetName() + TEXT(": ") + Mapping.Key.GetDisplayName().ToString()));
        }
    }
    return Result;
}

void AGPPDemoUIController::QuitGame()
{
    UKismetSystemLibrary::QuitGame(this, this, EQuitPreference::Quit, false);
}
