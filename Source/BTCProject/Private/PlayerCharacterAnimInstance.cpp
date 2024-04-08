// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterAnimInstance.h"
#include "PlayerCharacter.h"

void UPlayerCharacterAnimInstance::OnStateAnimationEnds()
{
	if (State == EPlayerState::Attack)
	{
		State = EPlayerState::Locomotion;
	}
	else
	{
		auto ownerActor = this->GetOwningActor();
		auto PlayerCharacter = Cast<APlayerCharacter>(ownerActor);

		if (PlayerCharacter == nullptr)
		{
			return;
		}
		if (State == EPlayerState::Hit)
		{
			if (PlayerCharacter->GetHealthPoints() > 0.0f)
			{
				State = EPlayerState::Locomotion;
			}
			else
			{
				State = EPlayerState::Die;
			}
		}
		else if (State == EPlayerState::Die)
		{
			//Call die process 
			PlayerCharacter->IsKilled();
		}
	}
}
