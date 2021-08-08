// Fill out your copyright notice in the Description page of Project Settings.


#include "Brick.h"

// Sets default values
ABrick::ABrick()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));

	StaticMeshComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABrick::BeginPlay()
{
	Super::BeginPlay();

	// 获取该物体引用的材质.
	TArray<UMaterialInterface*> OutMaterials;
	StaticMeshComponent->GetUsedMaterials(OutMaterials);

	// 实例化材质, 并对每个材质设置不同的随机属性, 在这里确定只有一个材质.
	if (OutMaterials[0] != nullptr)
	{
		UMaterial* Mat = OutMaterials[0]->GetMaterial();
		if (UMaterialInstanceDynamic* MatInstance = UMaterialInstanceDynamic::Create(Mat, this))
		{
			MatInstance->SetVectorParameterValue(TEXT("Color"), FLinearColor(GetRandomFloat(), GetRandomFloat(), GetRandomFloat(), 0));
			StaticMeshComponent->SetMaterial(0, MatInstance);
		}
	}
}

// Called every frame
void ABrick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 实例的砖块在落下地面 10 米之后自动销毁.
	if (StaticMeshComponent->GetComponentLocation().Z <= -1000)
	{
		Destroy();
	}
}

float ABrick::GetRandomFloat() const
{
	FRandomStream Stream;
	Stream.GenerateNewSeed();
	return Stream.FRandRange(0.0f, 1.0f);
}

