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

// 에디터에서 수행할 수 있도록 설정
/*void AMyHarris3D::PostInitializeComponents ()
{
	Super::PostInitializeComponents();
	
	cout << "function called" << endl;
}*/

/*

void AMyHarris3D::PostActorCreated ()
{
	Super::PostActorCreated ();

	cout << "function called created" << endl;
}*/

/*void AMyHarris3D::PostLoad ()
{
	Super::PostLoad ();
	cout << "function called loaded" << endl;
	
	InitFlowChart ();
}

void AMyHarris3D::PostInitProperties ()
{
	Super::PostInitProperties ();
	cout << "function called init" << endl;
	
	InitFlowChart ();
}

void AMyHarris3D::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	cout << "function called modify" << endl;
	
	InitFlowChart ();
}*/

// Construction
void AMyHarris3D::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	// 첫 실행 혹은 에디터에서 갱신 bool를 활성화할 시
	if (m_update_first == false || m_update_click == true)
	{
		// test
		/*if (m_update_first == false)
			cout << "first init" << endl;
		if (m_update_click == true)
			cout << "click to init" << endl;*/
		
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
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

	vrts_selected.clear();
	
	vrts_postSelected.Empty();
	vrtLocs_postSelected.Empty();
	vrtNors_postSelected.Empty();
	currentVrtLocs_postSelected.Empty();
	currentVrtNors_postSelected.Empty();
	vrtTypes_postSelected.Empty();

	vrts_unselected.Empty();
	vrtLocs_unselected.Empty();
	vrtNors_unselected.Empty();
	currentVrtLocs_unselected.Empty();
	currentVrtNors_unselected.Empty();

	vrts_overlapped.Empty();
	vrtLocs_overlapped.Empty();
	vrtNors_overlapped.Empty();
	currentVrtLocs_overlapped.Empty();
	currentVrtNors_overlapped.Empty();

	harrisRPoints.clear();

	InitMyHarris3D();
	CalculateHarrisResponse();
	CalculateNMS ();
	CalculateVertexType ();

	InitSelectedVertexLocation ();
	UpdateSelectedVertexLocation();
}

// Called when the game starts or when spawned
void AMyHarris3D::BeginPlay()
{
	Super::BeginPlay();
	
	//InitFlowChart ();

	//InitSelectedVertexLocation ();
	//UpdateSelectedVertexLocation();

	UpdateSelectedVertexLocation();
}

// Called every frame
void AMyHarris3D::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMyHarris3D::InitMyHarris3D()
{
	//AMyHarris3D::myMesh = msh;
	//AMyHarris3D::typeSelection = (int)m_type;

	// 링 사이즈
	AMyHarris3D::ringSize = m_ringSize;

	// 전체 버택스 비율로 내어 선택할 개수를 구할 때 사용되는 상수
	AMyHarris3D::fraction_constant = m_fraction;

	
	AMyHarris3D::k_parameter = m_k;
}


