// Copyright (c) 2022-2023 MrShaaban, Mohamad Shaaban, https://github.com/sha3sha3/UE-EasyKafka.

#pragma once

#include "CoreMinimal.h"
#include "RecordHeader.h"
#include "ProducerRecord.generated.h"

USTRUCT(BlueprintType, meta = (ShortToolTip = "Kafka Consumer Record"))
struct FProducerRecord
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EasyKafka|Producer", meta = (ShortToolTip = "Topic to push to"))
		FString Topic;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EasyKafka|Producer", meta = (ShortToolTip = "Partition to push to"))
		int32 Partition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EasyKafka|Producer", meta = (ShortToolTip = "Record headers"))
		FRecordHeader Headers = FRecordHeader();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EasyKafka|Producer", meta = (ShortToolTip = "Record Key"))
		FString Key;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EasyKafka|Producer", meta = (ShortToolTip = "Record Value"))
		FString Value;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EasyKafka|Producer", meta = (ShortToolTip = "Unique record id to identify this record on callback"))
		int64 Id;

	FProducerRecord()
	{
		Topic = "";
		Partition = -1;
		Key = "";
		Value = "";
		Id = -1;
	}
};