#include<iostream>
class BigInt{
  std::string data;
  bool isNegative;
public:
  BigInt();// Init 0
  BigInt(int x);// Init some int x converted to BigInt
  BigInt(unsigned int x);
  BigInt(long unsigned int x);
  const std::string&to_string();
  explicit BigInt(std::string x);//Init value in the string
                                 //check valid or exit1

  friend std::ostream& operator << (std::ostream&out, const BigInt& arg);
  friend std::istream& operator >> (std::istream& in, BigInt& arg);
  friend const BigInt operator + (BigInt const& a, BigInt const& b);
  friend const BigInt operator - (BigInt const& a, BigInt const& b);
  friend const BigInt operator * (BigInt const& a, BigInt const& b);
  friend const BigInt operator / (BigInt const& a, BigInt const& b);
  friend const BigInt operator % (BigInt const& a, BigInt const& b);
  friend bool operator < (BigInt const& a, BigInt const& b);
  friend bool operator > (BigInt const& a, BigInt const& b);
  friend bool operator == (BigInt const& a, BigInt const& b);
  friend bool operator != (BigInt const& a, BigInt const& b);
  friend bool operator <= (BigInt const& a, BigInt const& b);
  friend bool operator >= (BigInt const& a, BigInt const& b);
  BigInt operator++(){return *this=*this+1;}
  BigInt operator--(){return *this=*this-1;}
  BigInt operator++(int){BigInt a = *this;*this=*this+1;return a;}
  BigInt operator--(int){BigInt a = *this;*this=*this-1;return a;}
  BigInt operator+=(BigInt a){return *this=*this+a;}
  BigInt operator-=(BigInt a){return *this=*this-a;}
  BigInt operator*=(BigInt a){return *this=*this*a;}
  BigInt operator/=(BigInt a){return *this=*this/a;}
};
class failedToCreateBigIntObject{};
class BigIntDivisionError{};
std::string string_divide(const std::string& a,const std::string& b);
