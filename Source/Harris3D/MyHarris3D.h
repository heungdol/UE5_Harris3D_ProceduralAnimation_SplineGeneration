// ==================================================================================================
// MyHarris3D.h
// Harris3D를 이용한 Keypoint 탐지 후 활용할 수 있는 클래스 및 액터입니다.
// Github에 공개되어 있는 Harris3D 탐지 소스를 활용하였습니다.
// ==================================================================================================


#pragma once

#include <vector>
#include <set>

#include "MeshDescription.h"
#include "MyMesh.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyHarris3D.generated.h"

// DebugLine을 그리기 위한 시간 주기
#define DEBUG_DRAW_PERIOD 0.5
// 중복 Vertex의 Keypoint에 쓰일 더미 값
#define HARRIS_POINT_MIN -1000

using namespace std;

UCLASS()
class HARRIS3D_API AMyHarris3D : public AActor
{
	GENERATED_BODY()
	
public:
	virtual void OnConstruction(const FTransform& Transform) override;

	AMyHarris3D();

	// Keypoint 계산
	UFUNCTION(BlueprintCallable)
	void UpdateHarris3D ();

	// 업데이트 수행 여부
	UFUNCTION(BlueprintCallable)
	bool GetIsUpdated ();

	// 계산된 Keypoint Location 배열 반환
	UFUNCTION(BlueprintCallable)
	const TArray <FVector>& GetKeyPointLocations () const;

	// 계산된 Keypoint Normal 배열 반환
	UFUNCTION(BlueprintCallable)
	const TArray <FVector>& GetKeyPointNormals () const;

	// 계산된 Keypoint Type 배열 반환
	UFUNCTION(BlueprintCallable)
	const TArray <EVertexType>& GetKeyPointTypes () const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 첫 실행시 활용되는 변수
	bool m_update_first = false;

	// 업데이트 트리거
	UPROPERTY(EditAnywhere, Category="Inspector")
	bool m_update_click = false;
	
	// Vertex 이웃 정도
	UPROPERTY(EditAnywhere, Category="Inspector")
	int m_ringSize = 5;

	// Harris3D 변수
	UPROPERTY(EditAnywhere, Category="Inspector")
	double m_fraction = 0.01;

	// Harris3D 변수
	UPROPERTY(EditAnywhere, Category="Inspector")
	double m_k = 0.04;

	// StaticMesh
	UPROPERTY (VisibleAnywhere)
	UStaticMeshComponent* m_pMeshCom;

	// NMS를 활용한 중복 Keypoint 제거 여부
	UPROPERTY(EditAnywhere, Category="Inspector")
	bool m_nms = false;

	// NMS 변수: 거리 임계값
	UPROPERTY(EditAnywhere, Category="Inspector")
	double m_nms_dist = 10;

	// NMS 변수: Normal 내적 임계값
	UPROPERTY(EditAnywhere, Category="Inspector")
	double m_nms_dot = 0.5f;

	// Vertex Type 변수: Normal 내적 입계 값
	UPROPERTY(EditAnywhere, Category="Inspector")
	double m_bumpSink_dot_flat = 0.5f;
	
	// Vertex Type 변수: 평균을 구할 때의 이웃 정도
	UPROPERTY(EditAnywhere, Category="Inspector")
	int m_bumpSink_ring = 3;
	
	// 디버그 
	UPROPERTY(EditAnywhere, Category="Inspector")
	bool m_debugDraw = false;

	// 디버그: 후처리 된 것만
	UPROPERTY(EditAnywhere, Category="Inspector")
	bool m_debugDraw_postSelected = true;

	// 디버그: 선택되지 않은 것만 (사용하지 않음)
	bool m_debugDraw_unselected = false;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Harris3D 수행 후의 Keypoint 모음 (NMS 적용되지 않음)
	vector <int> vrts_selected;

	// Harris3D 수행 후의 Keypoint 모음 (NMS 적용)
	TArray <int> vrts_postSelected;
	TArray <FVector> vrtLocs_postSelected;
	TArray <FVector> vrtNors_postSelected;

	// Keypoint들의 Location 및 Normal, Vertex Type 배열 반환
	// 블루프린트에서 활용할 수 있음
	UPROPERTY(BlueprintReadOnly, Category="Inspector")
	TArray <FVector> currentVrtLocs_postSelected;

	UPROPERTY(BlueprintReadOnly, Category="Inspector")
	TArray <FVector> currentVrtNors_postSelected;
	
	UPROPERTY(BlueprintReadOnly, Category="Inspector")
	TArray <EVertexType> vrtTypes_postSelected;

	// NMS 후 필터링 된 Keypoint 모음 (사용하지 않음)
	// TArray <int> vrts_unselected;
	// TArray <FVector> vrtLocs_unselected;
	// TArray <FVector> vrtNors_unselected;
	// TArray <FVector> currentVrtLocs_unselected;
	// TArray <FVector> currentVrtNors_unselected;

	// 오버랩 된 Vertex 겸 Keypoint 모음 (사용하지 않음)
	// TArray <int> vrts_overlapped;
	// TArray <FVector> vrtLocs_overlapped;
	// TArray <FVector> vrtNors_overlapped;
	// TArray <FVector> currentVrtLocs_overlapped;
	// TArray <FVector> currentVrtNors_overlapped;
	
	// 논문에 제시된 클러스터링은 사용하지 않음
	//std::vector<int> selectedVrts_clustering;
	//TArray <FVectorD> selectedVrtLocs_clustering;
	//TArray <FVector> selectedVrtNors_clustering;
	//TArray <FVector> currentSelectedVrtLocs_clustering;
	//TArray <FVector> currentSelectedVrtNors_clustering;
	
	// 계산에 활용될 harris 변수 모음
	double fraction_constant;
	double k_parameter;
	int ringSize;

	// 계산된 Harris 상수 모음
	vector<double> harrisRPoints;

	// Actor Transform
	FVector actorLocation;
	FVector actorScale;
	FRotator actorRotation;

	// Actor Details
	MyMesh myMesh;

public:
	// 입력된 상수 값 저장
	void InitMyHarris3D ();

	// Keypoint 계산
	void CalculateHarrisResponse();
	void CalculateNMS ();
	void CalculateVertexType ();
	
	// Vertex Index를 이용하여 해당 Vertex가 이웃으로부터 가장 높은 Harris 상수를 가지는지
	bool GetIsLocalMaxima(unsigned int);
	
	// Vertex Index를 이용하여 해당 Vertex의 Location 및 Normal 반환
	FVector GetVertexLocationByIndex (int i);
	FVector GetVertexNormalByIndex (int i);

	// 실제 Unreal 에디터에서 사용될 수 있도록 Actor의 Transform를 이용하여 변수 가공
	void InitSelectedVertexLocation ();
	
	// 주기마다 Keypoint의 DebugLine 출력
	void UpdateSelectedVertexLocation ();
};
