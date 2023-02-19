// Copyright (c) 2022-2023 MrShaaban, Mohamad Shaaban, https://github.com/sha3sha3/UE-EasyKafka.
#include "KafkaConsumerConfig.generated.h"
#pragma once
UENUM(BlueprintType, meta=(ToolTip="Kafka Client Configurations"))
enum class EKafkaConsumerConfig : uint8
{
	BOOTSTRAP_SERVERS UMETA(ToolTip = "host:port pairs of brokers splitted ,."),
	GROUP_ID UMETA(ToolTip = "Group identifier."),
	CLIENT_ID UMETA(ToolTip = "Client identifier."),
	ENABLE_AUTO_COMMIT UMETA(ToolTip = "Automatically commits previously polled offsets on each `poll` operation."),
	AUTO_OFFSET_RESET UMETA(ToolTip = "Available options: latest, earliest."),
	ENABLE_PARTITION_EOF UMETA(ToolTip = "Emit RD_KAFKA_RESP_ERR_PARTITION_EOF event whenever the consumer reaches the end of a partition."),
	MAX_POLL_RECORDS UMETA(ToolTip = "maximum number of records poll() will return."),
	QUEUED_MIN_MESSAGES UMETA(ToolTip = "Minimum number of messages per topic/partition tries to maintain in the local consumer queue."),
	SESSION_TIMEOUT_MS UMETA(ToolTip = "Client group session and failure detection timeout."),
	SOCKET_TIMEOUT_MS UMETA(ToolTip = "Timeout for network requests."),
	ISOLATION_LEVEL UMETA(ToolTip = "Control how to read messages written transactionally."),
	PARTITION_ASSIGNMENT_STRATEGY UMETA(ToolTip = "The name of one or more partition assignment strategies."),
	SECURITY_PROTOCOL UMETA(ToolTip = "Protocol used to communicate with brokers."),
	SASL_KERBEROS_KINIT_CMD UMETA(ToolTip = "Shell command to refresh or acquire the client's Kerberos ticket."),
	SASL_KERBEROS_SERVICE_NAME UMETA(ToolTip = "The client's Kerberos principal name.")
};