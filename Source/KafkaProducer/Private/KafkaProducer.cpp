// Copyright (c) 2022-2023 MrShaaban, Mohamad Shaaban, https://github.com/sha3sha3/UE-EasyKafka.

#include "KafkaProducer.h"
#include "KafkaProducerConfig.h"


DEFINE_LOG_CATEGORY(LogKafkaProducer);
#define LOCTEXT_NAMESPACE "FKafkaProducerModule"

FKafkaProducerModule* FKafkaProducerModule::ProducerSingleton=nullptr;

void FKafkaProducerModule::StartupModule()
{
	UE_LOG(LogKafkaProducer, Display, TEXT("KafkaProducer Startup"));
	if (!ProducerSingleton)
		FKafkaProducerModule::ProducerSingleton = this;

}

void FKafkaProducerModule::ShutdownModule()
{
	UE_LOG(LogKafkaProducer, Display, TEXT("KafkaProducer Shutdown"));
	if (Producer)
	{
		Producer->close();
		delete Producer;
		Producer = nullptr;
	}
	if (KafkaProducerProps)
	{
		delete KafkaProducerProps;
		KafkaProducerProps = nullptr;
	}
	
}

FKafkaProducerModule& FKafkaProducerModule::Get()
{
	if (ProducerSingleton == nullptr)
	{
		check(IsInGameThread());
		FModuleManager::LoadModuleChecked<FKafkaProducerModule>("FKafkaProducerModule");
	}
	check(ProducerSingleton);

	return *ProducerSingleton;
}

void FKafkaProducerModule::CreateProducer(FString Servers, FString UserName, FString Password, TMap<EKafkaProducerConfig, FString> Configuration, bool bAutoEventPool, int KafkaLogLevel)
{
	TMap<FString, FString> Configurations;

	for (const TPair<EKafkaProducerConfig, FString>& pair : Configuration)
	{
		Configurations.Add(GetKafkaConfigString(pair.Key), pair.Value);
	}

	CreateProducer(Servers, UserName, Password, Configurations, bAutoEventPool, KafkaLogLevel);
}

void FKafkaProducerModule::CreateProducer(FString Servers, FString UserName, FString Password, TMap<FString, FString> Configuration, bool bAutoEventPool, int KafkaLogLevel)
{
	if (KafkaProducerProps)
		delete KafkaProducerProps;
	if (Producer) 
	{
		Producer->close();
		delete Producer;
	}

	KafkaProducerProps = new kafka::Properties({
		   {"bootstrap.servers",  std::string(TCHAR_TO_UTF8(*Servers))},
		   {"log_level",  std::to_string(KafkaLogLevel)}
		});
	/*
	Create producer/consumer with no usr/passwd
	*/
	if (!UserName.Equals("") && !Password.Equals(""))
	{
		KafkaProducerProps->put("security.protocol", "SASL_SSL");
		KafkaProducerProps->put("sasl.mechanisms", "PLAIN");
		KafkaProducerProps->put("sasl.username", std::string(TCHAR_TO_UTF8(*UserName)));
		KafkaProducerProps->put("sasl.password", std::string(TCHAR_TO_UTF8(*Password)));
	}

	for (const TPair<FString, FString>& pair : Configuration)
	{
		KafkaProducerProps->put(std::string(TCHAR_TO_UTF8(*pair.Key)), std::string(TCHAR_TO_UTF8(*pair.Value)));
	}

	Producer = new kafka::clients::KafkaProducer(*KafkaProducerProps, bAutoEventPool? kafka::clients::KafkaClient::EventsPollingOption::Auto:kafka::clients::KafkaClient::EventsPollingOption::Manual);
	UE_LOG(LogKafkaProducer, Display, TEXT("Producer Created"));

}

void FKafkaProducerModule::ProduceRecord(FString Topic, FString value, int64 Id, bool bAsync)
{
	if (!Producer)
		return;
	Produce(Topic, value, "", FRecordHeader(), -1, Id, bAsync);
}

void FKafkaProducerModule::ProduceRecords(FString Topic, TMap<FString, int64> ValueIdPair, bool bAsync)
{
	if (!Producer)
		return;
	for(auto record : ValueIdPair)
		Produce(Topic, record.Key, "", FRecordHeader(), -1, record.Value, bAsync);
}

void FKafkaProducerModule::ProduceRecord(FProducerRecord Record, bool bAsync)
{
	if (!Producer)
		return;
	Produce(Record.Topic, Record.Value, Record.Key, Record.Headers, Record.Partition, Record.Id, bAsync);
}

void FKafkaProducerModule::ProduceRecords(TArray<FProducerRecord> Records, bool bAsync)
{
	if (!Producer)
		return;
	for (auto Record: Records)
		Produce(Record.Topic, Record.Value, Record.Key, Record.Headers, Record.Partition, Record.Id, bAsync);
}



