// Copyright (c) 2022-2023 MrShaaban, Mohamad Shaaban, https://github.com/sha3sha3/UE-EasyKafka.

#pragma once
UENUM(BlueprintType, meta=(ToolTip="Kafka Client Configurations"))
enum class EKafkaProducerConfig : uint8
{
	BOOTSTRAP_SERVERS UMETA(ToolTip = "host:port pairs of brokers splitted ,."),
	CLIENT_ID UMETA(ToolTip = "messages sent from the client ID."),
	ACKS UMETA(ToolTip = "how many partition replicas must receive the record before the producer can consider the write successful."),
	QUEUE_BUFFERING_MAX_MESSAGES UMETA(ToolTip = "Maximum number of messages allowed on the producer queue."),
	QUEUE_BUFFERING_MAX_KBYTES UMETA(ToolTip = "Maximum total message size sum allowed on the producer queue."),
	LINGER_MS UMETA(ToolTip = "Delay in milliseconds to wait for messages in the producer queue, to accumulate before constructing messages batches to transmit to brokers."),
	BATCH_NUM_MESSAGES UMETA(ToolTip = "Maximum number of messages batched in one messageSet. The total MessageSet size is also limited by MESSAGE_MAX_BYTES."),
	BATCH_SIZE UMETA(ToolTip = "Maximum size (in bytes) of all messages batched in one MessageSet (including protocol framing overhead)."),
	MESSAGE_MAX_BYTES UMETA(ToolTip = "Maximum Kafka protocol request message size."),
	MESSAGE_TIMEOUT_MS UMETA(ToolTip = "limits the time a produced message waits for successful delivery."),
	REQUEST_TIMEOUT_MS UMETA(ToolTip = "This value is only enforced by the brokers and relies on `ACKS` being non-zero."),
	PARTITIONER UMETA(ToolTip = "The default partitioner for a ProducerRecord (with no partition assigned)."),
	MAX_IN_FLIGHT UMETA(ToolTip = "Maximum number of in-flight requests per broker connection."),
	ENABLE_IDEMPOTENCE UMETA(ToolTip = "When set to `true`, the producer will ensure that messages are succefully sent exactly once and in the original order."),
	TRANSACTIONAL_ID UMETA(ToolTip = "It's used to identify the same transactional producer instance across process restarts."),
	TRANSACTION_TIMEOUT_MS UMETA(ToolTip = "Th maximus amount of time in milliseconds that the transaction coordinator will wait for a trnsaction status update from the producer before proactively ablrting the ongoing transaction."),
	SECURITY_PROTOCOL UMETA(ToolTip = "Protocol used to communicate with brokers."),
	SASL_KERBEROS_KINIT_CMD UMETA(ToolTip = "Shell command to refresh or acquire the client's Kerberos ticket."),
	SASL_KERBEROS_SERVICE_NAME UMETA(ToolTip = " The client's Kerberos principal name.")
};