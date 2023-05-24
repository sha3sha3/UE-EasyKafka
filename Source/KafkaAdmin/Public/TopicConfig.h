// Copyright (c) 2022-2023 MrShaaban, Mohamad Shaaban, https://github.com/sha3sha3/UE-EasyKafka.
/*
https://kafka.apache.org/documentation.html#topicconfigs
*/
#include "TopicConfig.generated.h"
#pragma once
UENUM(BlueprintType, meta = (ToolTip = "Topic Configurations"))
enum class ETopicConfig : uint8
{
	CLEANUP_POLICY UMETA(ToolTip = "valid values [compact, delete]"),
	COMPRESSION_TYPE UMETA(ToolTip = "valid values [uncompressed, zstd, lz4, snappy, gzip, producer]."),
	DELETE_RETENTION_MS UMETA(ToolTip = "The amount of time to retain delete tombstone markers for log compacted topics."),
	FILE_DELETE_DELAY_MS UMETA(ToolTip = "The time to wait before deleting a file from the filesystem."),
	FLUSH_MESSAGES UMETA(ToolTip = "This setting allows specifying an interval at which we will force an fsync of data written to the log."),
	FLUSH_MS UMETA(ToolTip = "This setting allows specifying a time interval at which we will force an fsync of data written to the log."),
	FOLLOWER_REPLICATION_THROTTLED_REPLICAS UMETA(ToolTip = "A list of replicas for which log replication should be throttled on the follower side. e.g [partitionId]:[brokerId],[partitionId]:[brokerId],..."),
	INDEX_INTERVAL_BYTES UMETA(ToolTip = "This setting controls how frequently Kafka adds an index entry to its offset index."),
	LEADER_REPLICATION_THROTTLED_REPLICAS UMETA(ToolTip = "A list of replicas for which log replication should be throttled on the leader side. e.g [partitionId]:[brokerId],[partitionId]:[brokerId],..."),
	MAX_COMPACTION_LAG_MS UMETA(ToolTip = "The maximum time a message will remain ineligible for compaction in the log."),
	MAX_MESSAGE_BYTES UMETA(ToolTip = "The largest record batch size allowed by Kafka (after compression if compression is enabled)."),
	MESSAGE_TIMESTAMP_DIFFERENCE_MAX_MS UMETA(ToolTip = "The maximum difference allowed between the timestamp when a broker receives a message and the timestamp specified in the message."),
	MESSAGE_TIMESTAMP_TYPE UMETA(ToolTip = "Define whether the timestamp in the message is message create time or log append time."),
	MIN_CLEANABLE_DIRTY_RATIO UMETA(ToolTip = "This configuration controls how frequently the log compactor will attempt to clean the log."),
	MIN_COMPACTION_LAG_MS UMETA(ToolTip = "Define whether the timestamp in the message is message create time or log append time."),
	MIN_INSYNC_REPLICAS UMETA(ToolTip = "When a producer sets acks to all or - 1, this configuration specifies the minimum number of replicas that must acknowledge a write for the write to be considered successful."),
	PREALLOCATE UMETA(ToolTip = "True if we should preallocate the file on disk when creating a new log segment."),
	RETENTION_BYTES UMETA(ToolTip = "This configuration controls the maximum size a partition (which consists of log segments) can grow to before we will discard old log segments to free up space if we are using the delete retention policy."),
	RETENTION_MS UMETA(ToolTip = "This configuration controls the maximum time we will retain a log before we will discard old log segments to free up space if we are using the delete retention policy."),
	SEGMENT_BYTES UMETA(ToolTip = "This configuration controls the segment file size for the log."),
	SEGMENT_INDEX_BYTES UMETA(ToolTip = "This configuration controls the size of the index that maps offsets to file positions."),
	SEGMENT_JITTER_MS UMETA(ToolTip = "The maximum random jitter subtracted from the scheduled segment roll time to avoid thundering herds of segment rolling."),
	SEGMENT_MS UMETA(ToolTip = "This configuration controls the period of time after which Kafka will force the log to roll even if the segment file isn't full to ensure that retention can delete or compact old data."),
	UNCLEAN_LEADER_ELECTION_ENABLE UMETA(ToolTip = "Indicates whether to enable replicas not in the ISR set to be elected as leader as a last resort, even though doing so may result in data loss."),
	MESSAGE_DOWNCONVERSION_ENABLE UMETA(ToolTip = "This configuration controls whether down-conversion of message formats is enabled to satisfy consume requests. When set to false, broker will not perform down-conversion for consumers expecting an older message format.")
};