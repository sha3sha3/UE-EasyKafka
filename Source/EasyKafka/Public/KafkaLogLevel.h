// Copyright (c) 2022-2023 MrShaaban, Mohamad Shaaban, https://github.com/sha3sha3/UE-EasyKafka.
#include "KafkaLogLevel.generated.h"
#pragma once
UENUM(BlueprintType, meta = (ToolTip = "Kafka Log Level"))
enum class EKafkaLogLevel : uint8
{
	EMERG=0 UMETA(ToolTip = "EMERG"),
	ALERT=1 UMETA(ToolTip = "ALERT"),
	CRIT=2 UMETA(ToolTip = "CRIT"),
	ERR=3 UMETA(ToolTip = "ERR"),
	WARNING=4 UMETA(ToolTip = "WARNING"),
	NOTICE=5 UMETA(ToolTip = "NOTICE"),
	INFO=6 UMETA(ToolTip = "INFO"),
	DEBUG=7 UMETA(ToolTip = "DEBUG")
};