// Copyright (c) 2022-2023 MrShaaban, Mohamad Shaaban, https://github.com/sha3sha3/UE-EasyKafka.

#pragma once
#include "CoreMinimal.h"
#include "RecordMetadata.h"
#include "ProducerCallback.generated.h"

USTRUCT(BlueprintType, meta = (ShortToolTip = "Consumer Error"))
struct FProducerCallback
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EasyKafka|Producer", meta = (ShortToolTip = "Error message string"))
		    FString ErrorMessage;
	    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EasyKafka|Producer", meta = (ShortToolTip = "is there an error?"))
			bool bError;
	    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EasyKafka|Producer", meta = (ShortToolTip = "Record metadata"))
			FRecordMetadata RecordMetadata;

	FProducerCallback()
	{
		ErrorMessage = "";
		bError = false;
	}

	FProducerCallback(bool error, FString errorMessage, FRecordMetadata recordMetadata)
	{
		bError = error;
		ErrorMessage = errorMessage;
		RecordMetadata = recordMetadata;
	}
};