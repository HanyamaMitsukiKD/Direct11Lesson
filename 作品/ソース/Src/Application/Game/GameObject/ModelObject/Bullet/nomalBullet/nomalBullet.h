#pragma once

#include"Application/Game/GameObject/ModelObject/Bullet/BulletBase.h"

class nomalBullet :public BulletBase
{
public:
	nomalBullet() {}
	virtual ~nomalBullet() override {}

	void Update() override;
	void Init() override;
	void DrawLighting()override;
	

	void UpdateRotate();

private:
	//�e�̏�ԕʍX�V
	void NormalUpdate();

	//�C�[�W���O�֐�����
	//�e�̋���
	void UpdateMove();
	

	//�Փ˔���
	void UpdateGroundCollision();

	void SmokeUpdate();

	void EffectFire(Math::Vector3& pos);

private:
	Math::Vector3	m_moveDir;				//����
	Math::Vector3	m_targetPos;			//�^�[�Q�b�g���W

	int				loopAttckCount = 0;		//����U�����邩�i�e�̎c�e���G�l�~�[�Ɉˑ�������܂Łj
	float			bulletYPosSpeed = 0;	//�e��Y���W��o���Ă����ړ���

	//�C�[�W���O�֐��p
	float			m_moveSpeed = 0.0f;		//�X�s�[�h
};