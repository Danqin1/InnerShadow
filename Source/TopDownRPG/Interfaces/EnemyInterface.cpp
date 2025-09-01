// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyInterface.h"


// Add default functionality here for any IEnemyInterface functions that are not pure virtual.

ECharacterState IEnemyInterface::GetState()
{
	return ECharacterState::Nothing;
}

void IEnemyInterface::SetState(ECharacterState NewState, bool force)
{
}

void IEnemyInterface::ClearState(ECharacterState State)
{
}
