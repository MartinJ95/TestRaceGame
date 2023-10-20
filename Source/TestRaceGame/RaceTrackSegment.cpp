// Fill out your copyright notice in the Description page of Project Settings.

#include "RaceTrackSegment.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
ARaceTrackSegment::ARaceTrackSegment()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("segmentMesh"));
	m_meshComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

void ARaceTrackSegment::SetMesh(UStaticMesh* newMesh, UMaterialInterface* material)
{
	m_segmentMesh = newMesh;
	if (m_segmentMesh == nullptr)
		return;

	m_meshComp->SetStaticMesh(m_segmentMesh);
	SetDynamicInstance(material);
}

void ARaceTrackSegment::SetPositioning(ARaceTrackSegment* previous)
{
	SetStartPosition(previous);
}

// Called when the game starts or when spawned
void ARaceTrackSegment::BeginPlay()
{
	Super::BeginPlay();
	
}

inline void ARaceTrackSegment::SetStartPosition(ARaceTrackSegment* previous)
{
	if (previous == nullptr)
	{
		m_startPoint = FVector::ZeroVector;
		return;
	}
	m_startPoint = previous->m_endPoint;
}

inline void ARaceTrackSegment::SetEndPointRotation(ARaceTrackSegment* previous)
{
	if (previous == nullptr)
	{
		m_endPointRotation = FVector(0.f, FMath::FRandRange(-30.f, 30.f), FMath::FRandRange(-30.f, 30.f));
		return;
	}
	m_endPointRotation = previous->m_endPointRotation + FVector(0.f, FMath::FRandRange(-30.f, 30.f), FMath::FRandRange(-30.f, 30.f));
}

inline void ARaceTrackSegment::SetEndPoint(ARaceTrackSegment* previous, float& sideDir)
{
	FVector startRotation = FVector::ZeroVector;
	if (previous != nullptr)
		startRotation = previous->m_endPointRotation;
	m_endPoint = m_startPoint + (FVector::ForwardVector * (m_segmentMesh->GetBounds().SphereRadius * 2) * (1.f-(m_endPointRotation.Z / 30.f)));
	
	FVector rotatedStartDir = FVector::ForwardVector;
	rotatedStartDir.RotateAngleAxis(startRotation.Y, FVector::RightVector);
	rotatedStartDir.RotateAngleAxis(startRotation.Z, FVector::UpVector);
	sideDir = m_endPointRotation.Z - startRotation.Z > 0 ? 1.f : -1.f;

	m_endPoint += (FVector::CrossProduct(m_endPoint - m_startPoint, FVector::UpVector).GetSafeNormal()) * (FMath::Abs(sideDir) * m_endPointRotation.Z);
}

inline void ARaceTrackSegment::SetControlPoint(float& sideDir)
{
	FVector toEnd = m_endPoint - m_startPoint;
	toEnd *= 0.5;
	float distance = toEnd.Size()*0.5f;
	toEnd += (FVector::CrossProduct(m_startPoint, m_endPoint).GetSafeNormal() * (sideDir * 45.f));
	m_controlPoint = toEnd;
}

inline void ARaceTrackSegment::SetDynamicInstance(UMaterialInterface* material)
{
	m_materialInstance = UMaterialInstanceDynamic::Create(material, this);
}

inline void ARaceTrackSegment::UpdateDynamicInstance()
{
	if (m_materialInstance == nullptr)
		return;

	m_materialInstance->SetVectorParameterValue(TEXT("start"), m_startPoint);
	m_materialInstance->SetVectorParameterValue(TEXT("end"), m_endPoint);
	m_materialInstance->SetVectorParameterValue(TEXT("control"), m_controlPoint);
	m_materialInstance->SetScalarParameterValue(TEXT("size"), m_meshComp->GetStaticMesh()->GetBounds().SphereRadius * 2);
}

inline FVector ARaceTrackSegment::GetPosition(float& t)
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

// Called every frame
void ARaceTrackSegment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

