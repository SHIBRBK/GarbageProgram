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
		//被ダメージエフェクト
		static constexpr float TIME_DAMAGED_EFFECT = 1.0f;


		//衝突判定；球体の半径
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
		// ショット(ポインタ)
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
		//被ダメージエフェクトの時間計測用
		float stepDamaged_;
		// 耐久力
		int hp_;
		int DeadExplo;
		int DeadExploPlay;

	protected:
		// 状態遷移
		void ChangeState(STATE state);

};

