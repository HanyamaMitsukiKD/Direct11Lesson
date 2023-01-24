#pragma once


class KdThrowManager
{
public:


	static KdThrowManager& GetInstance()
	{
		static KdThrowManager instance;
		return instance;
	}

	void Update();
	void Init();
	void Draw();

	void EaseInOutQuint(float& x);

	const bool GetThrowFlg() const{ return throwFlg; }

	const float GetSpeed()const
	{
		return throwSpeed;
	}



private:
	KdThrowManager() {}
	~KdThrowManager() {}

	//スローモーション
	float	throwSpeed = 1.0f;
	float	animationthrowSpeed = 0.0f;
	bool	throwFlg = false;
	bool	counterFlg = false;
	int		m_count = 0;
};