//calculates the Harris reponse of each vertex
void AMyHarris3D::CalculateHarrisResponse()
{
	int vertexSize = myMesh.vertices.size();
	
	for (int indexVertex = 0; indexVertex < vertexSize; indexVertex++)
	{
		//vertexSize

		// 중복인 경우 계산하지 않고 컨티뉴
		if (indexVertex != myMesh.overlappingVert[indexVertex])
		{
			//harrisRPoints.push_back(harrisRPoints[myMesh.overlappingVert[indexVertex]]);
			harrisRPoints.push_back(1000);
			continue;
		}

		vector<double> x_coord, y_coord, z_coord;
		//caculate the neighbourhood
		set<int> set_nhd;

		//calculate the k rings neighbourhood of each vertex
		set_nhd = myMesh.CalculateNeighbourhood_Ring(indexVertex, ringSize);

		set<int>::iterator itr;
		for (itr = set_nhd.begin(); itr != set_nhd.end(); ++itr)
		{
			//get the x,y,z coordinates
			x_coord.push_back(myMesh.vertices[*itr].GetX());
			y_coord.push_back(myMesh.vertices[*itr].GetY());
			z_coord.push_back(myMesh.vertices[*itr].GetZ());
		}

		//adding the vertex itself to the set, the last element
		x_coord.push_back(myMesh.vertices[indexVertex].GetX());
		y_coord.push_back(myMesh.vertices[indexVertex].GetY());
		z_coord.push_back(myMesh.vertices[indexVertex].GetZ());


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
	// 주변 이웃한 버텍스 수가 가장 많은 버텍스
	vector<int> preselected;
	for (int nV = 0; nV < vertexSize; nV++)
	{
		// 중복 패스
		if (nV != myMesh.overlappingVert[nV])
		{
			continue;
		}
		
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

		vector<int> _selectedVertices;

		double maxi(0);
		for (int iIP = 0; iIP < preSelectedVertexes.size(); iIP++)
		{
			maxi = preSelectedHarrisValues.maxCoeff();
			for (int i = 0; i < preSelectedVertexes.size(); i++)
			{
				if (abs(maxi - preSelectedHarrisValues(i)) < 0.00001)
				{
					_selectedVertices.push_back(preSelectedVertexes(i));
					preSelectedHarrisValues(i) = 0;
					break;
				}
			}
		}

		//sort the preselected vertices, decreasing order
		sort(preselected.rbegin(), preselected.rend());

		vrts_selected = _selectedVertices;
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
	vrts_unselected.Empty();
	vrts_overlapped.Empty();
	
	while (vrts_selected.size() > 1)
	{
		//int index_current = 0;
		//int index_best = 0;
		
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
				
				//index_best = index_current;
			}

			//index_current++;
		}

		// 베스트 추가
		vrts_postSelected.Add(vrt_best);

		// 베스트 제외
		vrts_selected.erase(find (vrts_selected.begin(), vrts_selected.end(), vrt_best));
		
		// 필터링
		vector <int> filtering;
		
		FVector loc_best = myMesh.GetVertexLocByIndex(vrt_best);
		FVector nor_best = myMesh.GetVertexNorByIndex(vrt_best);

		iter = vrts_selected.begin();
		for (; iter != vrts_selected.end(); ++iter)
		{
			if (vrt_best ==  *iter)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("No Way"));
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
		iter = filtering.begin();
		for (; iter != filtering.end(); ++iter)
			vrts_unselected.Remove(*iter);
		
		//vrts_unselected.insert(vrts_unselected.end(), filtering.begin(), filtering.end());

		
		for (int index = 0; index < filtering.size(); index++)
		{
			vrts_selected.erase(find (vrts_selected.begin(), vrts_selected.end(), filtering[index]));
		}

	}

	// 남은거 마저 넣기
	for (int index = 0; index < vrts_selected.size(); index++)
	{
		vrts_postSelected.Add(vrts_selected[index]);
	}
	
	// 오버랩 확인
	for (int i = 0; i < myMesh.overlappingVert.size(); i++)
	{
		if (i != myMesh.overlappingVert[i])
			vrts_overlapped.Add(i);
	}

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Total Vertex Number: " + FString::FromInt(myMesh.vertices.size())));
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Overlapped Vertex Number: " + FString::FromInt(vrts_overlapped.Num())));
}

void AMyHarris3D::CalculateVertexType()
{
	// 선택된 keypoint 돌출 / 함몰 계산
	for (int index = 0; index < vrts_postSelected.Num(); index++)
	{
		myMesh.vertices[vrts_postSelected[index]].CalculateVertexType(&myMesh, m_bumpSink_ring, m_bumpSink_dot_flat);
	}
}


