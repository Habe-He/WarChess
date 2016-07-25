#include "Player.h"

void Player::setName(const char* name)
{
	m_sName = name;
}

const char* Player::getName()
{
	return m_sName.c_str();
}

void Player::setLocal(bool _local)
{
	m_bLocal = _local;
}

bool Player::isLocal()
{
	return m_bLocal;
}
