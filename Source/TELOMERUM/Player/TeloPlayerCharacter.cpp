// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TeloPlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Player/TeloLockOnComponent.h"
#include "Player/TeloInteractComponent.h"
#include "Enemy/TeloEnemyCharacter.h"
#include "UI/TeloUISubsystem.h"
#include "Engine/LocalPlayer.h"

// Sets default values
ATeloPlayerCharacter::ATeloPlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 캐릭터가 컨트롤러 회전에 따라 회전하지 않음
	bUseControllerRotationYaw = false;

	// 카메라 붐 생성 (캐릭터 뒤에 위치시키기 위함)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->SetRelativeLocation(FVector(0.0f, 0.0f, 75.0f)); // 캐릭터 머리 위쪽에 위치
	CameraBoom->bUsePawnControlRotation = true; // 컨트롤러 회전에 따라 회전

	// 카메라 렉
	CameraBoom->bEnableCameraLag = true;		// 카메라 렉 활성화
	CameraBoom->CameraLagSpeed = 8.0f;			// 카메라가 목표 위치로 따라오는 속도
	CameraBoom->CameraLagMaxDistance = 120.0f;	// 카메라가 목표 위치에서 최대 거리 제한 (0이면 무제한)

	// 회전 렉
	CameraBoom->bEnableCameraRotationLag = true;// 카메라 회전 렉 활성화
	CameraBoom->CameraRotationLagSpeed = 15.0f; // 카메라가 목표 회전으로 따라오는 속도

	// 팔로우 카메라 생성
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false; // 카메라가 컨트롤러 회전에 따라 회전하지 않음

	// Lock On 컴포넌트 생성
	LockOnComponent = CreateDefaultSubobject<UTeloLockOnComponent>(TEXT("LockOnComponent"));

	// Interact 컴포넌트 생성
	InteractComponent = CreateDefaultSubobject<UTeloInteractComponent>(TEXT("InteractComponent"));

	// 초기 상태 설정
	MaxHP = 100.0f;
	MoveSpeedScale = 1.5f;
	JumpPowerScale = 2.0f;

	// CharacterMovement 세팅
	GetCharacterMovement()->bOrientRotationToMovement = true;				// 캐릭터가 이동 방향에 따라 회전하도록 설정
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	GetCharacterMovement()->BrakingDecelerationFalling = 50.0f;				// 공중 감속력
	GetCharacterMovement()->AirControl = 0.7f;								// 공중 제어
	GetCharacterMovement()->GravityScale = 2.0f;							// 중력 배율

	GetCharacterMovement()->GroundFriction = 8.0f;							// 마찰력
	GetCharacterMovement()->BrakingDecelerationWalking = 2048.0f;			// 감속력

	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;	// 웅크리기 가능
	GetCharacterMovement()->bCanWalkOffLedgesWhenCrouching = true;			// 웅크리고 턱을 내려가기 가능

	GetCharacterMovement()->MaxWalkSpeed *= MoveSpeedScale;					// 이동 속도
	GetCharacterMovement()->JumpZVelocity *= JumpPowerScale;				// 점프 힘
}

// Called when the game starts or when spawned
void ATeloPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<APlayerController>(GetController());
	CameraBoomDefaultRelativeLocation = CameraBoom->GetRelativeLocation(); // 카메라 붐의 기본 상대 위치 저장

	// 컴포넌트 값 확인
	if (MoveAction == NULL)
		UE_LOG(LogTemp, Warning, TEXT("[ATeloPlayerCharacter] MoveAction is NULL"));
	if (LookAction == NULL)
		UE_LOG(LogTemp, Warning, TEXT("[ATeloPlayerCharacter] LookAction is NULL"));
	if (JumpAction == NULL)
		UE_LOG(LogTemp, Warning, TEXT("[ATeloPlayerCharacter] JumpAction is NULL"));
	if (CrouchAction == NULL)
		UE_LOG(LogTemp, Warning, TEXT("[ATeloPlayerCharacter] CrouchAction is NULL"));
	if (DashAction == NULL)
		UE_LOG(LogTemp, Warning, TEXT("[ATeloPlayerCharacter] DashAction is NULL"));
	if (AttackAction == NULL)
		UE_LOG(LogTemp, Warning, TEXT("[ATeloPlayerCharacter] AttackAction is NULL"));
	if (BlockAction == NULL)
		UE_LOG(LogTemp, Warning, TEXT("[ATeloPlayerCharacter] BlockAction is NULL"));
	if (LockOnAction == NULL)
		UE_LOG(LogTemp, Warning, TEXT("[ATeloPlayerCharacter] LockOnAction is NULL"));
	if (InteractAction == NULL)
		UE_LOG(LogTemp, Warning, TEXT("[ATeloPlayerCharacter] InteractAction is NULL"));
	if (InventoryAction == NULL)
		UE_LOG(LogTemp, Warning, TEXT("[ATeloPlayerCharacter] InventoryAction is NULL"));
}

