// Copyright (c) 2022-2023 MrShaaban, Mohamad Shaaban, https://github.com/sha3sha3/UE-EasyKafka.
#pragma once
#include "CoreMinimal.h"
#include "ConsumerCallback.generated.h"

USTRUCT(BlueprintType, meta = (ShortToolTip = "Consumer Error"))
struct FConsumerCallback
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EasyKafka|Consumer", meta = (ShortToolTip = "Error message string"))
		FString ErrorMessage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EasyKafka|Consumer", meta = (ShortToolTip = "is there an error?"))
		bool bError;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EasyKafka|Consumer", meta = (ShortToolTip = "Offsets"))
		TMap<FString, int64> TopicPartitionOffsets;

	FConsumerCallback()
	{
		ErrorMessage = "";
		bError = false;
	}
};