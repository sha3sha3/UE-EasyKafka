// Copyright (c) 2022-2023 MrShaaban, Mohamad Shaaban, https://github.com/sha3sha3/UE-EasyKafka.

#include "EasyKafka.h"
#include "KafkaConsumer.h"
#include "KafkaProducer.h"
#include "KafkaAdmin.h"


#define LOCTEXT_NAMESPACE "FEasyKafkaModule"

FEasyKafkaModule* FEasyKafkaModule::EasyKafka = nullptr;

void FEasyKafkaModule::StartupModule()
{
	if (!EasyKafka)
		EasyKafka = this;
	else
		return;
	if (!KafkaConsumer) {
		FModuleManager::LoadModuleChecked<FKafkaConsumerModule>("KafkaConsumer");
		KafkaConsumer = MakeShared<FKafkaConsumerModule>(FKafkaConsumerModule::Get());
	}
	if (!KafkaProducer) {
		FModuleManager::LoadModuleChecked<FKafkaProducerModule>("KafkaProducer");
		KafkaProducer = MakeShared <FKafkaProducerModule>(FKafkaProducerModule::Get());
	}
	if (!KafkaAdmin) {
		FModuleManager::LoadModuleChecked<FKafkaAdminModule>("KafkaAdmin");
		KafkaAdmin = MakeShared <FKafkaAdminModule>(FKafkaAdminModule::Get());
	}
}

void FEasyKafkaModule::ShutdownModule()
{
		
}


FEasyKafkaModule& FEasyKafkaModule::Get()
{
	if (EasyKafka == nullptr)
	{
		check(IsInGameThread());
		FModuleManager::LoadModuleChecked<FKafkaProducerModule>("FEasyKafkaModule");
	}
	check(EasyKafka);

	return *EasyKafka;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FEasyKafkaModule, EasyKafka)