// Copyright (c) 2022-2023 MrShaaban, Mohamad Shaaban, https://github.com/sha3sha3/UE-EasyKafka.

#pragma once

#include "Modules/ModuleManager.h"
#include "KafkaAdminConfig.h"
#include "TopicConfig.h"
#include "ListTopicsResult.h"
#include "AdminRequestResult.h"
#include "TopicPartitionOffset.h"
#include <string>

THIRD_PARTY_INCLUDES_START
#include "kafka/AdminClient.h"
THIRD_PARTY_INCLUDES_END

#define DEFAULT_COMMAND_TIMEOUT_MS 3000

using namespace kafka::clients;

DECLARE_LOG_CATEGORY_EXTERN(LogKafkaAdmin, Log, All);

/** Kafka Producer singletone module.*/
class KAFKAADMIN_API FKafkaAdminModule : public IModuleInterface
{
public:

	
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	/**
	* Get Kafka Admin singleton.
	*
	* @return FKafkaConsumerModule singleton
	*/
	static FKafkaAdminModule& Get();

	/**
	* Create a kafka Admin.
	*
	* @param Servers Container whose values are summed.
	* @param UserName authorization user/key.
	* @param Password authorization password.
	* @param Configuration List of EKafkaAdminConfig,String pair.
	* @param KafkaLogLevel Int 7 to 0
	* @warning Call it once only.
	*/
	void CreateAdmin(FString Servers, FString UserName, FString Password, TMap<EKafkaAdminConfig, FString> Configuration, int KafkaLogLevel = 5);

	/**
	* Create a kafka Admin.
	*
	* @param Servers Container whose values are summed.
	* @param UserName authorization user/key.
	* @param Password authorization password.
	* @param Configuration List of String,String pair.
	* @param KafkaLogLevel Int 7 to 0
	*/
	void CreateAdmin(FString Servers, FString UserName, FString Password, TMap<FString, FString> Configuration = {}, int KafkaLogLevel = 5);

	/**
	* Create batch of topics.
	*
	* @param Topics Array of topics.
	* @param NumPartitions number of partitions.
	* @param ReplicationFactor replication factor.
	* @param Configuration List of ETopicConfig,String pair.
	* @param  Timeout timeout, default 3000ms.
	* @warning Blocking.
	* @warning Configuration verified on server side.
	*/
	FAdminRequestResult CreateTopics(const TArray<FString> Topics, int NumPartitions,int  ReplicationFactor, TMap<ETopicConfig, FString> Configuration, int Timeout = DEFAULT_COMMAND_TIMEOUT_MS);
	
	/**
	* Delete batch of topics.
	*
	* @param Topics Array of topics.
	* @param  Timeout timeout, default 3000ms.
	* @warning Blocking.
	*/
	FAdminRequestResult DeleteTopics(const TArray<FString> Topics, int Timeout = DEFAULT_COMMAND_TIMEOUT_MS);

	/**
	* Delete batch of records.
	*
	* @TopicPartitionOffsets list of  FTopicPartitionOffset structure.
	* @param  Timeout timeout, default 3000ms.
	* @warning Blocking.
	*/
	FAdminRequestResult DeleteRecords(const TArray<FTopicPartitionOffset> TopicPartitionOffsets, int Timeout = DEFAULT_COMMAND_TIMEOUT_MS);
	
	/**
	* List topics.
	*
	* @param  Timeout timeout, default 3000ms.
	* @warning Blocking.
	*/
	FListTopicsResult ListTopics(int Timeout = DEFAULT_COMMAND_TIMEOUT_MS);

private:
	static FKafkaAdminModule* AdminSingleton;
	const FString GetKafkaConfigString(EKafkaAdminConfig KafkaConfig);
	const FString GetTopicConfigString(ETopicConfig KafkaConfig);

	kafka::Properties* KafkaAdminProps = nullptr;
	kafka::clients::AdminClient* KafkaAdminClient = nullptr;
	

};