// Called every frame
void ATeloPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

// Called to bind functionality to input
void ATeloPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATeloPlayerCharacter::MoveInput);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ATeloPlayerCharacter::MoveInputEnd);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATeloPlayerCharacter::LookInput);

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ATeloPlayerCharacter::DoJumpStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ATeloPlayerCharacter::DoJumpEnd);

		// Crouch
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &ATeloPlayerCharacter::DoCrouchStart);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &ATeloPlayerCharacter::DoCrouchEnd);

		// Dash
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, this, &ATeloPlayerCharacter::DoDashStart);

		// Attack
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ATeloPlayerCharacter::AttackInput);

		//// Block
		//EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Started, this, &ATeloPlayerCharacter::DoBlockStart);
		//EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Completed, this, &ATeloPlayerCharacter::DoBlockEnd);

		// Lock On
		EnhancedInputComponent->BindAction(LockOnAction, ETriggerEvent::Completed, this, &ATeloPlayerCharacter::DoLockOn);

		// Interact
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ATeloPlayerCharacter::InteractInput);

		// Inventory
		EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Started, this, &ATeloPlayerCharacter::InventoryInput);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[ATeloPlayerCharacter] No Enhanced Input Component!"));
	}
}

void ATeloPlayerCharacter::ResetMovementComps()
{
	GetCharacterMovement()->MaxWalkSpeed = 600.0f * MoveSpeedScale;	// 속도
	GetCharacterMovement()->GravityScale = 2.0f;					// 중력
	GetCharacterMovement()->GroundFriction = 8.0f;					// 마찰력
	GetCharacterMovement()->BrakingDecelerationWalking = 2048.0f;	// 감속력
	GetCharacterMovement()->BrakingDecelerationFalling = 50.0f;		// 공중 감속력
	GetCharacterMovement()->AirControl = 0.7f;						// 공중 제어
}

void ATeloPlayerCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	if (!bCanDash && !bIsDashing) // 대시 가능/대시 중이 아닐 시
	{
		GetWorldTimerManager().SetTimer(DashTimerHandle, this, &ATeloPlayerCharacter::DashCooldown, 0.5f, false);
	}

	bCanCrouch = true; // 점프/착지가 끝나면 앉기 가능
}

bool ATeloPlayerCharacter::CanJumpInternal_Implementation() const
{
	return JumpIsAllowedInternal(); // 앉기 시에도 점프 가능하도록 변경
}

void ATeloPlayerCharacter::MoveInput(const FInputActionValue& Value)
{
	InputVector = Value.Get<FVector2D>();

	DoMove(InputVector.X, InputVector.Y);
}

void ATeloPlayerCharacter::MoveInputEnd(const FInputActionValue& Value)
{
	InputVector = FVector2D::ZeroVector;
}

void ATeloPlayerCharacter::DoMove(float Right, float Forward)
{
	if (GetController())
	{
		if (IsAttacking()) return; // 공격 중일 시 이동 무시

		// 카메라의 Yaw 회전에 따른 이동 방향 설정
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, Forward * MoveSpeedScale);
		AddMovementInput(RightDirection, Right * MoveSpeedScale);
	}
}

