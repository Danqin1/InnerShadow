// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInterface.h"

#include "TopDownRPG/TopDownRPG.h"


// Add default functionality here for any IPlayerInterface functions that are not pure virtual.

ECharacterState IPlayerInterface::GetState()
{
	return Nothing;
}

void IPlayerInterface::SetState(ECharacterState NewState)
{
}

void IPlayerInterface::ClearState(ECharacterState State)
{
}