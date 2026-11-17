#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GPPDemoUIController.generated.h"

class UUserWidget;
class UGPPDemoHUDViewModel;
class UGPPDemoRunSubsystem;
class USoundMix;
class USoundClass;

enum class EGPPDemoScreen : uint8 { None, Main, Pause, Options, Controls };

// Existing T07 progress drives HUD; real UMG buttons call these operational commands.
UCLASS(Blueprintable)
class AGPPDemoUIController : public APlayerController
{
    GENERATED_BODY()

public:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    virtual void SetupInputComponent() override;

    UFUNCTION(BlueprintPure, Category="Demo|UI")
    UGPPDemoHUDViewModel* GetHUDViewModel() const { return HUDModel; }

    UFUNCTION(BlueprintCallable, Category="Demo|Game")
    bool StartNewGame();

    UFUNCTION(BlueprintCallable, Category="Demo|Game")
    bool ReturnToGame();

    UFUNCTION(BlueprintCallable, Category="Demo|Game")
    void TogglePause();

    UFUNCTION(BlueprintCallable, Category="Demo|UI")
    bool ShowOptions();

    UFUNCTION(BlueprintCallable, Category="Demo|UI")
    bool ShowControls();

    UFUNCTION(BlueprintCallable, Category="Demo|UI")
    bool BackToPreviousMenu();

    UFUNCTION(BlueprintCallable, Category="Demo|Save")
    bool RequestSave();

    UFUNCTION(BlueprintCallable, Category="Demo|Settings")
    bool SetGraphicsQuality(int32 Level);

    UFUNCTION(BlueprintCallable, Category="Demo|Settings")
    bool SetVSyncEnabled(bool bEnabled);

    UFUNCTION(BlueprintCallable, Category="Demo|Settings")
    bool SetMasterVolume(float Volume);

    UFUNCTION(BlueprintCallable, Category="Demo|Settings")
    bool SetSubtitlesEnabled(bool bEnabled);

    UFUNCTION(BlueprintPure, Category="Demo|Controls")
    TArray<FText> GetActiveControlBindings() const;

    UFUNCTION(BlueprintCallable, Category="Demo|Game")
    void QuitGame();

protected:
    UPROPERTY(EditDefaultsOnly, Category="Demo|Widgets")
    TSubclassOf<UUserWidget> HUDWidgetClass;
    UPROPERTY(EditDefaultsOnly, Category="Demo|Widgets")
    TSubclassOf<UUserWidget> MainMenuWidgetClass;
    UPROPERTY(EditDefaultsOnly, Category="Demo|Widgets")
    TSubclassOf<UUserWidget> PauseWidgetClass;
    UPROPERTY(EditDefaultsOnly, Category="Demo|Widgets")
    TSubclassOf<UUserWidget> OptionsWidgetClass;
    UPROPERTY(EditDefaultsOnly, Category="Demo|Widgets")
    TSubclassOf<UUserWidget> ControlsWidgetClass;
    UPROPERTY(EditDefaultsOnly, Category="Demo|Widgets")
    TSubclassOf<UUserWidget> SavingOverlayClass;

    UPROPERTY(EditDefaultsOnly, Category="Demo|Audio")
    TObjectPtr<USoundMix> MasterSoundMix;
    UPROPERTY(EditDefaultsOnly, Category="Demo|Audio")
    TObjectPtr<USoundClass> MasterSoundClass;

private:
    UPROPERTY(Transient)
    TObjectPtr<UGPPDemoHUDViewModel> HUDModel;
    UPROPERTY(Transient)
    TObjectPtr<UGPPDemoRunSubsystem> RunSubsystem;
    UPROPERTY(Transient)
    TObjectPtr<UUserWidget> HUDWidget;
    UPROPERTY(Transient)
    TObjectPtr<UUserWidget> ScreenWidget;
    UPROPERTY(Transient)
    TObjectPtr<UUserWidget> SavingWidget;

    EGPPDemoScreen Screen = EGPPDemoScreen::None;
    EGPPDemoScreen ReturnScreen = EGPPDemoScreen::Main;
    bool bSoundMixActivated = false;

    UFUNCTION()
    void HandleProgressChanged();
    UFUNCTION()
    void HandleSaveCompleted(bool bSucceeded);

    bool ShowScreen(EGPPDemoScreen Next, TSubclassOf<UUserWidget> Class);
    bool ShowMainMenu();
    bool ShowPauseMenu();
    void HideMenus();
    bool ApplyMasterVolume(float Volume, bool bPersist);
};
