#pragma once

#include <SFML/Graphics.hpp>

#include "ResourcesManager.h"

enum class CardType : unsigned char
{
	UP, DOWN, LEFT, RIGHT,
	DONT_UP, DONT_DOWN, DONT_LEFT, DONT_RIGHT,
	SPACE,
	DONT_SPACE,
	JOKER
};

class Card
{
private:
	sf::Sprite* m_sprite;
	sf::RenderTexture* m_front_texture;
	const CardType m_type;
	float m_scale;
	bool m_flipped_texture;
	sf::Vector2f m_offset;
	unsigned int m_order;

	sf::Sprite* m_card_image;

	void createFrontTexture();
	void createText();


public:
	static sf::Vector2f size;
	static unsigned int ammount;


	Card(const CardType& type);
	~Card();

	void update(const float& dt);

	unsigned int& getOrder();
	const sf::Vector2f getOrderedOffset();

	sf::Sprite* getSprite() const;
	const sf::Vector2f& getOffset() const;
	const CardType& getType() const;
};

