#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include"fhash.hpp"

int main(int argc,char*argv[])
{
	std::vector<fhash> list;
	std::string line;
	std::cerr<<"Usage:\n"
	<<"LINE_SEPERATED_HASH_RESULT | "
	<<argv[0]
	<<" > SAME_HASH_FILE\n\n";
	while(std::getline(std::cin,line))
	{
		//std::string hsh;
		fhash tmp;
		unsigned i = 0,j=0;
		while(i<line.size()&&line[i]!=' ')++i;
		j=i++;
		while(i<line.size())tmp.name.push_back(line[i++]);
		line[j] = '\0';
		tmp.hash = BigInt(line);
		list.push_back(tmp);
	}
	{	unsigned i = 0;
		while(i<list.size()-1)
		{
			bool flag = false;
			if(list[i + 1].hash == list[i].hash)
			{
				if(i&&list[i - 1].hash!=list[i].hash)
					std::cout<<std::endl;
				flag = true;
			}
			if(flag || (i && list[i - 1].hash == list[i].hash )
			) std::cout<<list[i].name<<std::endl;
			++i;
		}
	
	}
	std::cerr<<"Done."<<std::endl;
	return 0;
}
