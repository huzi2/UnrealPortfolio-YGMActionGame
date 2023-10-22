// Copyright Epic Games, Inc. All Rights Reserved.

#include "YGMActionGameLibrary.h"

float UYGMActionGameLibrary::GetAngleActorToActor(AActor* MainActor, AActor* TargetActor)
{
	if (!MainActor || !TargetActor) return 0.f;

	const FVector ToTarget = (TargetActor->GetActorLocation() - MainActor->GetActorLocation()).GetSafeNormal();
	const FVector ForwardVector = MainActor->GetActorForwardVector();
	const float DotProduct = FVector::DotProduct(ToTarget, ForwardVector);
	const float Angle = FMath::Acos(DotProduct) * (180.f / PI);
	return Angle;
}

float UYGMActionGameLibrary::GetDistance(const FVector& Location1, const FVector& Location2)
{
	return static_cast<float>((Location1 - Location2).Size());
}

EDir UYGMActionGameLibrary::GetDirection(const AActor* MainActor, const FVector& TargetLocation)
{
	if (!MainActor) return EDir::ED_Front;

	// 들어오는 공격이 어디쪽인지 확인. 전방벡터와 원점->공격지점 벡터끼리 내적
	const FVector Forward = MainActor->GetActorForwardVector();
	const FVector ImpactLowered(TargetLocation.X, TargetLocation.Y, MainActor->GetActorLocation().Z);
	const FVector ToHit = (ImpactLowered - MainActor->GetActorLocation()).GetSafeNormal();

	// 내적값에 Acos하면 각도가 나온다(|A|*|B| = cos(theta))
	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	double Theta = FMath::Acos(CosTheta);
	Theta = FMath::RadiansToDegrees(Theta);

	// 하지만 위 결과로는 양수만 나와서 각도는 알아도 왼쪽인지 오른쪽인지 알수가 없다. 그래서 외적함
	// 언리얼엔진에서 외적은 왼손법칙을 따르며 검지와 중지의 외적이 엄지를 따른다.
	// 그래서 엄지 위치(Z값)가 위인지 아래(0보다 작음)인지 확인해서 왼쪽인지 오른쪽인지 확인
	// 왼손으로 검지를 전방벡터, 중지를 히트포인트로 생각하면 중지가 오른쪽일때 엄지가 위로간다
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	// 엄지(Z값)가 아래(0보다 작음)이므로 방향은 왼쪽
	if (CrossProduct.Z < 0.0)
	{
		Theta *= -1.f;
	}

	if (Theta >= -45.f && Theta < 45.f)
	{
		return EDir::ED_Front;
	}
	else if (Theta >= -135.f && Theta < -45.f)
	{
		return EDir::ED_Left;
	}
	else if (Theta >= 45.f && Theta < 135.f)
	{
		return EDir::ED_Right;
	}
	// -135 ~ 135도
	return EDir::ED_Back;
}

float UYGMActionGameLibrary::GetMovementDirection(const FVector& Velocity, const FVector& Forward)
{
	if (Velocity.IsZero()) return 0.f;

	const FVector VelocityNormal = Velocity.GetSafeNormal();
	// 객체의 방향과 정규화된 속도 벡터 간의 내적을 계산한 후, 그 결과를 사용해 두 벡터 간의 각도를 얻음
	const float AngleBetween = FMath::Acos(FVector::DotProduct(Forward, VelocityNormal));
	// 객체의 방향과 정규화된 속도 벡터의 외적을 계산. 이를 통해 두 벡터가 형성하는 평면의 수직 벡터를 얻음
	const FVector CrossProduct = FVector::CrossProduct(Forward, VelocityNormal);
	// 두 벡터 간의 각도를 라디안에서 도로 변환
	const float Degree = FMath::RadiansToDegrees(AngleBetween);
	// 만약 외적의 결과가 0이면 (즉, 두 벡터가 서로 평행하면) 두 벡터 간의 각도를 그대로 반환
	// 그렇지 않으면 외적의 Z 컴포넌트의 부호에 따라 각도의 부호를 결정
	return CrossProduct.IsZero() ? Degree : Degree * FMath::Sign(CrossProduct.Z);
}

bool UYGMActionGameLibrary::CheckPercentage(const int32 Percent)
{
	return FMath::RandRange(1, 100) <= Percent ? true : false;
}
