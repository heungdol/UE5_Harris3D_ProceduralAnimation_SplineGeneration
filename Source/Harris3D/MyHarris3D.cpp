// Fill out your copyright notice in the Description page of Project Settings.

#include "MyHarris3D.h"

#include <iostream>
#include <set>
#include <iterator>
#include <algorithm> //for difference
#include <numeric> //for sum
#include <string>

#include <ThirdParty/Eigen/Eigen/Core>
#include <ThirdParty/Eigen/Eigen/Dense>
#include <ThirdParty/Eigen/Eigen/Eigenvalues>

#include "Containers/UnrealString.h"

using namespace std;
using namespace ECollisionEnabled;

// Construction
void AMyHarris3D::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	// 첫 실행 혹은 에디터에서 갱신 bool를 활성화할 시
	if (m_update_first == false || m_update_click == true)
	{
		m_update_first = true;
		m_update_click = false;
		
		UpdateHarris3D ();
	}
}

bool AMyHarris3D::GetIsUpdated()
{
	return m_update_first;
}

// Sets default values
AMyHarris3D::AMyHarris3D()
{
	// 해당 Actor가 Tick를 사용하도록 설정
	PrimaryActorTick.bCanEverTick = true;

	// StaticMesh 컴포넌트 생성 후 부착
	m_pMeshCom = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	RootComponent = m_pMeshCom;
}

void AMyHarris3D::UpdateHarris3D ()
{
	// 메쉬 판단
	if (!m_pMeshCom)
		return;
	
	myMesh = MyMesh (m_pMeshCom);

	// 만약 적절하지 않은 모델이라면
	// (버텍스 개수가 50000개 이상인 경우)
	if (myMesh.GetIsEnableModel() == false)
		return;	

	// 콜리젼 설정
	m_pMeshCom->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	// 사용할 배열 초기화
	vrts_selected.clear();
	vrts_postSelected.Empty();
	
	vrtLocs_postSelected.Empty();
	vrtNors_postSelected.Empty();
	vrtTypes_postSelected.Empty();

	currentVrtLocs_postSelected.Empty();
	currentVrtNors_postSelected.Empty();

	// vrts_unselected.Empty();
	// vrtLocs_unselected.Empty();
	// vrtNors_unselected.Empty();
	// currentVrtLocs_unselected.Empty();
	// currentVrtNors_unselected.Empty();

	// vrts_overlapped.Empty();
	// vrtLocs_overlapped.Empty();
	// vrtNors_overlapped.Empty();
	// currentVrtLocs_overlapped.Empty();
	// currentVrtNors_overlapped.Empty();

	harrisRPoints.clear();

	// Harris3D -> NMS -> VertexType 순으로 수행
	InitMyHarris3D();
	CalculateHarrisResponse();
	CalculateNMS ();
	CalculateVertexType ();

	// 계산된 Keypoint들을 World에서 활용할 수 있도록 가공 후 Debug 출력
	InitSelectedVertexLocation ();
	UpdateSelectedVertexLocation();
}

// Called when the game starts or when spawned
void AMyHarris3D::BeginPlay()
{
	Super::BeginPlay();

	UpdateSelectedVertexLocation();
}

// Called every frame
void AMyHarris3D::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMyHarris3D::InitMyHarris3D()
{
	// 링 사이즈
	AMyHarris3D::ringSize = m_ringSize;

	// 전체 버택스 비율로 내어 선택할 개수를 구할 때 사용되는 상수
	AMyHarris3D::fraction_constant = m_fraction;

	// 계산에 사용되는 상수
	AMyHarris3D::k_parameter = m_k;
}

