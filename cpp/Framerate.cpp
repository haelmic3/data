#include<string>
#include<cstring>
#include"Framerate.hpp"

void Framerate::setColor(sf::Color color)
{
	text.setFillColor(color);
}
void Framerate::setOutlineColor(sf::Color color)
{
	text.setOutlineColor(color);
}
void Framerate::setOutlineThickness (float thickness)
{
	text.setOutlineThickness (thickness);
}
void Framerate::draw(sf::RenderTarget&target, sf::RenderStates states) const
{
	target.draw(this->text,states);
}
Framerate::Framerate(unsigned fontsize,std::string fontname,sf::Color color)
:	fcount(0)
,	fupdate(0)
,	frame(16666.6f)
,	fmax(1000000.0f)
,	fmin(0.0f)
,	fsum(0.0f)
,	fhist{0}
,	font()
,	text()
,	timer()
{
	if (!(*this).font.loadFromFile(fontname))
	{
		throw font;
	}
	memset(fhist,0,sizeof(double)*fhs);
	text.setFont(font);
	text.setFillColor(color);
	text.setCharacterSize(fontsize);
	tick();
}
#define TICKSTOFPS(t) (\
	std::to_string(((u_int64_t)(10000000.f/t))/10u)+"."+\
	std::to_string(((u_int64_t)(10000000.f/t))%10u)\
)
void Framerate::update()
{
	text.setString(
	"max: "+(fmax!=0.f?(TICKSTOFPS(fmax)):"infinity")+
	"\navr: "+(fsum!=0.f?
	std::to_string((u_int64_t)(10000000.f*(double)fhs/fsum)/10)+"."+
	std::to_string((u_int64_t)(10000000.f*(double)fhs/fsum)%10):"infinity")+
	//"\ncur: "+(frame!=0.f?(TICKSTOFPS(frame)):"infinity")+
	"\nmin: "+(fmin!=0.f?(TICKSTOFPS(fmin)):"infinity")
	);
}
void Framerate::tick()
{
	this->frame = (this->timer.restart()).asMicroseconds();
	(++this->fcount)%=10000000000000000000U;
	this->fsum-=this->fhist[this->fcount%fhs];
	this->fsum+=(this->fhist[this->fcount%fhs]=this->frame);
	if(this->fcount < this->frame+this->fupdate)
	{
		fupdate = fcount;
		fmax = (frame+99*fmax)/100;
		fmin = (frame+99*fmin)/100;
	}
	if(frame < fmax)fmax=frame;
	if(frame > fmin)fmin=frame;
	update();
}
double Framerate::average()
{
	return (1000000.f/fsum);
}
double Framerate::max()
{
	return (1000000.f/fmax);
}
double Framerate::min()
{
	return (1000000.f/fmin);
}
