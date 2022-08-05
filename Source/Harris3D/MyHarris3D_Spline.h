// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyHarris3D_Spline.generated.h"

UCLASS()
class HARRIS3D_API AMyHarris3D_Spline : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyHarris3D_Spline();

	virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION(BlueprintNativeEvent)
	void InitializeHarris3DSpline();
	virtual void InitializeHarris3DSpline_Implementation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool m_update_first = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category="Inspector")
	bool m_update_click = false;
};
