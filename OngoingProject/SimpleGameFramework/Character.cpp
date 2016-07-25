#include "stdafx.h"
#include "Character.h"


Character::Character()
{

}


Character::~Character()
{

}

void Character::Attack(Character& characterRef)
{

}

void Character::instaKill() const
{
	health = 0;

	Character* nonConstThis = const_cast<Character*>(this);
	nonConstThis->OnDied();
}