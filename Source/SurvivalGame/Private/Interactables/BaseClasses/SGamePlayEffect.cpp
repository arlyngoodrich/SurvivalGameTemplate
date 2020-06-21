// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/BaseClasses/SGamePlayEffect.h"

USGamePlayEffect::USGamePlayEffect()
{

}

void USGamePlayEffect::Trigger(ASCharacter* UsingCharacter)
{
	BP_OnTriggered(UsingCharacter);
}


void USGamePlayEffect::BP_Trigger(ASCharacter* UsingCharacter)
{
	Trigger(UsingCharacter);
}