// Copyright (c) 2022-2023 MrShaaban, Mohamad Shaaban, https://github.com/sha3sha3/UE-EasyKafka.
#pragma once
#include "CoreMinimal.h"
#include "TopicPartitionOffset.generated.h"

USTRUCT(BlueprintType, meta = (ShortToolTip = "Topic Partition pair"))
struct FTopicPartitionOffset
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EasyKafka|Admin", meta = (ShortToolTip = "Topic string"))
		FString Topic;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EasyKafka|Admin", meta = (ShortToolTip = "Partition"))
		int Partition;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EasyKafka|Admin", meta = (ShortToolTip = "Offset"))
		int Offset;

	FTopicPartitionOffset(FString topic, int partition, int offset)
	{
		Topic = topic;
		Partition = partition;
		Offset = offset;
	}
	FTopicPartitionOffset()
	{
		Topic = "";
		Partition = 0;
		Offset = 0;
	}

};