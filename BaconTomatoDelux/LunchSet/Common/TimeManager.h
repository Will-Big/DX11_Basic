#pragma once

class TimeManager
{
public:
	TimeManager();
	~TimeManager() = default;

private:
	LARGE_INTEGER m_CurCount;
	LARGE_INTEGER m_PrevCount;
	LARGE_INTEGER m_Frequency;

	double m_DeltaTime;				// 프레임 간의 시간 값

public:
	void Initialize();
	void Update();
	void Finalize();

	float GetDeltaTime() { return static_cast<float>(m_DeltaTime); }
};