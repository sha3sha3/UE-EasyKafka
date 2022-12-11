// Copyright (c) 2022-2023 MrShaaban, Mohamad Shaaban, https://github.com/sha3sha3/UE-EasyKafka.

#pragma once

#include "Modules/ModuleManager.h"
#include "KafkaProducerConfig.h"
#include "ProducerCallback.h"
#include "ProducerRecord.h"
#include <string>

THIRD_PARTY_INCLUDES_START
#include "kafka/KafkaProducer.h"
THIRD_PARTY_INCLUDES_END

using namespace kafka::clients;

DECLARE_LOG_CATEGORY_EXTERN(LogKafkaProducer, Log, All);

/** Kafka Producer singletone module.*/
class KAFKAPRODUCER_API FKafkaProducerModule : public IModuleInterface
{
public:

	
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	/**
	* Get Kafka Producer singleton.
	*
	* @return FKafkaProducerModule singleton.
	*/
	static FKafkaProducerModule& Get();

	/**
    * Create a kafka producer.
    * 
    * @param Servers Container whose values are summed.
	* @param UserName authorization user/key.
	* @param Password authorization password.
	* @param Configuration List of EKafkaProducerConfig,String map.
	* @param bAutoEventPool=true create thread that porcess events on the BG.
	* @warning Call it once only.
    */
	void CreateProducer(FString Servers, FString UserName, FString Password, TMap<EKafkaProducerConfig, FString> Configuration, bool bAutoEventPool = true);

	/**
	* Create a kafka producer.
	* 
	* @param Servers Container whose values are summed.
	* @param UserName authorization user/key.
	* @param Password authorization password.
	* @param Configuration List of String,String map.
	* @param bAutoEventPool=true create thread that porcess events on the BG.
	*/
	void CreateProducer(FString Servers, FString UserName, FString Password, TMap<FString, FString> Configuration = {}, bool bAutoEventPool = true);

	/**
	* Produce Single record.
	* 
	* @param Topic where to publish
	* @param value authorization user/key.
	* @param Id Used to identify record on 'OnProduce' Callback
	* @param bAsync push record Asynchronous, OnProduce callback will be called
	* @warning bAsync=false blocking.
	*/
	void ProduceRecord(FString Topic,FString Value, int64 Id=-1, bool bAsync = true);

	/**
	* Produce multiple records.
	*
	* @param Topic where to publish
	* @param ValueIdPair map of record value and it's unique Id.
	* @param bAsync push record Asynchronous, OnProduce callback will be called
	* @warning bAsync=false blocking.
	*/
	void ProduceRecords(FString Topic, TMap<FString, int64> ValueIdPair, bool bAsync = true);

	/**
	* Produce single record.
	*
	* @Record FProducerRecord record.
	* @param bAsync push record Asynchronous, OnProduce callback will be called.
	* @warning bAsync=false blocking.
	*/
	void ProduceRecord(FProducerRecord Record, bool bAsync = true);

	/**
	* Produce multiple records.
	*
	* @Records FProducerRecord records list.
	* @param bAsync push record Asynchronous, OnProduce callback will be called,
	* @warning bAsync=false blocking.
	*/
	void ProduceRecords(TArray<FProducerRecord> Records, bool bAsync = true);

	/**
	* Poll events for certain ammount of time (ms).
	*
	* @param BlockingTimeoutMs Time to poll for (ms).
	* @warning Blocking, don't call it on game thread.
	*/
	void PollEvents(int BlockingTimeoutMs);

	/**
	* Purge messages currently handled by producer.
	*
	*/
	void Purge();

	/**
	* Block until the previously sent messages have been delivered.
	*
	* @warning Blocking.
	* @warning might timeout before all records sent.
	*/
	void Flush();

	/**
	* Called on record produced/failed to produce.
	*
	*/
	DECLARE_EVENT_OneParam(FKafkaProducerModule, FProduceEvent, const FProducerCallback& callback)
		FProduceEvent& OnProduce() { return ProduceEvent; }

private:
	FORCEINLINE void Produce(FString Topic, FString Value, FString Key = "", FRecordHeader Headers = FRecordHeader(), int32 Partition = -1, int64 Id = -1, bool bAsync=true);
	FProduceEvent ProduceEvent;
	static FKafkaProducerModule* ProducerSingleton;
	const FString GetKafkaConfigString(EKafkaProducerConfig KafkaConfig);
	kafka::Properties* KafkaProducerProps = nullptr;
	kafka::clients::KafkaProducer* Producer = nullptr;
	const bool bEnableOnProduceCallback = true;
	

};
