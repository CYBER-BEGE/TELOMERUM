// Fill out your copyright notice in the Description page of Project Settings.


#include "TeloPlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
ATeloPlayerCharacter::ATeloPlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

// Called when the game starts or when spawned
void ATeloPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<APlayerController>(GetController());

	/* 컴포넌트 값 확인 */
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

	/* 초기 상태 설정 */
	MaxHealth = 100.0f;
	MoveSpeedScale = 1.5f;
	JumpPowerScale = 2.0f;

	/* CharacterMovement 세팅 */
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

// Called every frame
void ATeloPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Warning, TEXT("Velocity: %s"), *MovementVector.ToString());
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

		//// Attack
		//EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ATeloPlayerCharacter::DoAttackStart);

		//// Block
		//EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Started, this, &ATeloPlayerCharacter::DoBlockStart);
		////EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Completed, this, &ATeloPlayerCharacter::DoBlockEnd);
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
		AddMovementInput(GetActorRightVector(), Right * MoveSpeedScale);
		AddMovementInput(GetActorForwardVector(), Forward * MoveSpeedScale);
	}
}

void ATeloPlayerCharacter::LookInput(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void ATeloPlayerCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController())
	{
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
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
}

// 움직임이 있더라도 입력 값이 없으면 대시 불가
void ATeloPlayerCharacter::DoDashStart()
{
	//if (!HasJetpack) return;					// 제트팩 없을 시 종료
	if (!bCanDash || bIsDashing) return;		// 대시 불가능/대시 중일 시 종료
	if (InputVector.IsNearlyZero()) return;	// 이동 입력이 없을 시 종료

	bIsDashing = true;
	bCanDash = false;

	FRotator CameraRot = Controller->GetControlRotation(); // 카메라 회전값
	FVector ForwardDir = FRotationMatrix(CameraRot).GetUnitAxis(EAxis::X);
	FVector RightDir = FRotationMatrix(CameraRot).GetUnitAxis(EAxis::Y);
	FVector DashDir = (ForwardDir * InputVector.Y + RightDir * InputVector.X).GetSafeNormal();

	LaunchCharacter(DashDir * 2000.0f, true, true); // 임펄스 적용

	GetCharacterMovement()->Velocity = FVector::ZeroVector;		// 이동 정지
	GetCharacterMovement()->GravityScale = 0.0f;				// 중력 0
	GetCharacterMovement()->GroundFriction = 0.0f;				// 마찰력 0
	GetCharacterMovement()->BrakingDecelerationWalking = 0.0f;	// 감속력 0
	GetCharacterMovement()->MaxWalkSpeed = 0.0f;				// 이동 속도 0

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
}

void ATeloPlayerCharacter::DashCooldown()
{
	bCanDash = true;
}
