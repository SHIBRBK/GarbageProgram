#pragma once
#include<memory>
#include <vector>
#include "../Common/Transform.h"
#include "EnemyBase.h"
#include "EnemyShot.h"
class Collider;

class KinkyuNoTeki;
class PlayerShot;
using uniqueEMA = std::unique_ptr<KinkyuNoTeki>;

class KinkyuNoTeki :
	public EnemyBase
{

	public:

		enum class STATE
		{
			NONE,
			ALIVE,
			DEAD
		};

		static constexpr float TIME_DELAY_SHOT = 1.0f;
		//��_���[�W�G�t�F�N�g
		static constexpr float TIME_DAMAGED_EFFECT = 1.0f;


		//�Փ˔���G���̂̔��a
		static constexpr float COLLISION_RADIUS = 100.0f;
		KinkyuNoTeki();
		~KinkyuNoTeki()override;
		bool Init()override;
		void Update()override;

		void UpdateDead();
		void Draw()override;
		void Release()override;
		void AddCollider(Collider* collider);
		void ClearCollider(void);
		std::vector<EnemyShot*>& GetShots(void);
		// �V���b�g(�|�C���^)
		std::vector<EnemyShot*> shots_;
		void ProcessShot(void);
		void CreateShot(void);
		STATE state_;
		bool IsAlive(void) const;
		bool IsDead(void) const;
		const Transform& GetTransform(void) const;

	private:
		float Deg2RadF(float deg);

		Transform transform_;
		float stepShotDelay_;
		std::vector<Collider*> colliders_;
		//��_���[�W�G�t�F�N�g�̎��Ԍv���p
		float stepDamaged_;
		// �ϋv��
		int hp_;
		int DeadExplo;
		int DeadExploPlay;

	protected:
		// ��ԑJ��
		void ChangeState(STATE state);

};

