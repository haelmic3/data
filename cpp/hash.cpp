#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<thread>
#include<mutex>
#include<functional>
#include<chrono>
#include<iomanip>
#include"/home/haelmic/dev/quicksort/quicksort.hpp"
#include"fhash.hpp"

#define THREADS 14
std::mutex mxprint;

void hash(std::vector<fhash*>&list, const std::string label)
{
	unsigned i = 0;
	unsigned const len = list.size();
	std::ifstream f;
	while(i < len)
	{
		unsigned q = 0;
		char c;
		std::stringstream out;
		f.open(list[i]->name);
		if(!f)
		{
			mxprint.lock();
			out<<label<<" failed to open ‘"<<list[i]->name<<"’.\n";
			std::cerr<<out.str()<<std::flush;
			mxprint.unlock();
			++i;
			continue;
		}
		mxprint.lock();
		out<<label<<" "<<i+1<<"/"<<len<<": "<<list[i]->name<<"\n";
		std::cerr<<out.str()<<std::flush;
		mxprint.unlock();
		while(f.get(c))
		{
			uint64_t t = c;
			q++;
			//if(t>255)t=~t+1;
			list[i]->hash += t;
		}
		/*if(q)list[i]->hash /= q;
		else
		{
			list[i]->hash = 0;
		}/ **/
		++i;
		f.close();
	}
}
int main(int argc, char*argv[])
{
	unsigned i = 0,k,threads=THREADS;
	std::vector<fhash> list;
	std::chrono::high_resolution_clock::time_point start,now,last;
	std::string name;
	std::ios_base::sync_with_stdio(false);
	std::cerr<<"Usage:\n"<<argv[0]<<"< LINE_SEPERATED_HARDPATH_FILENAMES_FILE > RESULT_FILE\n\n";
	start = now = std::chrono::high_resolution_clock::now();
	while(std::getline(std::cin,name))
	{
		list.push_back(fhash(name,0));
	}
	if(threads>list.size())
	{threads = list.size();}

	{
	std::vector<fhash*> list_t[threads];
	while(i<list.size())for(k=0; k < threads; k++)
	{
			list_t[k].push_back(&list[i++]);
			if(i>=list.size())break;
	}
	{
		std::thread tt[threads];
		for(i=0;i<threads;i++)
		{
			std::stringstream st;
			st<<"Thread "<<i;
			tt[i] = std::thread
			(std::bind
				(
					hash
					,
					(std::vector<fhash*> &)list_t[i]
					,
					st.str()
				)
			);
		}
		for(i=0;i<threads;i++)
		{
			std::cout.flush();
			std::cerr.flush();
			tt[i].join();
			std::cerr<<"Thread "<<i<<" OK."<<std::endl;
		}
	}
	i = 0;
	for(i=0;i<list.size();i++)std::cerr<<list[i].hash<<" ";
	std::cerr<<std::endl;
	}

	std::cerr << std::setw( 9 )
		<<(std::chrono::duration_cast<std::chrono::duration<double>>
			(
				(
				now =
				std::
				chrono::
				high_resolution_clock::
				now()
				) - start
			)
		).count()
			<< " : "<< std::setw( 9 )<< std::chrono::duration_cast
				<std::chrono::duration<double>>
					(now-start).count() << "sec. "
		<<"Starting quicksort(list)"<<std::endl;
	quicksort(list);
	std::cerr << std::setw( 9 )
		<<(std::chrono::duration_cast<std::chrono::duration<double>>
			(
				(
				now =
				std::
				chrono::
				high_resolution_clock::
				now()
				) - start
			)
		).count()
			<< " : " << std::setw( 9 ) << std::chrono::duration_cast
				<std::chrono::duration<double>>
					(now-start).count() << "sec. "
		<<"Listing files."<<std::endl;
	i = 0;
	while(i < list.size())
	{
		std::cout
			<<list[i].hash.to_string()
			<<" "
			<<list[i].name<<std::endl;
		++i;
	}
	std::cerr << std::setw( 9 )
		<<(std::chrono::duration_cast<std::chrono::duration<double>>
			(
				(
				now =
				std::
				chrono::
				high_resolution_clock::
				now()
				) - start
			)
		  ).count()
			<< " : " << std::setw( 9 ) << std::chrono::duration_cast
				<std::chrono::duration<double>>
					(now-start).count() << "sec. Finished" << std::endl;
	return 0;
}
