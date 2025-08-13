#pragma once

class DevDebug
{
public:
	inline static void OnScreenLog(FString Message, FColor Color = FColor::Red, float Time = 5)
	{
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, Time, Color, Message);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);
		}
	}
};