// Github에서 공개된 소스코드 활용
//calculates the Harris reponse of each vertex
void AMyHarris3D::CalculateHarrisResponse()
{
	int vertexSize = myMesh.GetVertices ().size();
	
	for (int indexVertex = 0; indexVertex < vertexSize; indexVertex++)
	{
		// 중복인 경우 계산하지 않고 컨티뉴
		if (indexVertex != myMesh.GetOverlappingVertices ()[indexVertex])
		{
			// 더미 값을 넣음
			harrisRPoints.push_back(HARRIS_POINT_MIN);
			continue;
		}

		//caculate the neighbourhood
		vector<double> x_coord, y_coord, z_coord;
		set<int> set_nhd;

		//calculate the k rings neighbourhood of each vertex
		set_nhd = myMesh.CalculateNeighbourhood_Ring(indexVertex, ringSize);

		set<int>::iterator itr;
		for (itr = set_nhd.begin(); itr != set_nhd.end(); ++itr)
		{
			//get the x,y,z coordinates
			x_coord.push_back(myMesh.GetVertices ()[*itr].GetX());
			y_coord.push_back(myMesh.GetVertices ()[*itr].GetY());
			z_coord.push_back(myMesh.GetVertices ()[*itr].GetZ());
		}

		//adding the vertex itself to the set, the last element
		x_coord.push_back(myMesh.GetVertices ()[indexVertex].GetX());
		y_coord.push_back(myMesh.GetVertices ()[indexVertex].GetY());
		z_coord.push_back(myMesh.GetVertices ()[indexVertex].GetZ());

		//calculate centroid of the neighbourhood Vk(v)
		int nhd_size = x_coord.size();

		double sum_x = std::accumulate(x_coord.begin(), x_coord.end(), 0.0);
		double averg_x = (double)sum_x / nhd_size;

		double sum_y = std::accumulate(y_coord.begin(), y_coord.end(), 0.0);
		double averg_y = (double)sum_y / nhd_size;

		double sum_z = std::accumulate(z_coord.begin(), z_coord.end(), 0.0);
		double averg_z = (double)sum_z / nhd_size;

		//apply PCA to get the normal of the fitting plane
		//using Eigen Library

		//translate the set of points so that centroid is on the origin
		//Matrix= 3*nhd_size

		Eigen::MatrixXd nhd_matrix(3, nhd_size);
		for (int jj = 0; jj < nhd_size; jj++)
		{
			//store them in Matrix
			//x_trans = x_coord - x_centroid
			nhd_matrix(0, jj) = x_coord[jj] - averg_x;
			nhd_matrix(1, jj) = y_coord[jj] - averg_y;
			nhd_matrix(2, jj) = z_coord[jj] - averg_z;
		}

		//Covariance matrix C
		// 1/n-1*X*Xt
		Eigen::Matrix3d CovM;
		CovM = (nhd_matrix * nhd_matrix.transpose()) / (nhd_size - 1); //creates a symmetric matrix

		// Calculate EigenVectors and EigenValues of Covaraince matrix
		Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> es(CovM);
		//SelfAdjointEigenSolver if the matrix is symmetric(faster)

		Eigen::MatrixXd eig_values(3, 1);
		eig_values = es.eigenvalues().real(); //sorted in increasing order
		Eigen::Matrix3d principal_comps = es.eigenvectors().real();

		//sort eigenvectors in decreasing order by swaping
		Eigen::MatrixXd tmp(3, 1);
		tmp = principal_comps.col(0);
		principal_comps.col(0) = principal_comps.col(2);
		principal_comps.col(2) = tmp;

		//set of points is rotated so that the normal of the fitting plane is the z-axis
		Eigen::MatrixXd rotated_points(3, nhd_size);
		rotated_points = principal_comps.transpose() * nhd_matrix;

		//translate the set of points so that the point v is in the origin of the XY-plane
		double x_vertex = rotated_points(0, nhd_size - 1);
		double y_vertex = rotated_points(1, nhd_size - 1);
		double z_vertex = rotated_points(2, nhd_size - 1);

		Eigen::MatrixXd trans_points(3, nhd_size);
		for (int jk = 0; jk < nhd_size; jk++)
		{
			//trans_points = rotated_points - vertex
			trans_points(0, jk) = rotated_points(0, jk) - x_vertex;
			trans_points(1, jk) = rotated_points(1, jk) - y_vertex;
			trans_points(2, jk) = rotated_points(2, jk) - z_vertex;
		}

		//fit a quadratic surface to the set of transformed points
		//z = f(x,y) =p1/2*x2 +p2*x*y + p3/2*y2 +p4*x +p5*y +p6
		Eigen::MatrixXd eqns(nhd_size, 6); // equations
		Eigen::MatrixXd bvector(nhd_size, 1);
		Eigen::MatrixXd xvector(6, 1);
		for (int kk = 0; kk < nhd_size; kk++)
		{
			double xv = trans_points(0, kk);
			double yv = trans_points(1, kk);
			double zv = trans_points(2, kk);

			bvector(kk, 0) = zv;

			eqns(kk, 0) = (xv * xv) / 2; //coefficient of p1
			eqns(kk, 1) = xv * yv; //p2
			eqns(kk, 2) = (yv * yv) / 2; //p3
			eqns(kk, 3) = xv; //p4
			eqns(kk, 4) = yv; //p5
			eqns(kk, 5) = 1; //p6
		}

		//solve the linear system Ax=b
		xvector = eqns.colPivHouseholderQr().solve(bvector);

		//extract the solution of the linear system
		double p1 = xvector(0, 0);
		double p2 = xvector(1, 0);
		double p3 = xvector(2, 0);
		double p4 = xvector(3, 0);
		double p5 = xvector(4, 0);
		double p6 = xvector(5, 0);

		double A = p4 * p4 + 2 * p1 * p1 + 2 * p2 * p2;
		double B = p4 * p4 + 2 * p2 * p2 + 2 * p3 * p3; //difference with source code p5 = p2 =0.3..
		double C = p4 * p5 + 2 * p1 * p2 + 2 * p2 * p3;

		//Harris operator value in the point v        
		double harrisV = (A * B) - (C * C) - k_parameter * ((A + B) * (A + B));
		harrisRPoints.push_back(harrisV);
	} //endforeachvertex

	//Pre-selection of the interest points
	//preserve the vertices which are local maximum
	
	// 선 선택된 Keypoint들의 모음
	vector<int> preselected;
	for (int nV = 0; nV < vertexSize; nV++)
	{
		// 중복 패스
		if (nV != myMesh.GetOverlappingVertices ()[nV])
		{
			continue;
		}
		
		// 주변 이웃로부터 가장 높은 harris 수치를 가지면
		bool localMaxima = GetIsLocalMaxima(nV);
		if (localMaxima == true)
		{
			preselected.push_back(nV);
		}
	}

	//sort the preselected vertices, decreasing order
	sort(preselected.rbegin(), preselected.rend());
	{
		//Selecting interest points
		vector<int> selectedVertices; //Highest Harris Method

		//Convert set to VectorXi
		int numPreselected = preselected.size();
		Eigen::VectorXi preSelectedVertexes(numPreselected);
		int ctrlVar1(0);
		for (vector<int>::iterator it = preselected.begin(); it != preselected.end(); ++it)
		{
			preSelectedVertexes(ctrlVar1) = *it;
			ctrlVar1++;
		}

		//Get vector with harris values
		Eigen::VectorXd preSelectedHarrisValues(numPreselected);
		for (int iPre = 0; iPre < numPreselected; iPre++)
		{
			preSelectedHarrisValues(iPre) = harrisRPoints[preSelectedVertexes(iPre)];
		}

		// vector<int> _selectedVertices;

		double maxi(0);
		for (int iIP = 0; iIP < preSelectedVertexes.size(); iIP++)
		{
			maxi = preSelectedHarrisValues.maxCoeff();
			for (int i = 0; i < preSelectedVertexes.size(); i++)
			{
				if (abs(maxi - preSelectedHarrisValues(i)) < 0.00001)
				{
					vrts_selected.push_back(preSelectedVertexes(i));
					preSelectedHarrisValues(i) = 0;
					break;
				}
			}
		}

		//sort the preselected vertices, decreasing order
		sort(preselected.rbegin(), preselected.rend());

		// vrts_selected = _selectedVertices;
	}

	// 정렬 및 중복 제거
	sort (vrts_selected.begin(), vrts_selected.end());
	vrts_selected.erase (unique (vrts_selected.begin(), vrts_selected.end()), vrts_selected.end());
}

