// Copyright (c) 2022-2023 MrShaaban, Mohamad Shaaban, https://github.com/sha3sha3/UE-EasyKafka.
#include "KafkaAdminConfig.generated.h"
#pragma once
UENUM(BlueprintType, meta = (ToolTip = "Kafka Admin Configurations"))
enum class EKafkaAdminConfig : uint8
{
	BOOTSTRAP_SERVERS UMETA(ToolTip = "host:port pairs of brokers splitted ,."),
	SOCKET_TIMEOUT_MS UMETA(ToolTip = "Timeout for network requests."),
	SECURITY_PROTOCOL UMETA(ToolTip = "Protocol used to communicate with brokers."),
	SASL_KERBEROS_KINIT_CMD UMETA(ToolTip = "Shell command to refresh or acquire the client's Kerberos ticket."),
	SASL_KERBEROS_SERVICE_NAME UMETA(ToolTip = "The client's Kerberos principal name."),
	ENABLE_SSL_CERTIFICATE_VERIFICATION UMETA(ToolTip = " Enable SSL certificate verification of the broker."),
	LOG_LEVEL UMETA(ToolTip = " Rdkafka log level.")
};