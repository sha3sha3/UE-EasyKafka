// Copyright (c) 2022-2023 MrShaaban, Mohamad Shaaban, https://github.com/sha3sha3/UE-EasyKafka.

#pragma once

#include "Modules/ModuleManager.h"
#include "KafkaConsumer.h"
#include "KafkaProducer.h"
#include "KafkaAdmin.h"

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
	* Get Kafka Admin singleton.
	*
	* @return TSharedPtr<FKafkaAdminModule> singleton
	*/
	FORCEINLINE TSharedPtr<FKafkaAdminModule> GetAdmin() const {
		return KafkaAdmin;
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
	TSharedPtr<FKafkaAdminModule> KafkaAdmin = nullptr;
	static FEasyKafkaModule* EasyKafka;
};