// Non-Maximum Suppression
void AMyHarris3D::CalculateNMS ()
{
	if (m_nms == false)
	{
		vector <int>::iterator iter = vrts_selected.begin();
		for (; iter != vrts_selected.end(); ++iter)
			vrts_postSelected.Add(*iter);
		return;
	}

	// 초기화
	vrts_postSelected.Empty();
	// vrts_unselected.Empty();
	// vrts_overlapped.Empty();
	
	while (vrts_selected.size() > 1)
	{
		int vrt_best = vrts_selected[0];
		double harris_best = harrisRPoints[vrts_selected[0]];

		// 가장 베스트
		vector <int>::iterator iter = vrts_selected.begin()+1;
		for (; iter != vrts_selected.end(); ++iter)
		{
			if (harris_best < harrisRPoints[*iter])
			{
				vrt_best = *iter;
				harris_best = harrisRPoints[*iter];
			}
		}

		// 베스트 추가
		vrts_postSelected.Add(vrt_best);

		// 베스트 제외
		vrts_selected.erase(find (vrts_selected.begin(), vrts_selected.end(), vrt_best));
		
		// 베스트 Keypoint Location 및 Normal 
		FVector loc_best = myMesh.GetVertexLocByIndex(vrt_best);
		FVector nor_best = myMesh.GetVertexNorByIndex(vrt_best);

		// 필터링
		vector <int> filtering;
		iter = vrts_selected.begin();
		for (; iter != vrts_selected.end(); ++iter)
		{
			// 혹시나 베스트 Keypoint가 존재했을 경우 조건 추가
			if (vrt_best == *iter)
			{
				filtering.push_back(*iter);
				continue;
			}

			FVector loc_curt = myMesh.GetVertexLocByIndex(*iter);
			FVector nor_curt = myMesh.GetVertexNorByIndex(*iter);

			float dist = FVector::Distance(loc_curt, loc_best);
			float dot = FVector::DotProduct(nor_curt, nor_best);

			// IoU 대응: 거리와 노멀 내적값으로 판단
			if (dist < m_nms_dist && dot > m_nms_dot)
			{
				filtering.push_back(*iter);
			}
		}

		if (filtering.size() <= 0)
			continue;

		// 필터링 제외
		// iter = filtering.begin();
		// for (; iter != filtering.end(); ++iter)
		// 	vrts_unselected.Remove(*iter);

		for (int index = 0; index < filtering.size(); index++)
		{
			vrts_selected.erase(find (vrts_selected.begin(), vrts_selected.end(), filtering[index]));
		}
	}

	// 남은 Keypoint 넣고 마무리
	for (int index = 0; index < vrts_selected.size(); index++)
	{
		vrts_postSelected.Add(vrts_selected[index]);
	}
	
	// 오버랩 확인
	// for (int i = 0; i < myMesh.GetOverlappingVertices ().size(); i++)
	// {
	// 	if (i != myMesh.GetOverlappingVertices ()[i])
	// 		vrts_overlapped.Add(i);
	// }
}

