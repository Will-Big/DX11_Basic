#pragma once

class TimeManager
{
public:
	TimeManager();
	~TimeManager() = default;

private:
	LARGE_INTEGER m_CurCount {};
	LARGE_INTEGER m_PrevCount {};
	LARGE_INTEGER m_Frequency {};

	double m_DeltaTime = 0.f;				// 프레임 간의 시간 값
	double m_ElapsedTime = 0.f;
	uint32_t m_LastFPS = 0;
	uint32_t m_FPS = 0;

public:
	void Initialize();
	void Update();
	void Finalize();

	float GetDeltaTime() { return static_cast<float>(m_DeltaTime); }
	uint32_t GetFPS() { return m_LastFPS; }
};