// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHarris3D_Spline.h"

using namespace std;

// Sets default values
AMyHarris3D_Spline::AMyHarris3D_Spline()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AMyHarris3D_Spline::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	cout << "spline OnConstruction" << endl;
	
	if (m_update_first == false || m_update_click == true)
	{
		if (m_update_first == false)
			cout << "first init" << endl;
		if (m_update_click == true)
			cout << "click to init" << endl;
		
		m_update_first = true;
		m_update_click = false;
		
		InitializeHarris3DSpline ();
	}
}

void AMyHarris3D_Spline::InitializeHarris3DSpline_Implementation() {}

// Called when the game starts or when spawned
void AMyHarris3D_Spline::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyHarris3D_Spline::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

