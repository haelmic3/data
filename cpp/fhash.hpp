#ifndef _F_HASH_HPP
#define _F_HASH_HPP
#include"/home/haelmic/dev/bigint/BigInt.hpp"
struct fhash
{
	std::string name;
	BigInt hash;
	fhash():name(),hash(){}
	fhash(std::string file, uint64_t salt):name(file),hash(salt){}
	friend bool operator>(fhash& a,fhash& b){return a.hash > b.hash;}
	friend bool operator>=(fhash& a,fhash& b){return a.hash >= b.hash;}
	friend bool operator<(fhash& a,fhash& b){return a.hash < b.hash;}
	friend bool operator<=(fhash& a,fhash& b){return a.hash <= b.hash;}
};
#endif
