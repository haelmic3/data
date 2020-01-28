#include<cmath>
#include<iostream>
#include<sstream>
#include<fstream>
#include<vector>
#include<list>
#include<string>
#include<thread>
#include<chrono>
#include<Magick++.h>
#include<SFML/Graphics.hpp>
#include"Framerate.hpp"
union uint2{struct{unsigned a;unsigned b;};unsigned c[2];unsigned operator[](unsigned d){return c[d];}};
uint2 box(unsigned L)
{
	unsigned i=1;
	while((i * i)<L+1)
	{
		if(i*i>=L)return {i,i};
		if((i+1)*i>=L)return {i,i+1};
		if((i+1)*(i+1)>=L)return {i+1,i+1};
		if((i+1)*(2+i)>=L)return {i+1,i+2};
		i++;
	}
	return {i,i};
}
void setupErrorFile()
{
	std::ofstream error;
	error.open
	(
		"error.txt",
		std::ofstream::out | std::ofstream::trunc
	);
	error.close();
}
void fillList(std::vector<std::vector<std::string>>*list)
{
	std::string line;
	while(std::getline(std::cin,line))
	{
		std::vector<std::string> group;
		do{
			if(line.size()<3)break;
			group.push_back(std::string(line));
		}while(std::getline(std::cin,line));
		list->push_back(group);
	}
}
class xen:public sf::Drawable 
{
	sf::Sprite spr;
	std::vector<sf::Texture> texz;
	std::vector<sf::Texture>::iterator tit;
	void spriteSetup(
		unsigned fns,
		unsigned j,
		sf::Vector2u wsize,
		sf::Vector2u size
	){
		unsigned row, per;
		float padx = 0, pady = 0, scalex, scaley;
		{	uint2 p= box(fns);  row = p[0];  per = p[1];
			scalex = ((float)wsize.x/per);
			scaley = ((float)wsize.y/row);
		}
		if(scalex/size.x<=scaley/size.y)
		{
			spr.setScale(	scalex/size.x,	scalex/size.x	);
			pady =(	scaley - size.y	*	scalex / size.x	) / 2;
		}else{
			spr.setScale(	scaley/size.y,	scaley/size.y	);
			padx =(	scalex - size.x	*	scaley / size.y	) / 2;
		}
		spr.setPosition(	(j%per)*scalex+padx,	(j/per)*scaley+pady	);
	}
	void draw(sf::RenderTarget&target,sf::RenderStates states)const
	{
		target.draw(spr,states);
	}
 public:
	void tick()
	{
			if(++tit==texz.end())tit = texz.begin();
			spr.setTexture(*(tit));
	}
	xen():spr(),texz(),tit(texz.end()){}
	xen
	(
		std::string fname,
		unsigned fns,
		unsigned j,
		sf::Vector2u&wsize
	)
	{
		std::list<Magick::Image>imgl;
		{ // imgl init
			try
			{
				Magick::readImages(&imgl,fname);
			}
			catch(Magick::Exception &e)
			{
				std::cerr<<"xen("<<__LINE__<<") "<<e.what()<<std::endl;
				imgl.clear();
				imgl.push_back(Magick::Image());
			}
			catch(...){std::cerr<<"xen("<<__LINE__<<") ..."<<std::endl;}
			try
			{
				Magick::Geometry mgkg = ( * imgl.begin() ).size();
				spriteSetup(fns, j, wsize,
					sf::Vector2u( mgkg.width(), mgkg.height() )
				);
			}
			catch(...){std::cerr<<"... "<<__LINE__<<std::endl;}
		} // end of imgl init
		{ // texture generation
			std::list<Magick::Image>::iterator img;
			Magick::Blob mkb;
			img = imgl.begin();
			if(img!=imgl.end())
			{
				sf::Texture tex;
				try{	img->write( &mkb );	}catch(...)
				{	std::cerr<<"xen("<<__LINE__<<") ..."<<std::endl;	}
				tex.loadFromMemory(mkb.data(),mkb.length());
				texz.push_back(tex);
			}
			while(++img!=imgl.end())
			{
				Magick::Image imkt;
				sf::Texture tex;
				try
				{
					imkt.read( mkb );
					{
						Magick::Geometry mg = img->page();
						imkt.composite
						(	*img
						,	mg
						,	Magick::OverCompositeOp
						);
					}
					imkt.write( &mkb );
				}catch(Magick::Exception &e)
				{
					std::cerr<<"xen("<<__LINE__<<") "<<e.what()<<std::endl;
				}
				catch(...){std::cerr<<"xen("<<__LINE__<<") ..."<<std::endl;}

				tex.loadFromMemory(mkb.data(),mkb.length());
				texz.push_back(tex);
			}
			spr.setTexture(*(tit=texz.begin()));
		}
	}
};
int main(int argc, char*argv[])
{
	sf::RenderWindow window;
	std::vector<std::vector<std::string>> list;
	bool debug = false;
	Framerate fps(60,"FreeMono.ttf",sf::Color(255,220,200));
	fps.setOutlineColor(sf::Color::Black);
	fps.setOutlineThickness(5.5);
	{
		std::cerr
		<<"Usage:"<<*argv
		<<" < newline separated groups\
		of newline separated paths to images"
		<<std::endl;
	}
	Magick::InitializeMagick(*argv);
	fillList(&list);
	window.create(
		sf::VideoMode::getFullscreenModes()[0],
		"Image Comparison",sf::Style::None
	);
	window.setPosition({0,0});
	window.clear(sf::Color(31,0,0,255));
	fps.tick();
	window.draw(fps);
	window.display();// splash screen
// BEGIN
{
	std::chrono::microseconds
		touch(100000) // enough time to react
	,	tic(touch*4) // enough time to ponder 
	,	mintic(1)
	;
	unsigned framerate = 66667;
	auto i = list.begin();
	sf::Vector2u wsize = window .getSize();

	while(window.isOpen())
	{ // view entire list group by group
		bool viewable = true;
		std::vector<std::string>&fname = i==list.end()?*(i=list.begin()):*i;
		unsigned
			fns = fname.size()
		;
		xen imagez[fns];
		std::chrono::steady_clock::time_point
			tpoint = std::chrono::steady_clock::now()
		;
		#pragma omp parallel for
		for(unsigned j = 0; j < fns; j++)
		{
			imagez[j] = xen(fname[j],fns,j,wsize);
		}
		{
			auto mtime = std::chrono::steady_clock::now()-tpoint;
			if(touch>mtime)
			{
				std::this_thread::sleep_for(touch-mtime);
			}
		}
		while(viewable)
		{
			unsigned count = 0,j;
			tpoint = std::chrono::steady_clock::now();
			window.clear(sf::Color(0,0,0,127));
			j = fns;
			while (j) window.draw(imagez[fns - j--])
			;
			if(debug){
				window.draw(fps);
			}
			window.display();
			j = fns;
			while (j) imagez[fns - j--].tick()
			;
			{
				auto mtime = std::chrono::steady_clock::now()-tpoint;
				count += mtime / mintic;
			}
			while(viewable && window.isOpen())
			{ // stay on selected page
				sf::Event e;
				tpoint = std::chrono::steady_clock::now();
				while(window.pollEvent(e))
				{
					if(e.type == sf::Event::Closed)window.close();
				}
				if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				{
					window.close();
					exit(0);
				}
				if(
					sf::Keyboard::isKeyPressed
					(	sf::Keyboard::F	)
				){
					unsigned timeout = 1000;
					while(timeout--&&
						sf::Keyboard::isKeyPressed
					(	sf::Keyboard::F	)
					)std::this_thread::sleep_for(tic);
					debug = !debug;
				}
				if(
					sf::Keyboard::isKeyPressed
					(	sf::Keyboard::Left	)
				){
					unsigned timeout = 1000;
					while(timeout--&&
						sf::Keyboard::isKeyPressed
					(	sf::Keyboard::Left	)
					)std::this_thread::sleep_for(tic);
					viewable = false;
					if(i==list.begin())i = list.end();
					--i;
					//break; // viewable = false stops 2 loops and sends to outmost
				}else if
					(	sf::Keyboard::isKeyPressed
						(	sf::Keyboard::Return	)||
						sf::Keyboard::isKeyPressed
						(	sf::Keyboard::Space	)||
						sf::Keyboard::isKeyPressed
						(	sf::Keyboard::Right	)
					)
				{
					{
						unsigned timeout = 1000;
						while(timeout--&&(
							sf::Keyboard::isKeyPressed
							(	sf::Keyboard::Return	)||
							sf::Keyboard::isKeyPressed
							(	sf::Keyboard::Space	)||
							sf::Keyboard::isKeyPressed
							(	sf::Keyboard::Right	)
						))std::this_thread::sleep_for(tic);
					}
					if(i<list.end()){
						++i;
						viewable = false;
						//break; // viewable = false stops 2 loops and sends to outmost
					}
					else
					{
						viewable = false;
						i = list.end();
						i--;
						window.clear(sf::Color(0,0,63,255));
						if(debug){
							window.draw(fps);
						}
						window.display();
						std::this_thread::sleep_for(tic);
					}
				}else
				{
					if(count < framerate)
					{
						auto mtime = std::chrono::steady_clock::now()-tpoint;
						count += mtime / mintic;

						//tic = fulltic;
						//std::this_thread::sleep_for(mintic);
					}
					else // crutial that the inner loop is broken
					{
						fps.tick();
						break; // iterate animation
					}
				}
			}// while on page
		}// viewable loop to update page
		window.clear(sf::Color(63,0,0,255));
		for (unsigned j = fns; j>0; j--)
		{
			window.draw(imagez[fns-j]);
		}
		if(debug){
			window.draw(fps);
		}
		window.display();
	}// while application running
} // END
	return 0;
}