void AMyHarris3D::CalculateVertexType()
{
	// 선택된 keypoint 돌출 / 함몰 계산
	for (int index = 0; index < vrts_postSelected.Num(); index++)
	{
		MyVertex& mv = myMesh.GetVertices ()[vrts_postSelected[index]];
		mv.CalculateVertexType(&myMesh, m_bumpSink_ring, m_bumpSink_dot_flat);
	}
}


//To check whether a vertex is a local maximum or not
bool AMyHarris3D::GetIsLocalMaxima(unsigned int vertexIndex)
{
	set<int>::iterator itrr;
	for (itrr = myMesh.GetVertices ()[vertexIndex].GetNeighbours().begin()
	; itrr != myMesh.GetVertices ()[vertexIndex].GetNeighbours().end(); ++itrr)
	{
		if (harrisRPoints[vertexIndex] < harrisRPoints[*itrr])
		{
			return false;
		}
	}
	return true;
}

void AMyHarris3D::InitSelectedVertexLocation()
{
	// World 상의 Actor Transform
	actorLocation = GetActorLocation();
	actorScale = GetActorScale();
	actorRotation = GetActorRotation();
	
	// 선택된 Keypoint Location 및 Normal 저장
	for (int i = 0; i < vrts_postSelected.Num(); i++)
	{
		vrtLocs_postSelected.Push(myMesh.GetVertexLocByIndex(vrts_postSelected[i]));
		currentVrtLocs_postSelected.Push(myMesh.GetVertexLocByIndex(vrts_postSelected[i]));
		
		vrtNors_postSelected.Push (myMesh.GetVertexNorByIndex (vrts_postSelected[i]));
		currentVrtNors_postSelected.Push (myMesh.GetVertexNorByIndex (vrts_postSelected[i]));

		vrtTypes_postSelected.Push (myMesh.GetVertices ()[vrts_postSelected[i]].GetVertexType());
	}

	// for (int i = 0; i < vrts_unselected.Num(); i++)
	// {
	// 	vrtLocs_unselected.Push(myMesh.GetVertexLocByIndex(vrts_unselected[i]));
	// 	currentVrtLocs_unselected.Push(myMesh.GetVertexLocByIndex(vrts_unselected[i]));
		
	// 	vrtNors_unselected.Push (myMesh.GetVertexNorByIndex (vrts_unselected[i]));
	// 	currentVrtNors_unselected.Push (myMesh.GetVertexNorByIndex (vrts_unselected[i]));
	// }

	// World 상에 활용할 수 있도록 계산
	for (int i = 0; i < vrtLocs_postSelected.Num(); i++)
	{
		FVector offset = vrtLocs_postSelected[i];
		offset *= actorScale;
		offset = actorRotation.RotateVector(offset);
		
		currentVrtLocs_postSelected [i] = actorLocation + offset;
		currentVrtNors_postSelected [i] = actorRotation.RotateVector(vrtNors_postSelected [i]);
	}

	// for (int i = 0; i < vrtLocs_unselected.Num(); i++)
	// {
	// 	FVector offset = vrtLocs_unselected[i];
	// 	offset *= actorScale;
	// 	offset = actorRotation.RotateVector(offset);
		
	// 	currentVrtLocs_unselected [i] = actorLocation + offset;
	// 	currentVrtNors_unselected [i] = actorRotation.RotateVector(vrtNors_unselected [i]);
	// }
}


