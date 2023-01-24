#pragma once

class CounterState
{
public:
	static CounterState& GetInstance()
	{
		static CounterState instance;
		return instance;
	}

	void Update();

	const bool GetCounterFlg() const {
		return counterFlg;
	}

	void CounterFlgOn() { counterFlg = true; }
	void CounterFlgOFF() { counterFlg = false; }

private:
	CounterState(){}
	~CounterState(){}


	bool	counterFlg = false;
	int m_count = 0;
};