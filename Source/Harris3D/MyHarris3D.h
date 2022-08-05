// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>
#include <set>

#include "MeshDescription.h"
#include "MyMesh.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyHarris3D.generated.h"

using namespace std;

// ================================================================================================

UCLASS()
class HARRIS3D_API AMyHarris3D : public AActor
{
	GENERATED_BODY()
	
public:
	virtual void OnConstruction(const FTransform& Transform) override;

	// Sets default values for this actor's properties
	AMyHarris3D();

	//virtual void PostInitializeComponents() override;
	//virtual void PostActorCreated () override;

	//virtual void PostLoad () override;
	//virtual void PostInitProperties () override;
	//virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	UFUNCTION(BlueprintCallable)
	void UpdateHarris3D ();

	UFUNCTION(BlueprintCallable)
	bool GetIsUpdated ();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool m_update_first = false;

public:
	UPROPERTY(EditAnywhere, Category="Inspector")
	bool m_update_click = false;
	
	UPROPERTY(EditAnywhere, Category="Inspector")
	int m_ringSize = 5;

	//UPROPERTY(EditAnywhere, Category="Inspector")
	//bool m_type = false;

	UPROPERTY(EditAnywhere, Category="Inspector")
	double m_fraction = 0.01;

	UPROPERTY(EditAnywhere, Category="Inspector")
	double m_k = 0.04;

	//UPROPERTY(EditAnywhere, Category="Inspector")
	//FColor m_color = FColor (255, 0, 0);

	//UPROPERTY(EditAnywhere, Category="Inspector")
	//double m_radius = 5;

	UPROPERTY (VisibleAnywhere)
	UStaticMeshComponent* m_pMeshCom;

	UPROPERTY(EditAnywhere, Category="Inspector")
	bool m_nms = false;

	UPROPERTY(EditAnywhere, Category="Inspector")
	double m_nms_dist = 10;

	UPROPERTY(EditAnywhere, Category="Inspector")
	double m_nms_dot = 0.5f;

	//UPROPERTY(EditAnywhere, Category="Inspector")
	//double m_bumpSink_dot = 0.0f;

	UPROPERTY(EditAnywhere, Category="Inspector")
	double m_bumpSink_dot_flat = 0.5f;
	
	UPROPERTY(EditAnywhere, Category="Inspector")
	int m_bumpSink_ring = 3;
	
	UPROPERTY(EditAnywhere, Category="Inspector")
	bool m_debugDraw = false;

	UPROPERTY(EditAnywhere, Category="Inspector")
	bool m_debugDraw_postSelected = true;
	
	//UPROPERTY(EditAnywhere, Category="Inspector")
	bool m_debugDraw_unselected = false;

	//UPROPERTY(EditAnywhere, Category="Inspector")
	//char* m_meshDir = ;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	int typeSelection;
	double fraction_constant;
	double k_parameter;
	
	vector <int> vrts_selected;

	//UPROPERTY(BlueprintReadOnly, Category="Inspector")
	TArray <int> vrts_postSelected;
	TArray <FVector> vrtLocs_postSelected;
	TArray <FVector> vrtNors_postSelected;
	UPROPERTY(BlueprintReadOnly, Category="Inspector")
	TArray <FVector> currentVrtLocs_postSelected;
	UPROPERTY(BlueprintReadOnly, Category="Inspector")
	TArray <FVector> currentVrtNors_postSelected;
	UPROPERTY(BlueprintReadOnly, Category="Inspector")
	TArray <EVertexType> vrtTypes_postSelected;

	TArray <int> vrts_unselected;
	TArray <FVector> vrtLocs_unselected;
	TArray <FVector> vrtNors_unselected;
	TArray <FVector> currentVrtLocs_unselected;
	TArray <FVector> currentVrtNors_unselected;

	TArray <int> vrts_overlapped;
	TArray <FVector> vrtLocs_overlapped;
	TArray <FVector> vrtNors_overlapped;
	TArray <FVector> currentVrtLocs_overlapped;
	TArray <FVector> currentVrtNors_overlapped;
	
	// 논문에 제시된 클러스터링은 사용하지 않음
	//std::vector<int> selectedVrts_clustering;
	//TArray <FVector> selectedVrtLocs_clustering;
	//TArray <FVector> selectedVrtNors_clustering;
	//TArray <FVector> currentSelectedVrtLocs_clustering;
	//TArray <FVector> currentSelectedVrtNors_clustering;
	
	FVector actorLocation;
	FVector actorScale;
	FRotator actorRotation;

	MyMesh myMesh;
	int ringSize;
	vector<double> harrisRPoints;

	void InitMyHarris3D ();

	void CalculateHarrisResponse();
	void CalculateNMS ();
	void CalculateVertexType ();
	
	bool GetIsLocalMaxima(unsigned int);
	
	FVector GetVertexLocationByIndex (int i);
	FVector GetVertexNormalByIndex (int i);

	void InitSelectedVertexLocation ();
	void UpdateSelectedVertexLocation ();

	//UPROPERTY(BlueprintReadOnly, Category="Inspector")
	//UFUNCTION(BlueprintCallable, Category="Inspector")
	//EVertexType GetVertexTypeByIndex (int index);
};