//To check whether a vertex is a local maximum or not
bool AMyHarris3D::GetIsLocalMaxima(unsigned int vertexIndex)
{
	set<int> nhd = myMesh.vertices[vertexIndex].GetNeighbours();
	set<int>::iterator itrr;
	for (itrr = nhd.begin(); itrr != nhd.end(); ++itrr)
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
	actorLocation = GetActorLocation();
	actorScale = GetActorScale();
	actorRotation = GetActorRotation();
	
	// 선택된 점 위치 확인
	for (int i = 0; i < vrts_postSelected.Num(); i++)
	{
		vrtLocs_postSelected.Push(myMesh.GetVertexLocByIndex(vrts_postSelected[i]));
		currentVrtLocs_postSelected.Push(myMesh.GetVertexLocByIndex(vrts_postSelected[i]));
		
		vrtNors_postSelected.Push (myMesh.GetVertexNorByIndex (vrts_postSelected[i]));
		currentVrtNors_postSelected.Push (myMesh.GetVertexNorByIndex (vrts_postSelected[i]));

		vrtTypes_postSelected.Push (myMesh.vertices[vrts_postSelected[i]].GetVertexType());
	}

	for (int i = 0; i < vrts_unselected.Num(); i++)
	{
		vrtLocs_unselected.Push(myMesh.GetVertexLocByIndex(vrts_unselected[i]));
		currentVrtLocs_unselected.Push(myMesh.GetVertexLocByIndex(vrts_unselected[i]));
		
		vrtNors_unselected.Push (myMesh.GetVertexNorByIndex (vrts_unselected[i]));
		currentVrtNors_unselected.Push (myMesh.GetVertexNorByIndex (vrts_unselected[i]));
	}

	/*for (int i = 0; i < vrts_overlapped.Num(); i++)
	{
		vrtLocs_overlapped.Push(myMesh.GetVertexLocByIndex(vrts_overlapped[i]));
		currentVrtLocs_overlapped.Push(myMesh.GetVertexLocByIndex(vrts_overlapped[i]));
		
		vrtNors_overlapped.Push (myMesh.GetVertexNorByIndex (vrts_overlapped[i]));
		currentVrtNors_overlapped.Push (myMesh.GetVertexNorByIndex (vrts_overlapped[i]));
	}*/

	for (int i = 0; i < vrtLocs_postSelected.Num(); i++)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, TEXT("OHa"));

		FVector offset = vrtLocs_postSelected[i];
		offset *= actorScale;
		offset = actorRotation.RotateVector(offset);
		
		currentVrtLocs_postSelected [i] = actorLocation + offset;
		currentVrtNors_postSelected [i] = actorRotation.RotateVector(vrtNors_postSelected [i]);
	}

	for (int i = 0; i < vrtLocs_unselected.Num(); i++)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, TEXT("OHa"));

		FVector offset = vrtLocs_unselected[i];
		offset *= actorScale;
		offset = actorRotation.RotateVector(offset);
		
		currentVrtLocs_unselected [i] = actorLocation + offset;
		currentVrtNors_unselected [i] = actorRotation.RotateVector(vrtNors_unselected [i]);
	}

	/*for (int i = 0; i < selectedVrts_clustering.size(); i++)
	{
		selectedVrtLocs_clustering.Push(myMesh.GetVertexLocByIndex(selectedVrts_clustering[i]));
		currentSelectedVrtLocs_clustering.Push(myMesh.GetVertexLocByIndex(selectedVrts_clustering[i]));

		selectedVrtNors_clustering.Push (myMesh.GetVertexNorByIndex (selectedVrts_clustering[i]));
		currentSelectedVrtNors_clustering.Push (myMesh.GetVertexNorByIndex (selectedVrts_clustering[i]));
	}*/

}