void AMyHarris3D::UpdateSelectedVertexLocation()
{
	FTimerHandle debugDrawWaitHandle;

	if (GetWorld())
	GetWorld()->GetTimerManager().SetTimer(debugDrawWaitHandle, FTimerDelegate::CreateLambda([&]()
	{
		for (int i = 0; i < vrtLocs_postSelected.Num(); i++)
		{
			FVector offset = vrtLocs_postSelected[i];
			offset *= actorScale;
			offset = actorRotation.RotateVector(offset);
			
			currentVrtLocs_postSelected [i] = actorLocation + offset;
			currentVrtNors_postSelected [i] = actorRotation.RotateVector(vrtNors_postSelected [i]);

			if (m_debugDraw == true && m_debugDraw_postSelected == true)
			{
				if (myMesh.GetVertices ()[vrts_postSelected[i]].GetVertexType() == EVertexType::VERTEX_BUMP)
					DrawDebugLine(GetWorld()
					, currentVrtLocs_postSelected[i], currentVrtLocs_postSelected[i]+4*currentVrtNors_postSelected[i]
					, FColorList::Red, false, DEBUG_DRAW_PERIOD, 0, 1);
				else if (myMesh.GetVertices ()[vrts_postSelected[i]].GetVertexType() == EVertexType::VERTEX_SINK)
					DrawDebugLine(GetWorld()
					, currentVrtLocs_postSelected[i], currentVrtLocs_postSelected[i]+4*currentVrtNors_postSelected[i]
					, FColorList::Green, false, DEBUG_DRAW_PERIOD, 0, 1);
				else if (myMesh.GetVertices ()[vrts_postSelected[i]].GetVertexType() == EVertexType::VERTEX_FLAT)
					DrawDebugLine(GetWorld()
					, currentVrtLocs_postSelected[i], currentVrtLocs_postSelected[i]+4*currentVrtNors_postSelected[i]
					, FColorList::Orange, false, DEBUG_DRAW_PERIOD, 0, 1);
				else
					DrawDebugLine(GetWorld()
					, currentVrtLocs_postSelected[i], currentVrtLocs_postSelected[i]+4*currentVrtNors_postSelected[i]
					, FColorList::Black, false, DEBUG_DRAW_PERIOD, 0, 1);
			}

		}

		// for (int i = 0; i < vrtLocs_unselected.Num(); i++)
		// {
		// 	FVector offset = vrtLocs_unselected[i];
		// 	offset *= actorScale;
		// 	offset = actorRotation.RotateVector(offset);
			
		// 	currentVrtLocs_unselected [i] = actorLocation + offset;
		// 	currentVrtNors_unselected [i] = actorRotation.RotateVector(vrtNors_unselected [i]);

		// 	if (m_debugDraw == true && m_debugDraw_unselected == true)
		// 		DrawDebugLine(GetWorld()
		// 			, currentVrtLocs_unselected[i], currentVrtLocs_unselected[i]+4*currentVrtNors_unselected[i]
		// 			, FColorList::Yellow, false, 0.1, 0, 1);
		// }

		actorLocation = GetActorLocation();
		actorScale = GetActorScale();
		actorRotation = GetActorRotation();

		GetWorld()->GetTimerManager().ClearTimer(debugDrawWaitHandle);
	}), DEBUG_DRAW_PERIOD, true); //반복도 여기서 추가 변수를 선언해 설정가능
}

FVector AMyHarris3D::GetVertexLocationByIndex (int i)
{
	if (i >= currentVrtLocs_postSelected.Num())
		return FVector (0, 0, 0);
		
	return  currentVrtLocs_postSelected [i];
}


FVector AMyHarris3D::GetVertexNormalByIndex (int i)
{
	if (i >= currentVrtLocs_postSelected.Num())
		return FVector (0, 0, 1);
		
	return  currentVrtNors_postSelected [i];
}
	
const TArray <FVector>& AMyHarris3D::GetKeyPointLocations () const
{
	return currentVrtLocs_postSelected;
}

const TArray <FVector>& AMyHarris3D::GetKeyPointNormals () const
{
	return currentVrtNors_postSelected;
}

const TArray <EVertexType>& AMyHarris3D::GetKeyPointTypes () const
{
	return vrtTypes_postSelected;
}