void ATeloPlayerCharacter::LookInput(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void ATeloPlayerCharacter::DoLook(float Yaw, float Pitch)
{
	if (!GetController())
		return;

	// 락온 중 yaw 일부는 카메라 회전에 쓰고, 동시에 스냅 누적해서 전환
	if (LockOnComponent && LockOnComponent->IsLockOn())
	{
		// 전환 판정 (강한 스냅이면 true)
		const bool bSwitched = LockOnComponent->ConsumeYawForTargetSwitch(Yaw);

		// bSwitched가 true면 완전 고정 (0.0f), false면 설정된 비율만큼 허용
		const float AllowRatio = bSwitched ? 0.0f : LockOnYawAllowRatio;

		AddControllerYawInput(Yaw * AllowRatio);
		AddControllerPitchInput(Pitch);

		return;
	}

	// 락온 중이 아니면 평소대로
	AddControllerYawInput(Yaw);
	AddControllerPitchInput(Pitch);
}

void ATeloPlayerCharacter::DoJumpStart()
{
	bCanCrouch = false; // 점프 중에는 앉기 불가능
	DoAttackEnd();		// 점프 시 공격 강제종료
	DoCrouchEnd();		// 점프 시 앉기 강제종료

	Jump();
}

void ATeloPlayerCharacter::DoJumpEnd()
{
	StopJumping();
}

// 앉기 키 입력 시 방향키 입력이 없어도 캐릭터가 움직이고 있다면 슬라이딩
void ATeloPlayerCharacter::DoCrouchStart()
{
	if (!bCanCrouch) return; // 앉기 불가능 시 종료

	Crouch();
	//ApplyLockOnMovementMode(true); // 로코모션 해제

	// 카메라 붐의 Z 위치를 앉기 시 내려가는 만큼 올림
	if (CameraBoom)
	{
		FVector NewLocation = CameraBoomDefaultRelativeLocation;
		NewLocation.Z += CrouchCameraZOffset;
		CameraBoom->SetRelativeLocation(NewLocation);
	}

	if (!GetCharacterMovement()->Velocity.IsNearlyZero() && !GetCharacterMovement()->IsFalling()) // 정지/공중이 아닐 시 슬라이딩
	{
		FVector SlideDir = GetCharacterMovement()->Velocity.GetSafeNormal2D(); // XY벡터에서 방향만 추출
		FVector SlideImpulse = SlideDir * 800.0f * MoveSpeedScale;

		LaunchCharacter(SlideImpulse, true, false); // 수평 방향으로만 임펄스 적용

		GetCharacterMovement()->GroundFriction = 0.0f; // 마찰력 0
		GetCharacterMovement()->BrakingDecelerationWalking = 466.0f * MoveSpeedScale; // 감속력 감소
	}
}

void ATeloPlayerCharacter::DoCrouchEnd()
{
	UnCrouch();

	// 카메라 붐의 Z 위치를 기본 위치로 복구
	if (CameraBoom)
	{
		CameraBoom->SetRelativeLocation(CameraBoomDefaultRelativeLocation);
	}

	ResetMovementComps(); // 본래 마찰력/감속력 복구
	//ApplyLockOnMovementMode(false); // 로코모션 적용
}

// 현재 움직임과 상관없이 입력 값으로 대시
void ATeloPlayerCharacter::DoDashStart()
{
	if (!bCanDash || bIsDashing) return; // 대시 불가능/대시 중일 시 종료
	//if (InputVector.IsNearlyZero()) return; // 이동 입력이 없을 시 종료
	//if (GetCharacterMovement()->GetCurrentAcceleration().IsNearlyZero()) return; // 가속이 없을 시 종료 (입력 없을 시)
	
	DoAttackEnd(); // 대시 시 공격 강제종료

	bIsDashing = true;
	bCanDash = false;

	const FRotator Rotation = GetController()->GetControlRotation();	// 카메라의 Yaw 회전에 따른 대시 방향 설정
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);					// Yaw 회전만 사용하여 방향 계산 (Pitch는 무시)

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);	// 카메라의 정면 방향
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);		// 카메라의 오른쪽 방향

	FVector DashDir = (ForwardDirection * InputVector.Y + RightDirection * InputVector.X).GetSafeNormal2D(); // Right = InputVector.X, Forward = InputVector.Y

	if (DashDir.IsNearlyZero()) // 입력이 없을 시
	{
		if (LockOnComponent && LockOnComponent->IsLockOn() && LockOnComponent->GetTarget()) // 락온 중이고 타겟이 있을 시
		{
			FVector ToTarget = GetActorLocation() - LockOnComponent->GetTargetPointWorldLocation();
			ToTarget.Z = 0.0f;
			if (!ToTarget.IsNearlyZero())
			{
				const FRotator TargetYaw = ToTarget.Rotation();
				SetActorRotation(FRotator(0.f, TargetYaw.Yaw, 0.f)); // 타겟 반대 방향으로 회전
			}

			DashDir = (GetActorLocation() - LockOnComponent->GetTargetPointWorldLocation()).GetSafeNormal2D(); // 타겟 반대 방향으로 대시
		}
		else
		{
			DashDir = GetActorForwardVector(); // 전방 대시
		}
	}

	//ApplyLockOnMovementMode(true); // 대시 중 로코모션 해제

	GetCharacterMovement()->Velocity = FVector::ZeroVector;		// 이동 정지
	GetCharacterMovement()->GravityScale = 0.0f;				// 중력 0
	GetCharacterMovement()->GroundFriction = 0.0f;				// 마찰력 0
	GetCharacterMovement()->BrakingDecelerationWalking = 0.0f;	// 감속력 0
	GetCharacterMovement()->MaxWalkSpeed = 0.0f;				// 이동 속도 0

	LaunchCharacter(DashDir * 2000.0f, true, true); // 임펄스 적용

	GetWorldTimerManager().SetTimer(DashTimerHandle, this, &ATeloPlayerCharacter::DoDashEnd, 0.2f, false);
}

