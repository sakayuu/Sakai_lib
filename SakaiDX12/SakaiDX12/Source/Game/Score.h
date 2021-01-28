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
	// �ˌ���
	static int numObShot;

	// ������
	static int hit;

	std::string scoreRank = "C";

	std::string message;

};