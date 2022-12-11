// Copyright (c) 2022-2023 MrShaaban, Mohamad Shaaban, https://github.com/sha3sha3/UE-EasyKafka.

#pragma once
#include "CoreMinimal.h"
#include "RecordMetadata.generated.h"

USTRUCT(BlueprintType, meta = (ShortToolTip = "Kafka producer record Metadata"))
struct FRecordMetadata
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EasyKafka|Producer")
		FString Topic;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EasyKafka|Producer")
		int32 Partition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EasyKafka|Producer")
		int64 Offset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EasyKafka|Producer")
		int64 RecordId;

	FRecordMetadata()
	{
		Topic = "";
		Partition = -1;
		Offset = -1;
		RecordId = -1;
	}
	FRecordMetadata(FString topic, int32 partition, int64 offset, int64 recordId)
	{
		Topic = topic;
		Partition = partition;
		Offset = offset;
		RecordId = recordId;
	}
};