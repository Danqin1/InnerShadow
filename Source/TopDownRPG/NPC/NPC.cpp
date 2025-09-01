// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC.h"

#include "TopDownRPG/DevDebug.h"
#include "TopDownRPG/Player/RPGCharacter.h"
#include "TopDownRPG/QuestSystem/QuestSystem.h"
#include "TopDownRPG/UI/Interaction/UW_Interact.h"


// Sets default values
ANPC::ANPC()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InteractionWidget = CreateDefaultSubobject<UWidgetComponent>("InteractionWidget");
	InteractionWidget->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ANPC::BeginPlay()
{
	Super::BeginPlay();

	if(auto* UWInteract = Cast<UUW_Interact>(InteractionWidget->GetWidget()))
	{
		UWInteract->SetInteractionText("Get Quest");
	}
	if(InteractionWidget)
	{
		InteractionWidget->SetVisibility(false);
	}

	if (UQuestSystem* QuestSystem = GetGameInstance()->GetSubsystem<UQuestSystem>())
	{
		Quests = QuestSystem->GetQuestsForNPC(NPCID);
	}
	else
	{
		DevDebug::OnScreenLog("QuestSystem is not found in NPC: " + GetName(), FColor::Red, 5.f);
	}
}

void ANPC::Interact(ACharacter* Character)
{
	if(auto* RPGCharacter = Cast<ARPGCharacter>(Character))
	{
		InteractCharacter = RPGCharacter;
		InteractCharacter->SetState(Interaction);
		if (UIClass)
		{
			UIWidget = CreateWidget<UUW_NPC>(GetWorld(), UIClass);
			UIWidget->OnClose.AddDynamic(this, &ANPC::InteractionFinished);
			UIWidget->AddToViewport();
			UIWidget->Populate(Quests);
		}
		else
		{
			DevDebug::OnScreenLog("UIClass is not set for NPC: " + GetName(), FColor::Red, 5.f);
		}
	}
}

void ANPC::InteractionFinished()
{
	if(InteractCharacter)
	{
		InteractCharacter->ClearState(Interaction);
	}
	if(UIWidget)
	{
		UIWidget->OnClose.RemoveDynamic(this, &ANPC::InteractionFinished);
		UIWidget->RemoveFromParent();
		UIWidget->Destruct();
		UIWidget = nullptr;
	}
}

void ANPC::SetAsTarget(bool isTargeted)
{
	if(InteractionWidget)
	{
		InteractionWidget->SetVisibility(isTargeted);
	}
}