void FKafkaProducerModule::Produce(FString Topic, FString value, FString key, FRecordHeader Headers, int32 Partition, int64 Id, bool bAsync)
{
	/*
	To be optimized
	sync is blocking never use it in bp
	*/
	UE_LOG(LogKafkaProducer, Display, TEXT("Producing record:\nTopic=%s\nValue=%s\nKey=%s"), *Topic, *value, *key);
	auto ValuePtr =std::make_shared<std::string>(std::string(TCHAR_TO_UTF8(*value)));
	auto KeyPtr = std::make_shared<std::string>(std::string(TCHAR_TO_UTF8(*key)));

	auto Record = producer::ProducerRecord(std::string(TCHAR_TO_UTF8(*Topic)),
		                                   key==""? kafka::NullKey: kafka::Key(KeyPtr->c_str(), KeyPtr->size()),
		                                   kafka::Value(ValuePtr->c_str(), ValuePtr->size()));
	if (Partition > -1)
		Record.setPartition(Partition);
	if (Id > -1)
		Record.setId(Id);
		
		
	auto strHeaders=std::make_shared <std::list<std::string>>();
	for (auto header : Headers.StrHeaders) 
	{
		strHeaders->push_back(std::string(TCHAR_TO_UTF8(*header.Value)));
		Record.headers().push_back(kafka::Header(std::string(TCHAR_TO_UTF8(*header.Key)), kafka::Header::Value(strHeaders->back().c_str(), strHeaders->back().size())));
	}

	auto intHeaders = std::make_shared <std::list<std::string>>();
	for (auto header : Headers.IntHeaders) 
	{
		intHeaders->push_back(std::to_string(header.Value));
		Record.headers().push_back(kafka::Header(std::string(TCHAR_TO_UTF8(*header.Key)), kafka::Header::Value(intHeaders->back().c_str(), intHeaders->back().size())));
	}

	if (bAsync) {
		Producer->send(Record,

			[ValuePtr, KeyPtr, this]//capture this instead of exposing ProduceEvent publicly
		    (const producer::RecordMetadata& metadata, const kafka::Error& error) {

				FRecordMetadata recordMetadata = FRecordMetadata(UTF8_TO_TCHAR(metadata.topic().c_str()),
					metadata.partition(), metadata.offset() ? *std::move(metadata.offset()) : -1,
					metadata.recordId() ? *std::move(metadata.recordId()) : -1);

				if (!bEnableOnProduceCallback)
					return;

				if (!error) 
				{
					ProduceEvent.Broadcast(FProducerCallback(false, "", recordMetadata));
				}
				else 
				{
					ProduceEvent.Broadcast(FProducerCallback(true, UTF8_TO_TCHAR(error.message().c_str()), recordMetadata));	
				}

			});
	}
	else
		Producer->syncSend(Record);
	
}

void FKafkaProducerModule::PollEvents(int BlockingTimeoutMs)
{
	if (!Producer)
		return;
	UE_LOG(LogKafkaProducer, Display, TEXT("Started Pooling for events. Timeout %d"),&BlockingTimeoutMs);
	Producer->pollEvents(std::chrono::milliseconds(BlockingTimeoutMs));
}

void FKafkaProducerModule::Purge()
{
	if (Producer) 
	{
		Producer->purge();
	}
}

void FKafkaProducerModule::Flush()
{
	if (Producer)
	{
		Producer->flush();
	}
}

const FString FKafkaProducerModule::GetKafkaConfigString(EKafkaProducerConfig KafkaConfig)
{
	switch (KafkaConfig)
	{
	case    EKafkaProducerConfig::BOOTSTRAP_SERVERS:
		return "bootstrap.servers";
	case	EKafkaProducerConfig::CLIENT_ID:
		return "client.id";
	case	EKafkaProducerConfig::ACKS:
		return "acks";
	case	EKafkaProducerConfig::QUEUE_BUFFERING_MAX_MESSAGES:
		return "queue.buffering.max.messages";
	case	EKafkaProducerConfig::QUEUE_BUFFERING_MAX_KBYTES:
		return "queue.buffering.max.kbytes";
	case    EKafkaProducerConfig::LINGER_MS:
		return "linger.ms";
	case    EKafkaProducerConfig::BATCH_NUM_MESSAGES:
		return "batch.num.messages";
	case	EKafkaProducerConfig::BATCH_SIZE:
		return "batch.size";
	case	EKafkaProducerConfig::MESSAGE_MAX_BYTES:
		return "message.max.bytes";
	case	EKafkaProducerConfig::MESSAGE_TIMEOUT_MS:
		return "message.timeout.ms";
	case	EKafkaProducerConfig::REQUEST_TIMEOUT_MS:
		return "request.timeout.ms";
	case	EKafkaProducerConfig::PARTITIONER:
		return "partitioner";
	case	EKafkaProducerConfig::MAX_IN_FLIGHT:
		return "max.in.flight";
	case	EKafkaProducerConfig::ENABLE_IDEMPOTENCE:
		return "enable.idempotence";
	case	EKafkaProducerConfig::TRANSACTIONAL_ID:
		return "transactional.id";
	case	EKafkaProducerConfig::TRANSACTION_TIMEOUT_MS:
		return "transaction.timeout.ms";
	case	EKafkaProducerConfig::SECURITY_PROTOCOL:
		return "security.protocol";
	case	EKafkaProducerConfig::SASL_KERBEROS_KINIT_CMD:
		return "sasl.kerberos.kinit.cmd";
	case	EKafkaProducerConfig::SASL_KERBEROS_SERVICE_NAME:
		return "sasl.kerberos.service.name";
	case    EKafkaProducerConfig::ENABLE_SSL_CERTIFICATE_VERIFICATION:
		return "enable.ssl.certificate.verification";
	case    EKafkaProducerConfig::LOG_LEVEL:
		return "log_level";
	default:
		return "";
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FKafkaProducerModule, kafkaProducer)