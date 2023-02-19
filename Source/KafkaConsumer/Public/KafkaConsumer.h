// Copyright (c) 2022-2023 MrShaaban, Mohamad Shaaban, https://github.com/sha3sha3/UE-EasyKafka.

#pragma once

#include "Modules/ModuleManager.h"
#include "CoreMinimal.h"
#include "KafkaConsumerConfig.h"
#include "Templates/SharedPointer.h"
#include <string>
#include "HAL/Runnable.h"
#include "ConsumerRecord.h"
#include "ConsumerCallback.h"
THIRD_PARTY_INCLUDES_START
#include "kafka/KafkaConsumer.h"
THIRD_PARTY_INCLUDES_END

/**
 *
 */
using namespace kafka::clients;

DECLARE_LOG_CATEGORY_EXTERN(LogKafkaConsumer, Log, All);


/** Kafka Consumer singletone module.*/
class KAFKACONSUMER_API FKafkaConsumerModule : public IModuleInterface,public FRunnable
{
public:


	virtual void StartupModule() override;
	virtual void ShutdownModule() override;


	/**
	* Get Kafka Consumer singleton.
	*
	* @return FKafkaConsumerModule singleton
	*/
	static FKafkaConsumerModule& Get();

	/**
	* Get bAllCommitted.
	*
	* @return true if all offsets commited, else false.
	*/
	const bool IsAllCommited() const
	{
		return bAllCommitted;
	}

	/**
	* Get LastTimeCommitted.
	*
	* @return FDateTime of last time offsets commited.
	*/
	const FDateTime LastTimeCommited() const
	{
		return LastTimeCommitted;
	}

	/*
	only FKafkaConsumer can broadcast events
	other objects can subscribe
	Not Blueprint exposed
	*/

	/**
	* On consumer error callback.
	*/
	DECLARE_EVENT_OneParam(FKafkaConsumerModule, FErrorEvent, const FString& Error)
	FErrorEvent& OnError() { return ErrorEvent; }

	/**
	* On new messages consumed callback.
	*/
	DECLARE_EVENT_OneParam(FKafkaConsumerModule, FNewMessageEvent, const TArray<FConsumerRecord>& Messages)
	FNewMessageEvent& OnNewMessage() { return NewMessageEvent; }

	/**
	* On Commit async callback.
	*/
	DECLARE_EVENT_OneParam(FKafkaConsumerModule, FCommitEvent, const FConsumerCallback& callback)
	FCommitEvent& OnCommit() {return CommitEvent;}

	/**
	* On Stop consuming callback.
	*/
	DECLARE_EVENT(FKafkaConsumerModule, FStopEvent)
	FStopEvent& OnStop() { return StopEvent; }



	/**
	* Create a kafka Consumer.
	*
	* @param Servers Container whose values are summed.
	* @param UserName authorization user/key.
	* @param Password authorization password.
	* @param Configuration List of EKafkaConsumerConfig,String map.
	* @warning Call it once only.
	*/
	void CreateConsumer(FString Servers, FString UserName, FString Password, const TMap<EKafkaConsumerConfig, FString>& Configuration);

	/**
	* Create a kafka Consumer.
	*
	* @param Servers Container whose values are summed.
	* @param UserName authorization user/key.
	* @param Password authorization password.
	* @param Configuration List of String,String map.
	* @warning Call it once only.
	*/
	void CreateConsumer(FString Servers, FString UserName, FString Password, const TMap<FString, FString>& Configuration = {});

	/**
	* Subscribe to topics.
	*
	* @param Topics Array of string topics names.
	* @parm TimeoutMs block until subscribe success, or timeout after ms.
	*/
	void Subscribe(const TArray<FString>& Topics, int TimeoutMs=5000);

	/**
	* Unsubscribe from all topics.
	*
	* @param TimeoutMs block until unsubscribe success, or timeout after ms.
	*/
	void Unsubscribe(int TimeoutMs=5000);

	/**
	* Commit offsets synchronous.
	* @return IsCommited
	* @warning Blocking, should be used on same consumer thread otherwise you might consume same record multiple times.
	*/
	bool Commit();

	/**
	* Commit offsets asynchronous. OnCommit callback called.
	*
	*/
	void CommitAsync();

	/**
	* Start Consuming async.
	* 
	* @return threadId
	*/
	int StartConsuming();

	/**
	* Stop Consuming async, OnStop callback called.
	*
	*/
	void StopConsuming();

	/**
	* Pause consuming records until ContinueConsuming is called.
	* 
	* @note This method doesn't affect partition subscription/assignment
	*/
	void PauseConsuming();
	
	/**
	* Resume consuming records after pause.
	* 
	*/
	void ResumeConsuming();

	/**
	* Get all subscription topics.
	*
	* @return list of all subscription 
	*/
	const TArray<FString> GetSubscription() const;

	/**
	* Set group Id of this consumer.
	*
	*/
	void SetGroupId(FString groupId);

private:
	FErrorEvent ErrorEvent;
	FNewMessageEvent NewMessageEvent;
	FStopEvent StopEvent;
	FCommitEvent CommitEvent;


	static FKafkaConsumerModule* ConsumerSingleton;
	bool bAutoCommit = true;
	bool bAllCommitted = true;
	kafka::clients::KafkaConsumer* Consumer = nullptr;
	const FString GetKafkaConfigString(EKafkaConsumerConfig KafkaConfig);
	FDateTime LastTimeCommitted = FDateTime::Now();
	kafka::Properties* KafkaConsumerProps = nullptr;
	virtual uint32 Run() override;
	virtual void Stop() override;

protected:
	FRunnableThread* Thread = nullptr;
	bool bRunThread = true;
	
};
