#include "Card.h"

#include <iostream>

sf::Vector2f Card::size = { 60, 100 };
unsigned int Card::ammount = 0;



Card::Card(const CardType& type)
	: m_type(type), m_scale(280.f / size.y), 
	m_flipped_texture(false),
	m_offset(-150, 0)
{
	m_order = Card::ammount;
	
	m_sprite = new sf::Sprite(ResourceManager::texture_card_back);

	m_sprite->setOrigin(size / 2.f);
	m_sprite->setScale({ -m_scale, m_scale });

	createText();
	createFrontTexture();
}

Card::~Card()
{
	delete m_card_image;

	delete m_sprite;
	delete m_front_texture;
}

void Card::createFrontTexture()
{
	m_front_texture = new sf::RenderTexture(sf::Vector2u(size));

	m_front_texture->setSmooth(0);

	m_front_texture->clear(sf::Color::Transparent);
	m_front_texture->draw(sf::Sprite(
		ResourceManager::texture_card_front
	));
	m_front_texture->draw(*m_card_image);
	m_front_texture->display();
}

void Card::createText()
{
	sf::Vector2f center = Card::size / 2.f;

	sf::FloatRect card_image_rect;
	float scale;

	float max_width = Card::size.x * 0.75;
	float max_height = Card::size.y * 0.20;


	switch (m_type)
	{
	case CardType::UP:
		m_card_image = new sf::Sprite(ResourceManager::texture_up);
		break;
	case CardType::DOWN:
		m_card_image = new sf::Sprite(ResourceManager::texture_down);
		break;
	case CardType::LEFT:
		m_card_image = new sf::Sprite(ResourceManager::texture_left);
		break;
	case CardType::RIGHT:
		m_card_image = new sf::Sprite(ResourceManager::texture_right);
		break;
	case CardType::SPACE:
		m_card_image = new sf::Sprite(ResourceManager::texture_space);
		break;


	case CardType::DONT_UP:
		m_card_image = new sf::Sprite(ResourceManager::texture_dont_up);
		break;
	case CardType::DONT_DOWN:
		m_card_image = new sf::Sprite(ResourceManager::texture_dont_down);
		break;
	case CardType::DONT_LEFT:
		m_card_image = new sf::Sprite(ResourceManager::texture_dont_left);
		break;
	case CardType::DONT_RIGHT:
		m_card_image = new sf::Sprite(ResourceManager::texture_dont_right);
		break;
	case CardType::DONT_SPACE:
		m_card_image = new sf::Sprite(ResourceManager::texture_dont_space);
		break;

	case CardType::JOKER:
		m_card_image = new sf::Sprite(ResourceManager::texture_joker);
		break;
	}



	card_image_rect = m_card_image->getLocalBounds();
	scale = std::min(
		max_width / card_image_rect.size.x,
		max_height / card_image_rect.size.y
	);
	m_card_image->setPosition(center);
	m_card_image->setOrigin({
		card_image_rect.size.x / 2.0f,
		card_image_rect.size.y / 2.0f
		});

	m_card_image->setScale({ scale, scale });

}






void Card::update(const float& dt)
{
	if (m_order == 0)
	{
		m_offset.x = std::lerp(m_offset.x, 150, 15.f * dt);

		sf::Vector2f _scale = m_sprite->getScale();

		if (!m_flipped_texture && _scale.x > 0.f)
		{
			m_flipped_texture = true;
			m_sprite->setTexture(m_front_texture->getTexture());
		}


		m_sprite->setScale({
			std::lerp(_scale.x, m_scale, 10.f * dt),
			m_scale
			});
	}
}

const sf::Vector2f Card::getOrderedOffset()
{
	return m_offset + m_offset * ((float)std::pow(1.1f, m_order) - 1);
}

unsigned int& Card::getOrder()
{
	return m_order;
}

sf::Sprite* Card::getSprite() const
{
	return m_sprite;
}

const sf::Vector2f& Card::getOffset() const
{
	return m_offset;
}

const CardType& Card::getType() const
{
	return m_type;
}