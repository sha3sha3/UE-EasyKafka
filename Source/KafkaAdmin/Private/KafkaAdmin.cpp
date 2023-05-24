// Copyright (c) 2022-2023 MrShaaban, Mohamad Shaaban, https://github.com/sha3sha3/UE-EasyKafka.

#include "KafkaAdmin.h"
#include <list>


DEFINE_LOG_CATEGORY(LogKafkaAdmin);
#define LOCTEXT_NAMESPACE "FKafkaAdminModule"

FKafkaAdminModule* FKafkaAdminModule::AdminSingleton = nullptr;

void FKafkaAdminModule::StartupModule()
{
	UE_LOG(LogKafkaAdmin, Display, TEXT("KafkaAdmin Startup"));
	if (!AdminSingleton)
		FKafkaAdminModule::AdminSingleton = this;

}

void FKafkaAdminModule::ShutdownModule()
{
	UE_LOG(LogKafkaAdmin, Display, TEXT("KafkaAdmin Shutdown"));
	if (KafkaAdminClient)
	{
		delete KafkaAdminClient;
		KafkaAdminClient = nullptr;
	}
	if (KafkaAdminProps)
	{
		delete KafkaAdminProps;
		KafkaAdminProps = nullptr;
	}
	
}

FKafkaAdminModule& FKafkaAdminModule::Get()
{
	if (AdminSingleton == nullptr)
	{
		check(IsInGameThread());
		FModuleManager::LoadModuleChecked<FKafkaAdminModule>("FKafkaAdminModule");
	}
	check(AdminSingleton);
	return *AdminSingleton;
}

void FKafkaAdminModule::CreateAdmin(FString Servers, FString UserName, FString Password, TMap<EKafkaAdminConfig, FString> Configuration, int KafkaLogLevel)
{
	TMap<FString, FString> Configurations;

	for (const TPair<EKafkaAdminConfig, FString>& pair : Configuration)
	{
		Configurations.Add(GetKafkaConfigString(pair.Key), pair.Value);
	}

	CreateAdmin(Servers, UserName, Password, Configurations, KafkaLogLevel);
}

void FKafkaAdminModule::CreateAdmin(FString Servers, FString UserName, FString Password, TMap<FString, FString> Configuration, int KafkaLogLevel)
{
	if (KafkaAdminProps)
		delete KafkaAdminProps;
	if (KafkaAdminClient)
	{
		delete KafkaAdminClient;
	}

	KafkaAdminProps = new kafka::Properties({
		   {"bootstrap.servers",  std::string(TCHAR_TO_UTF8(*Servers))},
		   {"log_level",  std::to_string(KafkaLogLevel)}
		});
	/*
	Create Admin with no usr/passwd
	*/
	if (!UserName.Equals("") && !Password.Equals(""))
	{
		KafkaAdminProps->put("security.protocol", "SASL_SSL");
		KafkaAdminProps->put("sasl.mechanisms", "PLAIN");
		KafkaAdminProps->put("sasl.username", std::string(TCHAR_TO_UTF8(*UserName)));
		KafkaAdminProps->put("sasl.password", std::string(TCHAR_TO_UTF8(*Password)));
	}

	for (const TPair<FString, FString>& pair : Configuration)
	{
		KafkaAdminProps->put(std::string(TCHAR_TO_UTF8(*pair.Key)), std::string(TCHAR_TO_UTF8(*pair.Value)));
	}

	KafkaAdminClient = new kafka::clients::AdminClient(*KafkaAdminProps);
	UE_LOG(LogKafkaAdmin, Display, TEXT("Admin Created"));
}

FAdminRequestResult FKafkaAdminModule::CreateTopics(const TArray<FString> Topics, int NumPartitions, int ReplicationFactor, TMap<ETopicConfig, FString> Configurations, int Timeout)
{
	if (!KafkaAdminClient)
		return FAdminRequestResult("Admin client not initialized",true);

	std::set<std::string> topics;
	for (const FString& Topic : Topics)
		topics.insert(std::string(TCHAR_TO_UTF8(*Topic)));

	kafka::Properties *TopicsConfiguration = new kafka::Properties();
	for (const TPair<ETopicConfig, FString>& Configuration : Configurations)
		TopicsConfiguration->put(std::string(TCHAR_TO_UTF8(*GetTopicConfigString(Configuration.Key))), std::string(TCHAR_TO_UTF8(*Configuration.Value)));

	auto result = KafkaAdminClient->createTopics(topics, NumPartitions, ReplicationFactor, *TopicsConfiguration, std::chrono::milliseconds(Timeout));

	delete TopicsConfiguration;

	
	if (result.error)
		return FAdminRequestResult(UTF8_TO_TCHAR(result.error.message().c_str()), true);
	
	return FAdminRequestResult();
}

FAdminRequestResult FKafkaAdminModule::DeleteTopics(const TArray<FString> Topics, int Timeout)
{
	if (!KafkaAdminClient)
		return FAdminRequestResult("Admin client not initialized", true);

	std::set<std::string> topics;
	for (const FString& Topic : Topics)
		topics.insert(std::string(TCHAR_TO_UTF8(*Topic)));

	auto result = KafkaAdminClient->deleteTopics(topics, std::chrono::milliseconds(Timeout));

	if (result.error)
		return FAdminRequestResult(UTF8_TO_TCHAR(result.error.message().c_str()), true);

	return FAdminRequestResult();
}

