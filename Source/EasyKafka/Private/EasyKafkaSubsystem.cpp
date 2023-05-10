// Copyright (c) 2022-2023 MrShaaban, Mohamad Shaaban, https://github.com/sha3sha3/UE-EasyKafka.

#include "EasyKafkaSubsystem.h"
#include "KafkaConsumer.h"
#include "kafkaProducer.h"




void UEasyKafkaSubsystem::Deinitialize()
{
	if(EasyKafka->GetConsumer())
		EasyKafka->GetConsumer()->StopConsuming();//kill the thread
}

void UEasyKafkaSubsystem::CreateConsumerDefault(FString Servers, FString UserName, FString Password, EKafkaLogLevel KafkaLogLevel)
{
	if(EasyKafka->GetConsumer())
	EasyKafka->GetConsumer()->CreateConsumer(Servers, UserName, Password, TMap<FString,FString>(), (int)KafkaLogLevel);
}

void UEasyKafkaSubsystem::CreateConsumer(FString Servers, FString UserName, FString Password, TMap<EKafkaConsumerConfig, FString> Configuration, EKafkaLogLevel KafkaLogLevel)
{
	if(EasyKafka->GetConsumer())
	EasyKafka->GetConsumer()->CreateConsumer(Servers, UserName, Password, Configuration, (int)KafkaLogLevel);
}

void UEasyKafkaSubsystem::CreateConsumerStr(FString Servers, FString UserName, FString Password, TMap<FString, FString> Configuration, EKafkaLogLevel KafkaLogLevel)
{
	if(EasyKafka->GetConsumer())
	EasyKafka->GetConsumer()->CreateConsumer(Servers, UserName, Password, Configuration, (int)KafkaLogLevel);
}

void UEasyKafkaSubsystem::Subscribe(TArray<FString> Topics, int Timeout)
{
	if(EasyKafka->GetConsumer())
	EasyKafka->GetConsumer()->Subscribe(Topics, Timeout);
}

void UEasyKafkaSubsystem::Unsubscribe(int Timeout)
{
	if(EasyKafka->GetConsumer())
	EasyKafka->GetConsumer()->Unsubscribe(Timeout);
}

bool UEasyKafkaSubsystem::Commit()
{
	if (EasyKafka->GetConsumer())
		return EasyKafka->GetConsumer()->Commit();
	else
		return false;
}

void UEasyKafkaSubsystem::CommitAsync()
{
	if(EasyKafka->GetConsumer())
	EasyKafka->GetConsumer()->CommitAsync();
}

int32 UEasyKafkaSubsystem::StartConsuming()
{
	if (EasyKafka->GetConsumer())
		return EasyKafka->GetConsumer()->StartConsuming();
	else 
		return -1;
}

void UEasyKafkaSubsystem::StopConsuming()
{
	if (EasyKafka->GetConsumer())
	EasyKafka->GetConsumer()->StopConsuming();
}

void UEasyKafkaSubsystem::PauseConsuming()
{
	if (EasyKafka->GetConsumer())
		EasyKafka->GetConsumer()->PauseConsuming();
}

void UEasyKafkaSubsystem::ResumeConsuming()
{
	if (EasyKafka->GetConsumer())
		EasyKafka->GetConsumer()->ResumeConsuming();
}

TArray<FString> UEasyKafkaSubsystem::GetSubscription()
{
	if (EasyKafka->GetConsumer())
		return EasyKafka->GetConsumer()->GetSubscription();
	else
		return TArray<FString>();
}

void UEasyKafkaSubsystem::SetGroupId(FString groupId)
{
	if (EasyKafka->GetConsumer())
		EasyKafka->GetConsumer()->SetGroupId(groupId);
}

