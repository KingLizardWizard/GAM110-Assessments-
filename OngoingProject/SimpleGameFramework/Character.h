#pragma once
class Character
{
public:
	Character();
	virtual ~Character();

	void Attack(Character& characterRef);
	void PrintInfo() {}
	void OnDodge() {}
	void OnDied() {}

	void instaKill() const;

public:
	mutable int health = 100;
	int maxHealth = 150;
	Vector2i location;
	int minDamage = 1;
	int maxDamage = 5;
	float hitChance = 0.8f;
};

