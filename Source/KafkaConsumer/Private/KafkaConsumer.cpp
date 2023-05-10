// Copyright (c) 2022-2023 MrShaaban, Mohamad Shaaban, https://github.com/sha3sha3/UE-EasyKafka.

#include "KafkaConsumer.h"
#include "HAL/RunnableThread.h"



DEFINE_LOG_CATEGORY(LogKafkaConsumer);
#define LOCTEXT_NAMESPACE "FKafkaConsumerModule"

FKafkaConsumerModule* FKafkaConsumerModule::ConsumerSingleton = nullptr;


FKafkaConsumerModule& FKafkaConsumerModule::Get()
{
	if (ConsumerSingleton == nullptr)
	{
		check(IsInGameThread());
		FModuleManager::LoadModuleChecked<FKafkaConsumerModule>("FKafkaConsumerModule");
	}
	check(ConsumerSingleton);
	return *ConsumerSingleton;
}

uint32 FKafkaConsumerModule::Run()
{
	if (!Consumer)
		return -1;

	bRunThread = true;

	UE_LOG(LogKafkaConsumer, Display, TEXT("Started Consuming Async."));

	while (bRunThread)
	{
		auto records = Consumer->poll(std::chrono::milliseconds(100));
		TArray<FConsumerRecord> Messages;
		for (const auto& record : records) {

			if (!record.error()) {
				FConsumerRecord ConsumerRecord;
				ConsumerRecord.Topic = UTF8_TO_TCHAR(record.topic().c_str());
				ConsumerRecord.Partition = record.partition();
				ConsumerRecord.Offset = record.offset();
				ConsumerRecord.Key = UTF8_TO_TCHAR(record.key().toString().c_str());
				ConsumerRecord.Value = UTF8_TO_TCHAR(record.value().toString().c_str());
				ConsumerRecord.Timestamp = FDateTime(record.timestamp().msSinceEpoch);

				for (int i = 0; i < record.headers().size(); i++) {

					ConsumerRecord.Headers.Add(UTF8_TO_TCHAR(record.headers()[i].key.c_str()), UTF8_TO_TCHAR(record.headers()[i].value.toString().c_str()));
				}
				Messages.Add(ConsumerRecord);
				/*
				Here your logic should go if you wish to use manual commit
				*/
			}
			else {
				auto error = UTF8_TO_TCHAR(record.toString().c_str());
				ErrorEvent.Broadcast(error);
				UE_LOG(LogKafkaConsumer, Error, TEXT("%s"),error);
				continue;
			}
		}

		if (Messages.Num() > 0)
			NewMessageEvent.Broadcast(Messages);

	}
	StopEvent.Broadcast();
	return 0;
}

void FKafkaConsumerModule::Stop()
{
	bRunThread = false;
	UE_LOG(LogKafkaConsumer, Display, TEXT("Stopped Consuming."));
}

void FKafkaConsumerModule::SetGroupId(FString groupId)
{
	if (Consumer)
		Consumer->setGroupId(std::string(TCHAR_TO_UTF8(*groupId)));
}

void FKafkaConsumerModule::StopConsuming() 
{
	if (Thread)
	{
		Thread->Kill();
		delete Thread;
		Thread = nullptr;
	}
	if (Consumer)
		Consumer->close();
}

void FKafkaConsumerModule::PauseConsuming()
{
	if (Consumer) 
	{
		Consumer->pause();
	}
}

void FKafkaConsumerModule::ResumeConsuming()
{
	if (Consumer)
	{
		Consumer->resume();
	}
}

const FString FKafkaConsumerModule::GetKafkaConfigString(EKafkaConsumerConfig KafkaConfig)
{
	switch (KafkaConfig)
	{
	case    EKafkaConsumerConfig::BOOTSTRAP_SERVERS:
		return "bootstrap.servers";
	case	EKafkaConsumerConfig::GROUP_ID:
		return "group.id";
	case	EKafkaConsumerConfig::CLIENT_ID:
		return "client.id";
	case	EKafkaConsumerConfig::ENABLE_AUTO_COMMIT:
		return "enable.auto.commit";
	case	EKafkaConsumerConfig::AUTO_OFFSET_RESET:
		return "auto.offset.reset";
	case    EKafkaConsumerConfig::ENABLE_PARTITION_EOF:
		return "enable.partition.eof";
	case    EKafkaConsumerConfig::MAX_POLL_RECORDS:
		return "max.poll.records";
	case	EKafkaConsumerConfig::QUEUED_MIN_MESSAGES:
		return "queued.min.messages";
	case	EKafkaConsumerConfig::SESSION_TIMEOUT_MS:
		return "session.timeout.ms";
	case	EKafkaConsumerConfig::SOCKET_TIMEOUT_MS:
		return "socket.timeout.ms";
	case	EKafkaConsumerConfig::ISOLATION_LEVEL:
		return "isolation.level";
	case	EKafkaConsumerConfig::PARTITION_ASSIGNMENT_STRATEGY:
		return "partition.assignment.strategy";
	case	EKafkaConsumerConfig::SECURITY_PROTOCOL:
		return "security.protocol";
	case	EKafkaConsumerConfig::SASL_KERBEROS_KINIT_CMD:
		return "sasl.kerberos.kinit.cmd";
	case	EKafkaConsumerConfig::SASL_KERBEROS_SERVICE_NAME:
		return "sasl.kerberos.service.name";
	case    EKafkaConsumerConfig::ENABLE_SSL_CERTIFICATE_VERIFICATION:
		return "enable.ssl.certificate.verification";
	case    EKafkaConsumerConfig::LOG_LEVEL:
		return "log_level";
	default:
		return "";
	}
}



