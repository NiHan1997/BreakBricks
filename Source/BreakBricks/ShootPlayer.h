// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Brick.h"
#include "Bullet.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"

#include "ShootPlayer.generated.h"

UCLASS()
class BREAKBRICKS_API AShootPlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AShootPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(EditAnywhere)
	USpringArmComponent* SpringArmComponent;
	
	UPROPERTY(EditAnywhere)
	UCameraComponent* CameraComponent;

private:
	// 蓝图的砖块物体.
	TSubclassOf<ABrick> BlueprintBrick;

	// 蓝图的子弹物体.
	TSubclassOf<ABullet> BlueprintBullet;

private:
	// 加载砖块蓝图.
	void LoadBrickBlueprint();

	// 加载子弹蓝图.
	void LoadBulletBlueprint();

	// 生成墙壁.
	void CreateBrickWall() const;

	// 发射子弹.
	void ShootBullet();
};
