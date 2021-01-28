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
	// ËŒ‚‰ñ”
	static int numObShot;

	// –½’†‰ñ”
	static int hit;

	std::string scoreRank = "C";

	std::string message;

};