void AMyHarris3D::UpdateSelectedVertexLocation()
{
	//cout << vrtLocs_postSelected.Num() << endl;
	
	FTimerHandle debugDrawWaitHandle;
	
	float WaitTime = 0.1; //시간 설정
	//bool  firstInit =  false;

	if (GetWorld())
	GetWorld()->GetTimerManager().SetTimer(debugDrawWaitHandle, FTimerDelegate::CreateLambda([&]()
	{
		//if (firstInit == true)
		//{
			for (int i = 0; i < vrtLocs_postSelected.Num(); i++)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, TEXT("OHa"));

				FVector offset = vrtLocs_postSelected[i];
				offset *= actorScale;
				offset = actorRotation.RotateVector(offset);
				
				currentVrtLocs_postSelected [i] = actorLocation + offset;
				currentVrtNors_postSelected [i] = actorRotation.RotateVector(vrtNors_postSelected [i]);

				if (m_debugDraw == true && m_debugDraw_postSelected == true)
				{
					if (myMesh.vertices[vrts_postSelected[i]].GetVertexType() == EVertexType::VERTEX_BUMP)
						DrawDebugLine(GetWorld()
						, currentVrtLocs_postSelected[i], currentVrtLocs_postSelected[i]+4*currentVrtNors_postSelected[i]
						, FColorList::Red, false, 0.1, 0, 1);
					else if (myMesh.vertices[vrts_postSelected[i]].GetVertexType() == EVertexType::VERTEX_SINK)
						DrawDebugLine(GetWorld()
						, currentVrtLocs_postSelected[i], currentVrtLocs_postSelected[i]+4*currentVrtNors_postSelected[i]
						, FColorList::Green, false, 0.1, 0, 1);
					else if (myMesh.vertices[vrts_postSelected[i]].GetVertexType() == EVertexType::VERTEX_FLAT)
						DrawDebugLine(GetWorld()
						, currentVrtLocs_postSelected[i], currentVrtLocs_postSelected[i]+4*currentVrtNors_postSelected[i]
						, FColorList::Orange, false, 0.1, 0, 1);
					else
						DrawDebugLine(GetWorld()
						, currentVrtLocs_postSelected[i], currentVrtLocs_postSelected[i]+4*currentVrtNors_postSelected[i]
						, FColorList::Black, false, 0.1, 0, 1);
				}

			}

			for (int i = 0; i < vrtLocs_unselected.Num(); i++)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, TEXT("OHa"));

				FVector offset = vrtLocs_unselected[i];
				offset *= actorScale;
				offset = actorRotation.RotateVector(offset);
				
				currentVrtLocs_unselected [i] = actorLocation + offset;
				currentVrtNors_unselected [i] = actorRotation.RotateVector(vrtNors_unselected [i]);

				if (m_debugDraw == true && m_debugDraw_unselected == true)
					DrawDebugLine(GetWorld()
						, currentVrtLocs_unselected[i], currentVrtLocs_unselected[i]+4*currentVrtNors_unselected[i]
						, FColorList::Yellow, false, 0.1, 0, 1);
			}

			/*for (int i = 0; i < vrts_overlapped.size(); i++)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, TEXT("OHa"));

				FVector offset = vrtLocs_overlapped[i];
				offset *= actorScale;
				offset = actorRotation.RotateVector(offset);
				
				currentVrtLocs_overlapped [i] = actorLocation + offset;
				currentVrtNors_overlapped [i] = actorRotation.RotateVector(vrtNors_overlapped [i]);

				//if (m_debugDraw == true)
					DrawDebugLine(GetWorld()
						, currentVrtLocs_overlapped[i], currentVrtLocs_overlapped[i]+10*currentVrtNors_overlapped[i]
						, FColorList::Blue, false, 0.1, 0, 1);
			}*/

			/*for (int i = 0; i < selectedVrtLocs_clustering.Num(); i++)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, TEXT("OHa"));

				FVector offset = selectedVrtLocs_clustering[i];
				offset *= actorScale;
				offset = actorRotation.RotateVector(offset);

				currentSelectedVrtLocs_clustering [i] = actorLocation + offset;
				currentSelectedVrtNors_clustering [i] = actorRotation.RotateVector(selectedVrtNors_clustering [i]);
			

				//if (m_debugDraw == true)
				//DrawDebugSphere(GetWorld(),  currentSelectedVrtLocs_clustering [i], m_radius, 8, FColorList::Blue, false, .1f, 0, 01);
			}*/

			actorLocation = GetActorLocation();
			actorScale = GetActorScale();
			actorRotation = GetActorRotation();
		//}/
		//else
		//{
		//	firstInit = true;
		//}

		GetWorld()->GetTimerManager().ClearTimer(debugDrawWaitHandle);
	}), WaitTime, true); //반복도 여기서 추가 변수를 선언해 설정가능
}

/*EVertexType AMyHarris3D::GetVertexTypeByIndex(int index)
{
	if (index < 0 || index >= vrts_postSelected.Num())
		return EVertexType::NONE;

	return vrtTypes_postSelected [index];
}*/

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
	