void FKafkaConsumerModule::CreateConsumer(FString Servers, FString UserName, FString Password, const TMap<EKafkaConsumerConfig, FString>& Configuration)
{
	TMap<FString, FString> Configurations;

	for (const TPair<EKafkaConsumerConfig, FString>& pair : Configuration)
	{
		Configurations.Add(FKafkaConsumerModule::GetKafkaConfigString(pair.Key), pair.Value);
	}

	FKafkaConsumerModule::CreateConsumer(Servers, UserName, Password, Configurations);
}

void FKafkaConsumerModule::CreateConsumer(FString Servers, FString UserName, FString Password, const TMap<FString, FString>& Configuration)
{
	
	/*
	should not be required unless you are in  the editor
	Or you call Create Consumer multiple times
	P.S: Module is singleton but Consumer is not
	meaning multiple CreateConsumer will override Consumer value
	*/
	StopConsuming();

	if (KafkaConsumerProps)
		delete KafkaConsumerProps;
	if (Consumer)
		delete Consumer;


	KafkaConsumerProps = new kafka::Properties({
		   {"bootstrap.servers",  std::string(TCHAR_TO_UTF8(*Servers))},
		   {"enable.auto.commit", {"true"}}
		});
	/*
	Create producer/consumer with no usr/passwd
	*/
	if (!UserName.Equals("") && !Password.Equals(""))
	{
		KafkaConsumerProps->put("security.protocol", "SASL_SSL");
		KafkaConsumerProps->put("sasl.mechanisms", "PLAIN");
		KafkaConsumerProps->put("enable.ssl.certificate.verification", "false");
		KafkaConsumerProps->put("sasl.username", std::string(TCHAR_TO_UTF8(*UserName)));
		KafkaConsumerProps->put("sasl.password", std::string(TCHAR_TO_UTF8(*Password)));
	}

	for (const TPair<FString, FString>& pair : Configuration)
	{
		KafkaConsumerProps->put(std::string(TCHAR_TO_UTF8(*pair.Key)), std::string(TCHAR_TO_UTF8(*pair.Value)));
	}

	Consumer = new kafka::clients::KafkaConsumer(*KafkaConsumerProps, kafka::clients::KafkaClient::EventsPollingOption::Manual);
}

void FKafkaConsumerModule::Subscribe(const TArray<FString>& Topics, int TimeoutMs)
{
	if (!Consumer)
		return;

	std::set<std::string> topics;

	for (const auto topic : Topics)
	{
		UE_LOG(LogKafkaConsumer, Display, TEXT("Subscribing To:%s."),*topic);
		topics.insert(std::string(TCHAR_TO_UTF8(*topic)));
	}
	Consumer->subscribe(topics, consumer::NullRebalanceCallback, std::chrono::milliseconds(TimeoutMs));
}

void FKafkaConsumerModule::Unsubscribe(int TimeoutMs)
{
	if (!Consumer)
		return;
	Consumer->unsubscribe(std::chrono::milliseconds(TimeoutMs));
	UE_LOG(LogKafkaConsumer, Display, TEXT("Unsubscribed from all topics."));
}

bool FKafkaConsumerModule::Commit()
{
	if (!Consumer)
		return false;

	if (!bAllCommitted) {
		Consumer->commitSync();
		LastTimeCommitted = FDateTime::Now();
		bAllCommitted = true;
	}

	UE_LOG(LogKafkaConsumer, Display, TEXT("Offset Commited.")); 

	return true;
}

void FKafkaConsumerModule::CommitAsync()
{
	if (!Consumer)
		return;

	if (!bAllCommitted) {
		UE_LOG(LogKafkaConsumer, Display, TEXT("Commiting Offset Async."));
		Consumer->commitAsync([this](const kafka::TopicPartitionOffsets& topicPartitionOffsets, const kafka::Error& error) 
			{
				FConsumerCallback callback;
				UE_LOG(LogKafkaConsumer, Display, TEXT("Offset Commited Async."));
				for (const auto& topicPartitionOffset : topicPartitionOffsets)
				{
					callback.TopicPartitionOffsets.Add(UTF8_TO_TCHAR(topicPartitionOffset.first.first.c_str()), topicPartitionOffset.second);
				}

				if (error) {
					callback.bError = true;
					callback.ErrorMessage = UTF8_TO_TCHAR(error.message().c_str());
				}

				CommitEvent.Broadcast(callback);
			});
		LastTimeCommitted = FDateTime::Now();
		bAllCommitted = true;
	}

}

int FKafkaConsumerModule::StartConsuming()
{
	if (Consumer == nullptr)
		return -1;
	if (Thread)
		return 1;

	Thread = FRunnableThread::Create(this, TEXT("Kafka Consumer"));
	return 0;
}

const TArray<FString> FKafkaConsumerModule::GetSubscription() const
{

	if (!Consumer)
		return TArray<FString>();

	TArray<FString> Subscriptions;
	for (const auto topic : Consumer->subscription())
	{
		Subscriptions.Add(UTF8_TO_TCHAR(topic.c_str()));
	}
	return Subscriptions;
}

void FKafkaConsumerModule::StartupModule()
{
	UE_LOG(LogKafkaConsumer, Display, TEXT("KafkaConsumer Startup"));
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	ConsumerSingleton = this;
	
}

void FKafkaConsumerModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	UE_LOG(LogKafkaConsumer, Display, TEXT("KafkaConsumer Shutdown"));
	if (Thread)
	{
		StopConsuming();
	}
	if (Consumer)
	{
		Consumer->close();
		delete Consumer;
		Consumer = nullptr;
	}
	if (KafkaConsumerProps)
	{
		delete KafkaConsumerProps;
		KafkaConsumerProps = nullptr;
	}
	
}


#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FKafkaConsumerModule, KafkaConsumer)