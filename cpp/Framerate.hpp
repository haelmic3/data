#include<string>
#include<SFML/Graphics.hpp>
#define fhs 100
class Framerate :public sf::Drawable
{
	unsigned
			fcount
		,	fupdate
	;
	double
			frame
		,	fmax
		,	fmin
		,	fsum
		,	fhist[fhs]
	;
	sf::Font
			font
	;
	sf::Text
			text
	;
	sf::Clock
			timer
	;
	void update();
 public:
	Framerate(unsigned fontsize,std::string fontname,sf::Color color = sf::Color::Yellow);
	virtual void draw(sf::RenderTarget&target, sf::RenderStates states) const;
	void tick();
	double average();
	double max();
	double min();
	void setColor(sf::Color color);
	void setOutlineColor(sf::Color color);
	void setOutlineThickness (float thickness);
};

