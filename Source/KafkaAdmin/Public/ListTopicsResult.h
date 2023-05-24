// Copyright (c) 2022-2023 MrShaaban, Mohamad Shaaban, https://github.com/sha3sha3/UE-EasyKafka.
#pragma once
#include "CoreMinimal.h"
#include "ListTopicsResult.generated.h"

USTRUCT(BlueprintType, meta = (ShortToolTip = "Admin client request result"))
struct FListTopicsResult
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EasyKafka|Admin", meta = (ShortToolTip = "Error message string"))
		FString ErrorMessage;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EasyKafka|Admin", meta = (ShortToolTip = "is there an error?"))
		bool bError;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EasyKafka|Admin", meta = (ShortToolTip = "List of topics"))
		TArray<FString> Topics;

	FListTopicsResult()
	{
		ErrorMessage = "";
		bError = false;
	}

	FListTopicsResult(FString errorMessage, bool Error)
	{
		ErrorMessage = errorMessage;
		bError = Error;
	}

	FListTopicsResult(TArray<FString> &topics)
	{
		Topics = topics;
		ErrorMessage = "";
		bError = false;
	}
};