// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "Initialisation.generated.h"

UCLASS()
class PROJECTLAB35_API AInitialisation : public AActor
{
	GENERATED_BODY()
	
public:
	//~ Begin AInitialisation interface
	/**
	* Default constructor
	*/
	AInitialisation();

	USceneComponent* m_pSceneComponent;

	UPROPERTY(EditAnywhere)
	int32 m_graphPointsCountX;

	UPROPERTY(EditAnywhere)
	int32 m_graphPointsCountY;

	UPROPERTY(EditAnywhere)
	int32 m_objectsCount;

	UPROPERTY(EditAnywhere)
	int32 m_offsetX;

	UPROPERTY(EditAnywhere)
	int32 m_offsetY;

	UPROPERTY(EditAnywhere)
	int32 m_offsetZ;

	UPROPERTY(EditAnywhere)
	int32 m_objectVelocity;

	FTransform m_InstanceTransform;

	//~ End AInitialisation interface
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