FAdminRequestResult FKafkaAdminModule::DeleteRecords(const TArray<FTopicPartitionOffset> TopicPartitionOffsets, int Timeout)
{
	if (!KafkaAdminClient)
		return FAdminRequestResult("Admin client not initialized", true);

	std::map<kafka::TopicPartition, kafka::Offset> topicPartitionOffsets;
	for (const FTopicPartitionOffset& TopicPartitionOffset : TopicPartitionOffsets)
		topicPartitionOffsets.insert(std::pair(std::pair(std::string(TCHAR_TO_UTF8(*TopicPartitionOffset.Topic)), TopicPartitionOffset.Partition), TopicPartitionOffset.Offset));

	auto result = KafkaAdminClient->deleteRecords(topicPartitionOffsets, std::chrono::milliseconds(Timeout));

	if (result.error)
		return FAdminRequestResult(UTF8_TO_TCHAR(result.error.message().c_str()), true);

	return FAdminRequestResult();
}

FListTopicsResult FKafkaAdminModule::ListTopics(int Timeout)
{
	if (!KafkaAdminClient)
		return FListTopicsResult("Admin client not initialized",true);

	auto result = KafkaAdminClient->listTopics(std::chrono::milliseconds(Timeout));

	if (result.error)
		return FListTopicsResult(UTF8_TO_TCHAR(result.error.message().c_str()), true);

	TArray<FString> Topics;
	for (const std::string& Topic : result.topics)
		Topics.Add(UTF8_TO_TCHAR(Topic.c_str()));

	return FListTopicsResult(Topics);
}

const FString FKafkaAdminModule::GetKafkaConfigString(EKafkaAdminConfig KafkaConfig)
{
	switch (KafkaConfig)
	{
	case    EKafkaAdminConfig::BOOTSTRAP_SERVERS:
		return "bootstrap.servers";
	case	EKafkaAdminConfig::SOCKET_TIMEOUT_MS:
		return "socket.timeout.ms";
	case	EKafkaAdminConfig::SECURITY_PROTOCOL:
		return "security.protocol";
	case	EKafkaAdminConfig::SASL_KERBEROS_KINIT_CMD:
		return "sasl.kerberos.kinit.cmd";
	case	EKafkaAdminConfig::SASL_KERBEROS_SERVICE_NAME:
		return "sasl.kerberos.service.name";
	case    EKafkaAdminConfig::ENABLE_SSL_CERTIFICATE_VERIFICATION:
		return "enable.ssl.certificate.verification";
	case    EKafkaAdminConfig::LOG_LEVEL:
		return "log_level";
	default:
		return "";
	}
}

const FString FKafkaAdminModule::GetTopicConfigString(ETopicConfig TopicConfig)
{
	switch (TopicConfig)
	{
	case    ETopicConfig::CLEANUP_POLICY:
		return "cleanup.policy";
	case    ETopicConfig::COMPRESSION_TYPE:
		return "compression.type";
	case    ETopicConfig::DELETE_RETENTION_MS:
		return "delete.retention.ms";
	case    ETopicConfig::FILE_DELETE_DELAY_MS:
		return "file.delete.delay.ms";
	case    ETopicConfig::FLUSH_MESSAGES:
		return "flush.messages";
	case    ETopicConfig::FLUSH_MS:
		return "flush.ms";
	case    ETopicConfig::FOLLOWER_REPLICATION_THROTTLED_REPLICAS:
		return "follower.replication.throttled.replicas";
	case    ETopicConfig::INDEX_INTERVAL_BYTES:
		return "index.interval.bytes";
	case    ETopicConfig::LEADER_REPLICATION_THROTTLED_REPLICAS:
		return "leader.replication.throttled.replicas";
	case    ETopicConfig::MAX_COMPACTION_LAG_MS:
		return "max.compaction.lag.ms";
	case    ETopicConfig::MAX_MESSAGE_BYTES:
		return "max.message.bytes";
	case    ETopicConfig::MESSAGE_TIMESTAMP_DIFFERENCE_MAX_MS:
		return "message.timestamp.difference.max.ms";
	case    ETopicConfig::MESSAGE_TIMESTAMP_TYPE:
		return "message.timestamp.type";
	case    ETopicConfig::MIN_CLEANABLE_DIRTY_RATIO:
		return "min.cleanable.dirty.ratio";
	case    ETopicConfig::MIN_COMPACTION_LAG_MS:
		return "min.compaction.lag.ms";
	case    ETopicConfig::MIN_INSYNC_REPLICAS:
		return "min.insync.replicas";
	case    ETopicConfig::PREALLOCATE:
		return "preallocate";
	case    ETopicConfig::RETENTION_BYTES:
		return "retention.bytes";
	case    ETopicConfig::RETENTION_MS:
		return "retention.ms";
	case    ETopicConfig::SEGMENT_BYTES:
		return "segment.bytes";
	case    ETopicConfig::SEGMENT_INDEX_BYTES:
		return "segment.index.bytes";
	case    ETopicConfig::SEGMENT_JITTER_MS:
		return "segment.jitter.ms";
	case    ETopicConfig::SEGMENT_MS:
		return "segment.ms";
	case    ETopicConfig::UNCLEAN_LEADER_ELECTION_ENABLE:
		return "unclean.leader.election.enable";
	case    ETopicConfig::MESSAGE_DOWNCONVERSION_ENABLE:
		return "message.downconversion.enable";
	default:
		return "";
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FKafkaAdminModule, kafkaAdmin)