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

	// ������ ������ ��������� Ȯ��. ���溤�Ϳ� ����->�������� ���ͳ��� ����
	const FVector Forward = MainActor->GetActorForwardVector();
	const FVector ImpactLowered(TargetLocation.X, TargetLocation.Y, MainActor->GetActorLocation().Z);
	const FVector ToHit = (ImpactLowered - MainActor->GetActorLocation()).GetSafeNormal();

	// �������� Acos�ϸ� ������ ���´�(|A|*|B| = cos(theta))
	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	double Theta = FMath::Acos(CosTheta);
	Theta = FMath::RadiansToDegrees(Theta);

	// ������ �� ����δ� ����� ���ͼ� ������ �˾Ƶ� �������� ���������� �˼��� ����. �׷��� ������
	// �𸮾������� ������ �޼չ�Ģ�� ������ ������ ������ ������ ������ ������.
	// �׷��� ���� ��ġ(Z��)�� ������ �Ʒ�(0���� ����)���� Ȯ���ؼ� �������� ���������� Ȯ��
	// �޼����� ������ ���溤��, ������ ��Ʈ����Ʈ�� �����ϸ� ������ �������϶� ������ ���ΰ���
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	// ����(Z��)�� �Ʒ�(0���� ����)�̹Ƿ� ������ ����
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
	// -135 ~ 135��
	return EDir::ED_Back;
}

float UYGMActionGameLibrary::GetMovementDirection(const FVector& Velocity, const FVector& Forward)
{
	if (Velocity.IsZero()) return 0.f;

	const FVector VelocityNormal = Velocity.GetSafeNormal();
	// ��ü�� ����� ����ȭ�� �ӵ� ���� ���� ������ ����� ��, �� ����� ����� �� ���� ���� ������ ����
	const float AngleBetween = FMath::Acos(FVector::DotProduct(Forward, VelocityNormal));
	// ��ü�� ����� ����ȭ�� �ӵ� ������ ������ ���. �̸� ���� �� ���Ͱ� �����ϴ� ����� ���� ���͸� ����
	const FVector CrossProduct = FVector::CrossProduct(Forward, VelocityNormal);
	// �� ���� ���� ������ ���ȿ��� ���� ��ȯ
	const float Degree = FMath::RadiansToDegrees(AngleBetween);
	// ���� ������ ����� 0�̸� (��, �� ���Ͱ� ���� �����ϸ�) �� ���� ���� ������ �״�� ��ȯ
	// �׷��� ������ ������ Z ������Ʈ�� ��ȣ�� ���� ������ ��ȣ�� ����
	return CrossProduct.IsZero() ? Degree : Degree * FMath::Sign(CrossProduct.Z);
}

bool UYGMActionGameLibrary::CheckPercentage(const int32 Percent)
{
	return FMath::RandRange(1, 100) <= Percent ? true : false;
}