void ATeloPlayerCharacter::DoDashEnd()
{
	bIsDashing = false;

	GetCharacterMovement()->Velocity = FVector::ZeroVector;	// 대시 정지
	ResetMovementComps(); // 본래 속도/중력/마찰력/감속력 복구

	if (!GetCharacterMovement()->IsFalling()) // 점프 중이 아닐 시
	{
		GetWorldTimerManager().SetTimer(DashTimerHandle, this, &ATeloPlayerCharacter::DashCooldown, 0.5f, false);
	}

	//ApplyLockOnMovementMode(false);
}

void ATeloPlayerCharacter::DashCooldown()
{
	bCanDash = true;
}

void ATeloPlayerCharacter::DoLockOn()
{
	if (LockOnComponent)
	{
		LockOnComponent->ToggleLockOn();
	}
}

// 락온 시 잠시 로코모션 해제
//void ATeloPlayerCharacter::ApplyLockOnMovementMode(bool bLockOn)
//{
//	// 락온이 아닐 땐 리턴
//	if (!LockOnComponent || !LockOnComponent->IsLockOn())
//		return;
//
//	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
//	if (!MoveComp) return;
//
//	if (bLockOn) // 락온 중 특수한 동작 시
//	{
//		bUseControllerRotationYaw = false;				// 캐릭터가 컨트롤러 회전에 따라 회전하지 않음
//		MoveComp->bOrientRotationToMovement = true;		// 캐릭터가 이동 방향에 따라 회전하도록 설정
//	}
//	else // 락온 중 특수한 동작을 하지 않을 시
//	{
//		bUseControllerRotationYaw = true;				// 캐릭터가 컨트롤러 회전에 따라 회전
//		MoveComp->bOrientRotationToMovement = false;	// 캐릭터가 이동 방향에 따라 회전하지 않음
//	}
//}

void ATeloPlayerCharacter::AttackInput()
{
	AActor* Target = nullptr;

	if (LockOnComponent && LockOnComponent->IsLockOn())
	{
		Target = LockOnComponent->GetTarget();
	} 
	
	DoAttack(Target);
}

void ATeloPlayerCharacter::RotateToTarget(const AActor* Target)
{
	if (LockOnComponent && LockOnComponent->IsLockOn())
	{
		if (LockOnComponent->GetTarget())
		{
			Super::RotateToTarget(Target);
		}
	}
}

void ATeloPlayerCharacter::InteractInput()
{
	if (InteractComponent)
	{
		InteractComponent->TryInteract();
	}
}

void ATeloPlayerCharacter::InventoryInput()
{
	if (!PlayerController)
	{
		return;
	}

	ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	if (!LocalPlayer)
	{
		return;
	}

	UTeloUISubsystem* UISubsystem = LocalPlayer->GetSubsystem<UTeloUISubsystem>();
	if (!UISubsystem)
	{
		return;
	}

	UISubsystem->ToggleInventory();
}