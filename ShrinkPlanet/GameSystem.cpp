#include "GameSystem.h"

enum State { MENU, PLAYING, DONE };

int GameSystem::GetTowerBlock() {
	return towerBlock;
}

void GameSystem::AddTowerBlock(int a) {
	towerBlock += a;
}

void GameSystem::SetInitBlock(dGeomID initiBlock) {
	this->initiBlock = initiBlock;
	cout << this-> initiBlock << endl;
}


GameSystem* GameSystem::GetInstance() {
	if (instance == nullptr) {
		instance = new GameSystem();
	}
	return instance;
}


dGeomID GameSystem::GetInitBlockGeom() {

	return this->initiBlock;
}

dGeomID GameSystem::GetGroundGeom() {

	return this->ground;
}


void GameSystem::SetGroundGeom(dGeomID ground) {
	this->ground = ground;
}

bool GameSystem::CheckDeadBlock(dGeomID geom) {
	return deadBlock.find(geom) != deadBlock.end();
}

void GameSystem::CollideCheck(dGeomID geom1, dGeomID geom2) {
	
	//ground & new Block
	if (geom1 == this->ground && geom2 != this->initiBlock){
		if (!CheckDeadBlock(geom2)) {
			deadBlock.insert(geom2);
			UI::GetInstance()->PopScoreBoard();
		}
	}
	else if (geom2 == this->ground && geom1 != this->initiBlock) {
		if (!CheckDeadBlock(geom1)) {
			deadBlock.insert(geom1);
			UI::GetInstance()->PopScoreBoard();
		}
	}
}



void GameSystem::BlockProduced() {
	producedBlock++;
}


void GameSystem::SetState(GameSystem::State state) {

	this->curState = state;
}
GameSystem::State GameSystem::GetState() {

	return curState;
}

set<dGeomID> GameSystem::GetDeadBlock() {
	return deadBlock;

}