// Copyright (c) 2022-2023 MrShaaban, Mohamad Shaaban, https://github.com/sha3sha3/UE-EasyKafka.
#pragma once

#include "CoreMinimal.h"
#include "ConsumerRecord.generated.h"

USTRUCT(BlueprintType, meta = (ShortToolTip = "Kafka Consumer Record"))
struct FConsumerRecord
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EasyKafka|Consumer", meta = (ShortToolTip = "Topic from where the record consumed."))
		FString Topic;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EasyKafka|Consumer", meta = (ShortToolTip = "Partition from where the record consumed."))
		int32 Partition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EasyKafka|Consumer", meta = (ShortToolTip = "Record offset."))
		int64 Offset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EasyKafka|Consumer", meta = (ShortToolTip = "Consuming timestamp."))
		FDateTime Timestamp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EasyKafka|Consumer", meta = (ShortToolTip = "Record headers."))
		TMap<FString, FString> Headers = {};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EasyKafka|Consumer", meta = (ShortToolTip = "Record key."))
		FString Key;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EasyKafka|Consumer", meta = (ShortToolTip = "Record value."))
		FString Value;

	FConsumerRecord()
	{
		Topic = "";
		Partition = -1;
		Offset = -1;
		Timestamp = FDateTime::Now();
		Key = "";
		Value = "";
	}
};