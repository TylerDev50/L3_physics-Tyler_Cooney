#include "Sheep.h"

Sheep::Sheep()
{
	// initialise animations
	for (int i = 0; i < 4; i++)
		m_walkDown.addFrame({ { 64 * i, 0 }, { 64, 64 } });
	m_walkDown.setLooping(true);
	m_walkDown.setFrameSpeed(0.25f);

	for (int i = 0; i < 4; i++)
		m_walkUp.addFrame({ { (64 * (i + 4)), 0 }, { 64, 64 } });
	m_walkUp.setLooping(true);
	m_walkUp.setFrameSpeed(0.25f);

	for (int i = 0; i < 4; i++)
		m_walkUpRight.addFrame({ { 64 * i, 64 }, { 64, 64 } });
	m_walkUpRight.setLooping(true);
	m_walkUpRight.setFrameSpeed(0.25f);

	for (int i = 0; i < 4; i++)
		m_walkRight.addFrame({ { (64 * (i + 4)), 64 }, { 64, 64 } });
	m_walkRight.setLooping(true);
	m_walkRight.setFrameSpeed(0.25f);

	for (int i = 0; i < 4; i++)
		m_walkDownRight.addFrame({ { 64 * i, 128 }, { 64, 64 } });
	m_walkDownRight.setLooping(true);
	m_walkDownRight.setFrameSpeed(0.25f);

	// the next 4 animations go clockwise from Up through Right to Down.

	m_currentAnimation = &m_walkDown;
	setTextureRect(m_currentAnimation->getCurrentFrame());
}

Sheep::~Sheep()
{
}

void Sheep::handleInput(float dt)
{
	// set sheep direction
	// decrement and check the input buffer.
	m_inputBuffer -= dt;
	if (m_inputBuffer > 0)
	{
		// not long enough has passed since the last input change, so don't handle input
		return;
	}
	// grab this to detect changes per frame for later
	Direction last_dir = m_direction;

	// sheep brake
	if (m_input->isKeyDown(sf::Keyboard::Scancode::Space))
	{
		m_direction = Direction::NONE;
		return;
	}

	// Set 8-directional movement based on WASD
	if (m_input->isKeyDown(sf::Keyboard::Scancode::A))
	{
		if (m_input->isKeyDown(sf::Keyboard::Scancode::W))
		{
			m_direction = Direction::UP_LEFT;
			m_currentAnimation = &m_walkUpRight;
			m_currentAnimation->setFlipped(true);
		}


		else if (m_input->isKeyDown(sf::Keyboard::Scancode::S))
		{
			m_direction = Direction::DOWN_LEFT;
			m_currentAnimation = &m_walkDownRight;
			m_currentAnimation->setFlipped(true);
		}
		else
		{
			m_direction = Direction::LEFT;
			m_currentAnimation = &m_walkRight;
			m_currentAnimation->setFlipped(true);
		}

	}
	else if (m_input->isKeyDown(sf::Keyboard::Scancode::D))
	{
		if (m_input->isKeyDown(sf::Keyboard::Scancode::W))
		{
			m_direction = Direction::UP_RIGHT;
			m_currentAnimation = &m_walkUpRight;
			m_currentAnimation->setFlipped(false);
		}
		else if (m_input->isKeyDown(sf::Keyboard::Scancode::S))
		{
			m_direction = Direction::DOWN_RIGHT;
			m_currentAnimation = &m_walkDownRight;
			m_currentAnimation->setFlipped(false);
		}
		else
		{
			m_direction = Direction::RIGHT;
			m_currentAnimation = &m_walkRight;
			m_currentAnimation->setFlipped(false);
		}

	}
	else
	{
		if (m_input->isKeyDown(sf::Keyboard::Scancode::W))
		{
			m_direction = Direction::UP;
			m_currentAnimation = &m_walkUp;

		}

		else if (m_input->isKeyDown(sf::Keyboard::Scancode::S))
		{
			m_direction = Direction::DOWN;
			m_currentAnimation = &m_walkDown;
		}
	}

	// set input buffer if needed, this makes diagonal movement easier
	if (m_direction != last_dir)
		m_inputBuffer = INPUT_BUFFER_LENGTH;
}


void Sheep::update(float dt)
{
	setTextureRect(m_currentAnimation->getCurrentFrame());
	if (m_direction != Direction::NONE)
		m_currentAnimation->animate(dt);

	// move the sheep
	// for diagonal movement
	float diagonal_speed = m_speed * APPROX_ONE_OVER_ROOT_TWO * dt;
	float orthog_speed = m_speed * dt;	// orthogonal movement

	switch (m_direction)
	{
	case Direction::UP:
		move({ 0, -orthog_speed });
		break;
	case Direction::UP_RIGHT:
		move({ diagonal_speed, -diagonal_speed });
		break;
	case Direction::RIGHT:
		move({ orthog_speed,0 });
		break;
	case Direction::DOWN_RIGHT:
		move({ diagonal_speed, diagonal_speed });
		break;
	case Direction::DOWN:
		move({ 0, orthog_speed });
		break;
	case Direction::DOWN_LEFT:
		move({ -diagonal_speed, diagonal_speed });
		break;
	case Direction::LEFT:
		move({ -orthog_speed,0 });
		break;
	case Direction::UP_LEFT:
		move({ -diagonal_speed, -diagonal_speed });
		break;
	}
}