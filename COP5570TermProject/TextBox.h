#pragma once

#include <string>
#include <SFML/Graphics.hpp>

//text box and text button class
class TextBox
{
public:
	const bool IS_BUTTON;	//boolean to control whether text is uneditable

	TextBox(std::string string, sf::RenderWindow* window, sf::Font &font, sf::Color buttonColor, sf::Color buttonOutlineColor, sf::Color textColor, sf::Color textOutlineColor, bool isButton);

	void setSize(float width, float height, int fontSize);
	void setPosition(float x, float y);
	void setRotation(float theta);

	void setString(std::string string);
	std::string getString();
	int getValue(int min);
	void addCharacter(char character);

	void draw();
	bool doesContainPoint(sf::Vector2i point);

private:
	sf::RenderWindow* const WINDOW;	//window to draw on

	sf::RectangleShape shape;	//background of button
	sf::Text text;				//text object of button
	std::string string;			//string in text object
};
