// Copyright (c) 2022-2023 MrShaaban, Mohamad Shaaban, https://github.com/sha3sha3/UE-EasyKafka.

#pragma once

#include "Modules/ModuleManager.h"
#include "KafkaConsumer.h"
#include "KafkaProducer.h"

class EASYKAFKA_API FEasyKafkaModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	/**
	* Get Kafka Consumer singleton.
	*
	* @return TSharedPtr<FKafkaConsumerModule> singleton
	*/
	FORCEINLINE TSharedPtr<FKafkaConsumerModule> GetConsumer() const{
		return KafkaConsumer;
	}

	/**
	* Get Kafka Producer singleton.
	*
	* @return TSharedPtr<FKafkaProducerModule> singleton
	*/
	FORCEINLINE TSharedPtr<FKafkaProducerModule> GetProducer() const{
		return KafkaProducer;
	}

	/**
	* Get Easy kafka singleton.
	*
	* @return FEasyKafkaModule ref.
	*/
	static FEasyKafkaModule& Get();
private:
	TSharedPtr<FKafkaConsumerModule> KafkaConsumer = nullptr;
	TSharedPtr<FKafkaProducerModule> KafkaProducer = nullptr;
	static FEasyKafkaModule* EasyKafka;
};
