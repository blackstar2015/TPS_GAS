// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/AttributeInfo.h"

FTPSAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{
	for(const FTPSAttributeInfo& Info: AttributeInformation)
	{
		if(Info.AttributeTag == AttributeTag)
		{
			return Info;
		}
	}
	if(bLogNotFound)
	{
		UE_LOG(LogTemp,Error, TEXT("Cant find AttributeTag [%s] on attributeInfo [%s]"),
			*AttributeTag.ToString(),*GetNameSafe(this))
	}
	return FTPSAttributeInfo();
}
