#include "TextBox.h"

const double RADIANS_PER_DEGREE = 0.01745329251994329577;

//text box constructor
//initializes member variables
TextBox::TextBox(std::string string, sf::RenderWindow* window, sf::Font &font, sf::Color buttonColor, sf::Color buttonOutlineColor, sf::Color textColor, sf::Color textOutlineColor, bool isButton) : IS_BUTTON(isButton), WINDOW(window)
{
	this->string = string;

	shape.setOutlineThickness(3.0f);
	shape.setOutlineColor(buttonOutlineColor);
	shape.setFillColor(buttonColor);

	text.setOutlineThickness(3.0f);
	text.setOutlineColor(textOutlineColor);
	text.setFillColor(textColor);
	text.setFont(font);
}

//sets size of shape and font
void TextBox::setSize(float width, float height, int fontSize)
{
	shape.setSize(sf::Vector2f(width, height));

	text.setString(string);
	text.setCharacterSize(fontSize);
	text.setOrigin(0.5f * text.getLocalBounds().width, 0.5f * text.getLocalBounds().height);
}

//sets position of text box
void TextBox::setPosition(float x, float y)
{
	shape.setPosition(x, y);
	text.setPosition(0.5f * shape.getSize().x + x, 0.5f * shape.getSize().y + y);
}


//sets rotation of text box
void TextBox::setRotation(float theta)
{
	float s = sinf(theta * RADIANS_PER_DEGREE);
	float c = cosf(theta * RADIANS_PER_DEGREE);

	shape.setRotation(theta);
	text.setRotation(theta);
	text.setPosition(shape.getPosition().x + 0.5f * (shape.getSize().x * c - shape.getSize().y * s), shape.getPosition().y + 0.5f * (shape.getSize().x * s + shape.getSize().y * c));
}

//sets text
void TextBox::setString(std::string string)
{
	this->string = string;
	text.setString(string);
	text.setOrigin(0.5f * text.getLocalBounds().width, 0.5f * text.getLocalBounds().height);
}

//gets text
std::string TextBox::getString()
{
	return string;
}

//gets text as int
//value will not be lower than min
//if string is not a number, then return value is min
int TextBox::getValue(int min)
{
	int value;

	try
	{
		value = std::stoi(string);

		return (value < min) ? (min) : (value);
	}
	catch (std::exception& e)
	{
		return min;
	}
}

//appends digit to text
//character 8 is backspace
void TextBox::addCharacter(char character)
{
	if (!IS_BUTTON)
	{
		if (character == (char)8)
		{
			if (string.size())
			{
				string = string.substr(0, string.size() - 1);
			}
		}
		else if (character >= '0' && character <= '9')
		{
			string += character;
		}

		text.setString(string);
		text.setOrigin(0.5f * text.getLocalBounds().width, 0.5f * text.getLocalBounds().height);
	}
}

//draws background and text
void TextBox::draw()
{
	WINDOW->draw(shape);
	WINDOW->draw(text);
}

//checks if point is inside text box
bool TextBox::doesContainPoint(sf::Vector2i point)
{
	return shape.getGlobalBounds().contains(point.x, point.y);
}
