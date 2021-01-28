#include "Score.h"

int Score::numObShot = 0;
int Score::hit = 0;

Score::Score()
{
}

Score::~Score()
{
}

void Score::SetScore(int numObShot, int hit)
{
	this->numObShot = numObShot;
	this->hit = hit;
}

const std::string Score::CheckScore()
{
	auto accuracy = hit / numObShot;
	auto timeParHit = hit / 10;

	if (accuracy + timeParHit < 0.8f) {
		scoreRank = "C";
		return scoreRank;
	}
	if (accuracy + timeParHit < 1.4f) {
		scoreRank = "B";
		return scoreRank;
	}
	if (accuracy + timeParHit < 2) {
		scoreRank = "A";
		return scoreRank;
	}
	if (accuracy + timeParHit >= 2) {
		scoreRank = "S";
	}
	return scoreRank;
}
