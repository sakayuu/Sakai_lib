#pragma once
#include <string>

class Score
{
public:
	Score();
	~Score();

	void SetScore(int numObShot, int hit);

	const std::string CheckScore();
private:


public:
	// 射撃回数
	static int numObShot;

	// 命中回数
	static int hit;

	std::string scoreRank = "C";

	std::string message;

};