// Copyright (c) 2022-2023 MrShaaban, Mohamad Shaaban, https://github.com/sha3sha3/UE-EasyKafka.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Templates/SharedPointer.h"
#include "ConsumerCallback.h"
#include "ConsumerRecord.h"
#include "EasyKafka.h"
#include "ProducerCallback.h"
#include "KafkaProducerConfig.h"
#include "ProducerRecord.h"
#include "EasyKafkaSubsystem.generated.h"


UCLASS()
class EASYKAFKA_API UEasyKafkaSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()


public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	/*
	              Consumer Note
	Don't use manual commit unless you go single thread
	Instead of using Events
	Otherwise you might end up reading the message twice.
	*/
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FConsumerError, const FString&, Error);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FConsumingStopped);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCommit, const FConsumerCallback&, CommitCallback);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewMessage,const TArray<FConsumerRecord>&, Messages);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Create Cunsumer", ToolTip = "Create a kafka Consumer. Call it once.", Keywords = "EasyKafka sample test testing"), Category = "EasyKafka|Consumer")
		 void CreateConsumerDefault(FString Servers, FString UserName, FString Password);
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Create Cunsumer", ToolTip = "Create a kafka Consumer. Call it once.", Keywords = "EasyKafka sample test testing"), Category = "EasyKafka|Consumer")
		 void CreateConsumer(FString Servers, FString UserName, FString Password, TMap<EKafkaConsumerConfig, FString> Configuration);
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Create Cunsumer", ToolTip = "Create a kafka Consumer. Call it once.", Keywords = "EasyKafka sample test testing"), Category = "EasyKafka|Consumer")
		 void CreateConsumerStr(FString Servers, FString UserName, FString Password, TMap<FString, FString> Configuration);
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Subscribe", ToolTip = "Subscribe to topics", Keywords = "EasyKafka sample test testing"), Category = "EasyKafka|Consumer")
		 void Subscribe(TArray<FString> Topics, int Timeout=1000 );
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Unsubscribe", ToolTip ="Unsubscribe from all topics.", Keywords = "EasyKafka sample test testing"), Category = "EasyKafka|Consumer")
		 void Unsubscribe(int Timeout=1000);
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Commit", ToolTip = "Commit offsets synchronous.warning Blocking.might consume same record multiple times.", Keywords = "EasyKafka sample test testing"), Category = "EasyKafka|Consumer")
		 bool Commit();
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Commit Async", ToolTip = "", Keywords = "EasyKafka sample test testing"), Category = "EasyKafka|Consumer")
		 void CommitAsync();
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Start Consuming", ToolTip = "Start Consuming async.", Keywords = "EasyKafka sample test testing"), Category = "EasyKafka|Consumer")
		 int32 StartConsuming();
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Stop Consuming", ToolTip = "Stop Consuming async, OnStop callback called.", Keywords = "EasyKafka sample test testing"), Category = "EasyKafka|Consumer")
		 void StopConsuming();
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Pause Consuming", ToolTip = "Pause consuming records until ContinueConsuming is called.", Keywords = "EasyKafka sample test testing"), Category = "EasyKafka|Consumer")
		void PauseConsuming();
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Resume Consuming", ToolTip = "Resume consuming records after pause.", Keywords = "EasyKafka sample test testing"), Category = "EasyKafka|Consumer")
		void ResumeConsuming();
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Subscription", ToolTip = "Get all subscription topics.", Keywords = "EasyKafka sample test testing"), Category = "EasyKafka|Consumer")
		 TArray<FString> GetSubscription();
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetGoupId", ToolTip = "Set group Id of this consumer.", Keywords = "EasyKafka sample test testing"), Category = "EasyKafka|Consumer")
		 void SetGroupId(FString GroupId);
	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "OnConsumerError", ToolTip = "On consumer error callback.", Keywords = ""), Category = "EasyKafka|Consumer")
		 FConsumerError OnConsumerError;
	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "OnConsumeStopped", ToolTip = "On Stop consuming callback.", Keywords = ""), Category = "EasyKafka|Consumer")
		 FConsumingStopped OnConsumingStopped;
	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "OnCommit", ToolTip = "On Commit async callback.", Keywords = ""), Category = "EasyKafka|Consumer")
		 FOnCommit OnCommit;
	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "OnNewMessage", ToolTip = "On new messages consumed callback.", Keywords = ""), Category = "EasyKafka|Consumer")
		 FOnNewMessage OnNewMessage;

	

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnProduce, const FProducerCallback&, ProduceCallback);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Create Producer", ToolTip = "Create a kafka producer. Call once!", Keywords = ""), Category = "EasyKafka|Producer")
		void CreateProducerDefault(FString Servers, FString UserName, FString Password);
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Create Producer", ToolTip = "Create a kafka producer. Call once!", Keywords = ""), Category = "EasyKafka|Producer")
		void CreateProducerStr(FString Servers, FString UserName, FString Password, TMap<FString, FString> Configuration);
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Create Producer", ToolTip = "Create a kafka producer. Call once!", Keywords = ""), Category = "EasyKafka|Producer")
		void CreateProducer(FString Servers, FString UserName, FString Password, TMap<EKafkaProducerConfig, FString> Configuration);
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Produce Record", ToolTip = "Produce Single record", Keywords = ""), Category = "EasyKafka|Producer")
		void ProduceRecord(FString Topic, FString Value, int64 Id = -1);
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Produce Records", ToolTip = "Produce multiple records.", Keywords = ""), Category = "EasyKafka|Producer")
		void ProduceRecords(FString Topic, TMap<FString, int64> ValueIdPair);
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Produce Record", ToolTip = "Produce Single record", Keywords = ""), Category = "EasyKafka|Producer")
		void ProduceRecordStruct(FProducerRecord Record);
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Produce Records", ToolTip = "Produce multiple records.", Keywords = ""), Category = "EasyKafka|Producer")
		void ProduceRecordsStruct(TArray<FProducerRecord> Records);
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Flush Producer", ToolTip = "Block until the previously sent messages have been delivered. Don't call on game thread.", Keywords = ""), Category = "EasyKafka|Producer")
		void FlushProducer();
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Purge Messages", ToolTip = "Purge messages currently handled by producer.", Keywords = ""), Category = "EasyKafka|Producer")
		void PurgeMessages();
	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "OnMessageProduced", ToolTip = "OnMessage produced callback.(failed/succeded)", Keywords = ""), Category = "EasyKafka|Producer")
	FOnProduce OnMessageProduced;

	TSharedPtr<FEasyKafkaModule> GetEasyKafka();
private:
	TSharedPtr<FEasyKafkaModule> EasyKafka=nullptr;

	void OnConsumingError(const FString& Error);
	void OnNewMessageCallback(const TArray<FConsumerRecord>& Messages);
	void OnCommitCallback(const FConsumerCallback& callback);
	void OnStopConsuming();
	void OnNewMessageProduced(const FProducerCallback& callback);

};
