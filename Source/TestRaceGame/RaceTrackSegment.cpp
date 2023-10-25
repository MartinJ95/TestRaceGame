// Fill out your copyright notice in the Description page of Project Settings.

#include "RaceTrackSegment.h"
#include "Math/UnrealMathUtility.h"
#include "DrawDebugHelpers.h"

unsigned int ARaceTrackSegment::currentAmount = 0;

// Sets default values
ARaceTrackSegment::ARaceTrackSegment() : m_segmentMesh(), m_segmentDynamicMesh(),
m_meshComp(), m_startPoint(0.f, 0.f, 0.f), m_controlPoint(0.f, 0.f, 0.f), m_endPoint(0.f, 0.f, 0.f),
m_endPointRotation(0.f, 0.f, 0.f), m_materialInstance(), m_segmentID(0)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	currentAmount++;
	m_segmentID = currentAmount;
	FName name = *FString::Printf(TEXT("segmentMeshComp %i"), m_segmentID);
	FName name2 = *FString::Printf(TEXT("segmentMesh %i"), m_segmentID);
	m_meshComp = CreateDefaultSubobject<UStaticMeshComponent>(name);
	//m_segmentDynamicMesh = CreateDefaultSubobject<UStaticMesh>(name2);
	m_meshComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

void ARaceTrackSegment::SetMesh(UStaticMesh* newMesh, UMaterialInterface* material)
{
	m_segmentMesh = newMesh;
	if (m_segmentMesh == nullptr)
		return;
/*
	FStaticMeshSourceModel* sourceModel = &m_segmentMesh->SourceModels[0];
	FRawMesh rawMesh;
	sourceModel->LoadRawMesh(rawMesh);

	// Create a new model
	//m_segmentDynamicMesh = NewObject<UStaticMesh>();
	new(m_segmentDynamicMesh->SourceModels) FStaticMeshSourceModel();
	m_segmentDynamicMesh->SourceModels[0].SaveRawMesh(rawMesh);

	TArray<FText> BuildErrorsNuevo;
	m_segmentDynamicMesh->Build(true, &BuildErrorsNuevo);
	*/
	m_segmentDynamicMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("/TestRaceGame/Content/pathSegment")));
	m_meshComp->SetStaticMesh(m_segmentMesh);
	SetDynamicInstance(material);

}

void ARaceTrackSegment::SetPositioning(const ARaceTrackSegment* previous)
{
	SetStartPosition(previous);
	SetEndPointRotation(previous);
	float sideDir;
	SetEndPoint(previous, sideDir);
	SetControlPoint(sideDir);
	UpdateDynamicInstance();
}

inline void ARaceTrackSegment::DrawDebugCurve() const
{
	DrawDebugSphere(GetWorld(), m_startPoint, 100.f, 16, FColor::Green, false, 0.1f);
	DrawDebugSphere(GetWorld(), m_endPoint, 100.f, 16, FColor::Red, false, 0.1f);
	DrawDebugSphere(GetWorld(), m_controlPoint, 100.f, 16, FColor::Yellow, false, 0.1f);
	for (float i = 0.1; i < 1.f; i += 0.1f)
	{
		DrawDebugSphere(GetWorld(), GetPosition(i), 50.f, 16, FColor::Purple, false, 0.1f);
	}
}

// Called when the game starts or when spawned
void ARaceTrackSegment::BeginPlay()
{
	Super::BeginPlay();
	
}

inline void ARaceTrackSegment::SetStartPosition(const ARaceTrackSegment* previous)
{
	if (previous == nullptr)
	{
		m_startPoint = FVector::ZeroVector;
		m_startPointRotation = FVector::ZeroVector;
		return;
	}
	m_startPoint = previous->m_endPoint;
	m_startPointRotation = previous->m_startPointRotation;
}

inline void ARaceTrackSegment::SetEndPointRotation(const ARaceTrackSegment* previous)
{
	if (previous == nullptr)
	{
		m_endPointRotation = FVector(0.f, FMath::FRandRange(-30.f, 30.f), FMath::FRandRange(-30.f, 30.f));
		return;
	}
	m_endPointRotation = previous->m_endPointRotation + FVector(0.f, FMath::FRandRange(-30.f, 30.f), FMath::FRandRange(-30.f, 30.f));
}

inline void ARaceTrackSegment::SetEndPoint(const ARaceTrackSegment* previous, float& sideDir)
{
	FVector startRotation = FVector::ZeroVector;
	if (previous != nullptr)
		startRotation = previous->m_endPointRotation;
	m_endPoint = m_startPoint + (FVector::ForwardVector * ((m_segmentMesh->GetBounds().SphereRadius * 2) * (1.f-((m_endPointRotation.Z - m_startPointRotation.Z) / 30.f))));
	
	FVector rotatedStartDir = FVector::ForwardVector;
	rotatedStartDir.RotateAngleAxis(startRotation.Y, FVector::RightVector);
	rotatedStartDir.RotateAngleAxis(startRotation.Z, FVector::UpVector);
	sideDir = m_endPointRotation.Z - startRotation.Z > 0 ? 1.f : -1.f;

	m_endPoint += (FVector::CrossProduct(m_endPoint - m_startPoint, FVector::UpVector).GetSafeNormal()) * ((m_segmentMesh->GetBounds().SphereRadius*2) * (FMath::Abs(sideDir) *((m_endPointRotation.Z - m_startPointRotation.Z)/30.f)));
}

inline void ARaceTrackSegment::SetControlPoint(const float& sideDir)
{
	FVector toEnd = m_endPoint - m_startPoint;
	toEnd *= 0.5;
	float distance = toEnd.Size()*0.5f;
	toEnd += (FVector::CrossProduct(toEnd, FVector::UpVector).GetSafeNormal() * (sideDir * 45.f));
	m_controlPoint = m_startPoint + toEnd;
}

inline void ARaceTrackSegment::SetDynamicInstance(UMaterialInterface* material)
{
	m_materialInstance = UMaterialInstanceDynamic::Create(material, this);
	m_meshComp->GetStaticMesh()->SetMaterial(0, m_materialInstance);
	//m_meshComp->SetMaterial(0, m_materialInstance);
}

inline void ARaceTrackSegment::UpdateDynamicInstance()
{
	if (m_materialInstance == nullptr)
		return;

	m_materialInstance->SetVectorParameterValue(TEXT("StartPosition"), m_startPoint);
	m_materialInstance->SetVectorParameterValue(TEXT("EndPosition"), m_endPoint);
	m_materialInstance->SetVectorParameterValue(TEXT("ControlPosition"), m_controlPoint);
	m_materialInstance->SetScalarParameterValue(TEXT("Size"), m_meshComp->GetStaticMesh()->GetBounds().SphereRadius * 2);
}

inline FVector ARaceTrackSegment::GetPosition(float& t) const
{
	if (t > 1.f)
	{
		t -= 1.f;
		return m_endPoint;
	}
	if (t <= 0)
	{
		return m_startPoint;
	}
	return (FMath::Square(1.f - t)) * m_startPoint + 2 * t * (1.f - t) * m_controlPoint + FMath::Square(t) * m_endPoint;
}

inline void ARaceTrackSegment::SetID(const int id)
{
	m_segmentID = id;
}

ARaceTrackSegment::~ARaceTrackSegment()
{
	currentAmount--;
}

// Called every frame
void ARaceTrackSegment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

