// Copyright (c) 2022-2023 MrShaaban, Mohamad Shaaban, https://github.com/sha3sha3/UE-EasyKafka.

#pragma once
#include "CoreMinimal.h"
#include "RecordHeader.generated.h"

USTRUCT(BlueprintType, meta = (ShortToolTip = "Kafka producer record Header"))
struct FRecordHeader
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EasyKafka|Producer", meta = (ShortToolTip = "List of key value int headers"))
		TMap<FString,int> IntHeaders;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EasyKafka|Producer", meta = (ShortToolTip = "List of key value string headers"))
		TMap<FString,FString> StrHeaders;

	FRecordHeader()
	{
		IntHeaders = TMap<FString, int>();
		StrHeaders = TMap<FString, FString>();
	}
	FRecordHeader(TMap<FString, FString> strHeaders)
	{
		IntHeaders = TMap<FString, int>();
		StrHeaders = strHeaders;
	}
};