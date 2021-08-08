// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootPlayer.h"

#include "DrawDebugHelpers.h"
#include "Components/LineBatchComponent.h"

// Sets default values
AShootPlayer::AShootPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));

	SpringArmComponent->SetupAttachment(RootComponent);
	CameraComponent->SetupAttachment(SpringArmComponent);
	SpringArmComponent->TargetArmLength = 2000;
	SpringArmComponent->SetRelativeLocation(FVector(-100.0, 0, 400));

	// 加载操作要在构造函数中执行.
	LoadBrickBlueprint();
	LoadBulletBlueprint();
}

// Called when the game starts or when spawned
void AShootPlayer::BeginPlay()
{
	Super::BeginPlay();

	CreateBrickWall();
}

// Called every frame
void AShootPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AShootPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("ShootBullet", IE_Pressed, this, &AShootPlayer::ShootBullet);
}

void AShootPlayer::LoadBrickBlueprint()
{
	ConstructorHelpers::FClassFinder<AActor> BP_Brick(TEXT("Blueprint'/Game/Blueprints/BP_Brick.BP_Brick_C'"));
	if (BP_Brick.Succeeded())
	{
		BlueprintBrick = BP_Brick.Class;
	}
}

void AShootPlayer::LoadBulletBlueprint()
{
	ConstructorHelpers::FClassFinder<AActor> BP_Bullet(TEXT("Blueprint'/Game/Blueprints/BP_Bullet.BP_Bullet_C'"));
	if (BP_Bullet.Succeeded())
	{
		BlueprintBullet = BP_Bullet.Class;
	}
}

void AShootPlayer::CreateBrickWall() const
{
	for (int i = 0; i < 10; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			ABrick* Brick = GetWorld()->SpawnActor<ABrick>(BlueprintBrick, FVector(0, (i - 5) * 100, 5 + 100 * j),
				FRotator(0, 0, 0));
			Brick->SetFolderPath("Bricks");
		}		
	}
}

void AShootPlayer::ShootBullet()
{
	FVector CameraLocation;
	FRotator CameraRotation;
	Controller->GetPlayerViewPoint(CameraLocation, CameraRotation);					
	
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController != nullptr)
	{
		ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
		if (LocalPlayer != nullptr && LocalPlayer->ViewportClient != nullptr)
		{
			FVector Dir, Point;
			if (PlayerController->DeprojectMousePositionToWorld(Point, Dir))
			{
				const FVector TraceStart = CameraLocation;
				const FVector Direction = CameraRotation.Vector();

				const FVector TraceEnd = TraceStart + Dir * 10000;

				//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, true, 0.0f, 0.0f, 10.0f);

				/*
				FCollisionQueryParams TraceParams(FName(TEXT("Actor")), true, this);
				TraceParams.bTraceComplex = true;
				TraceParams.bReturnPhysicalMaterial = false;

				FHitResult Hit(EForceInit::ForceInit);
				GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, TraceParams);
				if (Cast<AActor>(Hit.GetActor()))
				{					
					UE_LOG(LogTemp, Warning, TEXT("射线检测: %s"), *Hit.GetActor()->GetName());
				}
				*/

				ABullet* Bullet = GetWorld()->SpawnActor<ABullet>(BlueprintBullet, TraceStart,
						FRotator(0, 0, 0));
				Bullet->SetFolderPath("Bullets");
				Bullet->AddForceToBullet(TraceEnd - TraceStart, 100000);
			}
		}
	}
}

