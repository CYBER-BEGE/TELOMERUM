// Fill out your copyright notice in the Description page of Project Settings.


#include "TeloPlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "TeloLockOnComponent.h"

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

	// 팔로우 카메라 생성
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false; // 카메라가 컨트롤러 회전에 따라 회전하지 않음

	// Lock On 컴포넌트 생성
	LockOnComponent = CreateDefaultSubobject<UTeloLockOnComponent>(TEXT("LockOnComponent"));

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

		//// Attack
		//EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ATeloPlayerCharacter::DoAttackStart);

		//// Block
		//EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Started, this, &ATeloPlayerCharacter::DoBlockStart);
		//EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Completed, this, &ATeloPlayerCharacter::DoBlockEnd);

		// Lock On
		EnhancedInputComponent->BindAction(LockOnAction, ETriggerEvent::Completed, this, &ATeloPlayerCharacter::DoLockOn);
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
}

void ATeloPlayerCharacter::MoveInput(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	DoMove(MovementVector.X, MovementVector.Y);
}

void ATeloPlayerCharacter::DoMove(float Right, float Forward)
{
	if (GetController())
	{
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
	Jump();
}

void ATeloPlayerCharacter::DoJumpEnd()
{
	StopJumping();
}

// 입력 값이 없어도 움직이고 있다면 슬라이딩
void ATeloPlayerCharacter::DoCrouchStart()
{
	Crouch();
	ApplyLockOnMovementMode(true);

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

	ResetMovementComps(); // 본래 마찰력/감속력 복구
	ApplyLockOnMovementMode(false);
}

// 현재 움직임과 상관없이 입력 값으로 대시
void ATeloPlayerCharacter::DoDashStart()
{
	if (!bCanDash || bIsDashing) return; // 대시 불가능/대시 중일 시 종료
	if (GetCharacterMovement()->GetCurrentAcceleration().IsNearlyZero()) return; // 가속이 없을 시 종료 (입력 없을 시)

	bIsDashing = true;
	bCanDash = false;

	//FVector DashDir = GetActorForwardVector();
	
	FVector DashDir = GetCharacterMovement()->GetCurrentAcceleration().GetSafeNormal2D();
	if (DashDir.IsNearlyZero())
	{
		DashDir = GetActorForwardVector(); // 입력 방향으로 대시
	}

	// 대시 중에는 이동방향 바라보기
	ApplyLockOnMovementMode(true);

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

	ApplyLockOnMovementMode(false);
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

void ATeloPlayerCharacter::ApplyLockOnMovementMode(bool bLockOn)
{
	// 락온이 아닐 땐 리턴
	if (!LockOnComponent || !LockOnComponent->IsLockOn())
		return;

	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	if (!MoveComp) return;

	if (bLockOn) // 락온 중 특수한 동작 시
	{
		bUseControllerRotationYaw = false;				// 캐릭터가 컨트롤러 회전에 따라 회전하지 않음
		MoveComp->bOrientRotationToMovement = true;		// 캐릭터가 이동 방향에 따라 회전하도록 설정
	}
	else // 락온 중 특수한 동작을 하지 않을 시
	{
		bUseControllerRotationYaw = true;				// 캐릭터가 컨트롤러 회전에 따라 회전
		MoveComp->bOrientRotationToMovement = false;	// 캐릭터가 이동 방향에 따라 회전하지 않음
	}
}