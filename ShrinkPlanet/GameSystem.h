#pragma once

#include "Rigidbody3D.h"
#include "UI.h"
#include <vector>
#include <iostream>
#include <set>

using namespace std;

class GameSystem{


public:
	enum State { MENU, PLAYING, DONE };

	GameSystem() {
		towerBlock = 0;
		producedBlock = 0;
	}
	static GameSystem* GetInstance();
	int GetTowerBlock();

	void AddTowerBlock(int a);
	void SetInitBlock(dGeomID);
	void BlockProduced();
	dGeomID GetInitBlockGeom();

	void SetGroundGeom(dGeomID);
	dGeomID GetGroundGeom();
	void CollideCheck(dGeomID, dGeomID);
	void SetState(GameSystem::State);
	set<dGeomID> GetDeadBlock();
	GameSystem::State GetState();

	const int maxBlock = 10;
	int shadingMode = 0;

private:

	bool CheckDeadBlock(dGeomID);
	static GameSystem* instance;
	dGeomID initiBlock;
	dGeomID ground;
	set<dGeomID> deadBlock;
	int producedBlock;
	int towerBlock;


	State curState;


};


