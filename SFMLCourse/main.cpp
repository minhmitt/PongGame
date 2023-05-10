#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <sstream>

/// Game Pong
/// Dinh Nhat Minh
/// MSV: 22025506
/// QH-2022-I/CQ-N-CLC


int main() {
	//Creation
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Game");

	//Fps limit
	window.setFramerateLimit(80);

	window.setKeyRepeatEnabled(false);

	//Variable that keeps the game loop running
	bool play = true;

	sf::Event event;

	//Font
	sf::Font font;
	if (font.loadFromFile("Data/arial.ttf") == 0)
	{
		return 1;
	}

	sf::Text score;
	score.setFont(font);
	score.setCharacterSize(30);
	score.setFillColor(sf::Color::Green);
	score.setPosition(380, 10);
	score.setString("0 : 0");

	//Images
	sf::Texture background;
	sf::Texture ball;
	sf::Texture pad1;
	sf::Texture pad2;
	
	if (background.loadFromFile("Data/background.png") == false)
	{
		return -1;
	}

	if (ball.loadFromFile("Data/ballShape.png") == false)
	{
		return -1;
	}

	if (pad1.loadFromFile("Data/blue.png") == false)
	{
		return -1;
	}

	if (pad2.loadFromFile("Data/red.png") == false)
	{
		return -1;
	}

	//Sound
	sf::SoundBuffer buff_hit;
	sf::Sound hit;
	hit.setBuffer(buff_hit);

	//Load the sound
	if (buff_hit.loadFromFile("Data/hitSound.wav") == false) {
		return -1;
	}

	//Load the music
	sf::Music music;
	if (music.openFromFile("Data/music.ogg") == false) {
		return -1;
	}

	music.setLoop(true);
	music.play();
	
	//States
	bool up = false;
	bool down = false;

	//Variables
	int yVelocityPad1 = 0;
	int yVelocityPad2 = 0;
	int xVelocityBall = -3;
	int yVelocityBall = -3;
	int player1Score = 0;
	int player2Score = 0;
	
	//Shapes
	sf::RectangleShape backgroundShape;
	backgroundShape.setSize(sf::Vector2f(800, 600));
	backgroundShape.setPosition(0, 0);
	backgroundShape.setTexture(&background);

	//Pad1 
	sf::RectangleShape pad1Shape;
	pad1Shape.setSize(sf::Vector2f(40, 150));
	pad1Shape.setPosition(50, 200);
	pad1Shape.setTexture(&pad1);
	pad1Shape.setOutlineColor(sf::Color::Blue);

	//Pad2
	sf::RectangleShape pad2Shape;
	pad2Shape.setSize(sf::Vector2f(40, 150));
	pad2Shape.setPosition(700, 200);
	pad2Shape.setTexture(&pad2);
	pad2Shape.setOutlineColor(sf::Color::Red);

	//Ball
	sf::CircleShape ballShape;
	ballShape.setRadius(30);
	ballShape.setPosition(370, 250);
	ballShape.setTexture(&ball);
	ballShape.setOutlineColor(sf::Color::Green);

	//Game loop
	while (play == true) 
	{
		//EVENTS
		while (window.pollEvent(event))
		{
			//Event type is window closed
			if (event.type == sf::Event::Closed) {
				//set play to false in order to stop the game loop
				play = false;
			}
		}
		
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up) {
			up = true;
		}

		if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Up) {
			up = false;
		}

		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down) {
			down = true;
		}

		if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Down) {
			down = false;
		}

		//LOGIC
		if (up == true) {
			yVelocityPad1 = -5;
		}

		if (down == true) {
			yVelocityPad1 = 5;
		}

		if (up == true && down == true) {
			yVelocityPad1 = 0;
		}

		if (up == false && down == false) {
			yVelocityPad1 = 0;
		}

		pad1Shape.move(0, yVelocityPad1);
		
		//Out of bounds check
		if (pad1Shape.getPosition().y < 0)
		{
			pad1Shape.setPosition(50, 0);
		}

		if (pad1Shape.getPosition().y > 450)
		{
			pad1Shape.setPosition(50, 450);
		}

		//Pad2 (AI)
		if (ballShape.getPosition().y < pad2Shape.getPosition().y)
		{
			yVelocityPad2 = -3;
		}

		if (ballShape.getPosition().y > pad2Shape.getPosition().y)
		{
			yVelocityPad2 = 3;
		}

		pad2Shape.move(0, yVelocityPad2);

		//Ball
		ballShape.move(xVelocityBall, yVelocityBall);
			
		//Out of bounds check for ball
		if (ballShape.getPosition().y < 0 || ballShape.getPosition().y > 450)
		{
			yVelocityBall = -yVelocityBall;
		}

		if (ballShape.getPosition().x < 50)
		{
			++player2Score;
			ballShape.setPosition(370, 250);
			yVelocityBall = 3; // ball for player 1
		}

		if (ballShape.getPosition().x > 800)
		{
			++player1Score;
			ballShape.setPosition(370, 250);
			xVelocityBall = 3; // ball for player 2
		}

		//Collision for pad1
		if (ballShape.getGlobalBounds().intersects(pad1Shape.getGlobalBounds()) == true)
		{
			xVelocityBall = -xVelocityBall;
			hit.play();
		}

		//Collision for pad2
		if (ballShape.getGlobalBounds().intersects(pad2Shape.getGlobalBounds()) == true)
		{
			xVelocityBall = -xVelocityBall;
			hit.play();
		}

		//RENDERING
		window.clear();

		window.draw(backgroundShape);
		window.draw(pad1Shape);
		window.draw(pad2Shape);
		window.draw(ballShape);

		//Score
		std::stringstream text;
		text << player1Score << " : " << player2Score;
		score.setString(text.str());
		window.draw(score);

		window.display();
	}


	//Clean up
	window.close();

	return 0;
}
