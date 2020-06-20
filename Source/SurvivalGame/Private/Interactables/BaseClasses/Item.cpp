// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/BaseClasses/Item.h"
#include "Character/SCharacter.h"

UItem::UItem()
{

}

bool UItem::OnUseItem(ASCharacter* UsingCharacter)
{
	return false;
}

bool UItem::BP_UseItem(ASCharacter* UsingCharacter)
{
	return OnUseItem(UsingCharacter);
}