void UEasyKafkaSubsystem::Initialize(FSubsystemCollectionBase& Collection) 
{
	//not gonna change anything unless u mess up the ELoadingPhase
	FModuleManager::LoadModuleChecked<FEasyKafkaModule>("EasyKafka");
	EasyKafka = MakeShared<FEasyKafkaModule>(FEasyKafkaModule::Get());

	EasyKafka->GetConsumer()->OnNewMessage().AddUObject(this, &UEasyKafkaSubsystem::OnNewMessageCallback);
	EasyKafka->GetConsumer()->OnError().AddUObject(this, &UEasyKafkaSubsystem::OnConsumingError);
	EasyKafka->GetConsumer()->OnCommit().AddUObject(this, &UEasyKafkaSubsystem::OnCommitCallback);
	EasyKafka->GetConsumer()->OnStop().AddUObject(this, &UEasyKafkaSubsystem::OnStopConsuming);
	EasyKafka->GetProducer()->OnProduce().AddUObject(this, &UEasyKafkaSubsystem::OnNewMessageProduced);
}



void UEasyKafkaSubsystem::CreateProducerDefault(FString Servers, FString UserName, FString Password, EKafkaLogLevel KafkaLogLevel)
{
	if (EasyKafka->GetProducer())
		EasyKafka->GetProducer()->CreateProducer(Servers, UserName, Password, TMap<FString, FString>(), true, (int)KafkaLogLevel);
}

void UEasyKafkaSubsystem::CreateProducerStr(FString Servers, FString UserName, FString Password, TMap<FString, FString> Configuration, EKafkaLogLevel KafkaLogLevel)
{
	if (EasyKafka->GetProducer())
		EasyKafka->GetProducer()->CreateProducer(Servers, UserName, Password,Configuration,true, (int)KafkaLogLevel);
}

void UEasyKafkaSubsystem::CreateProducer(FString Servers, FString UserName, FString Password, TMap<EKafkaProducerConfig, FString> Configuration, EKafkaLogLevel KafkaLogLevel)
{
	if (EasyKafka->GetProducer())
		EasyKafka->GetProducer()->CreateProducer(Servers, UserName, Password, Configuration, true, (int)KafkaLogLevel);
}

void UEasyKafkaSubsystem::ProduceRecord(FString Topic, FString Value, int64 Id)
{
	if (EasyKafka->GetProducer())
		EasyKafka->GetProducer()->ProduceRecord(Topic, Value, Id);
}

void UEasyKafkaSubsystem::ProduceRecords(FString Topic, TMap<FString, int64> ValueIdPair)
{
	if (EasyKafka->GetProducer())
		EasyKafka->GetProducer()->ProduceRecords(Topic, ValueIdPair);
}

void UEasyKafkaSubsystem::ProduceRecordStruct(FProducerRecord Record)
{
	if (EasyKafka->GetProducer())
		EasyKafka->GetProducer()->ProduceRecord(Record);
}

void UEasyKafkaSubsystem::ProduceRecordsStruct(TArray<FProducerRecord> Records)
{
	if (EasyKafka->GetProducer())
		EasyKafka->GetProducer()->ProduceRecords(Records);
}

void UEasyKafkaSubsystem::PurgeMessages()
{
	if (EasyKafka->GetProducer())
		EasyKafka->GetProducer()->Purge();
}

void UEasyKafkaSubsystem::FlushProducer()
{
	if (EasyKafka->GetProducer())
		EasyKafka->GetProducer()->Flush();
}

TSharedPtr<FEasyKafkaModule> UEasyKafkaSubsystem::GetEasyKafka()
{
	if (!EasyKafka)
		EasyKafka = MakeShared<FEasyKafkaModule>(FEasyKafkaModule::Get());

	return EasyKafka;
}

void UEasyKafkaSubsystem::OnConsumingError(const FString& Error)
{
	OnConsumerError.Broadcast(Error);
}

void UEasyKafkaSubsystem::OnNewMessageCallback(const TArray<FConsumerRecord>& Messages)
{
	OnNewMessage.Broadcast(Messages);
}

void UEasyKafkaSubsystem::OnCommitCallback(const FConsumerCallback& Callback)
{
	OnCommit.Broadcast(Callback);
}

void UEasyKafkaSubsystem::OnStopConsuming()
{
	OnConsumingStopped.Broadcast();
}

void UEasyKafkaSubsystem::OnNewMessageProduced(const FProducerCallback& Callback)
{
	OnMessageProduced.Broadcast(Callback);
}





