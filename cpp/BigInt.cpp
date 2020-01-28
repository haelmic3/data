#include "BigInt.hpp"
using namespace std;

//local

void die(){
  throw failedToCreateBigIntObject();
}
string& flip(string&a){
  char t;
  for(unsigned int i = 0; i < a.length()/2; i++){
    t = a[i];
    a[i] = a[a.size()-1-i];
    a[a.size()-1-i] = t;
  }
  return a;
}
string& unzero(string&data){
  if(data[0] == '0')
  {unsigned int i = 1,j;
    while(i < data.size() && data[i] == '0')i++;
    for(j = 0; j < data.size()-i; j++)
    {
      data[j] = data[j+i];
    }
    while(i--)data.pop_back();
  }
  return data;
}
bool string_less_than(const string&a,const string&b){
  return a.size() < b.size() || (a.size() == b.size() && a < b);
}
//end of local

BigInt::BigInt(){
  isNegative=false;
  data = "0";
}

BigInt::BigInt(long unsigned int x){
  isNegative = false;
  if(!x)data = "0";
  else while(x){
    data += '0' + x % 10;
    x/=10;
  }
  flip(data);
}
BigInt::BigInt(unsigned int x){
  isNegative = false;
  if(!x)data = "0";
  else while(x){
    data += '0' + x % 10;
    x/=10;
  }
  flip(data);
}
const std::string& BigInt::to_string()
{
	return unzero(data);
}

BigInt::BigInt(int x){
  if(!x){
    isNegative = false;
    data = "0";
  }else{
    isNegative = x < 0;
    if(isNegative)x=-x;
    while(x){
      data += '0' + x % 10;
      x/=10;
    }
    flip(data);
  }
}

BigInt::BigInt(string x){
  unsigned int i;
  data = "";
  isNegative=false;
  for(i=0; i < x.length();i++)
    if(x[i] > ' ')//we have something not white space
    { if(x[i] > '9')die();// bad data
      if(x[i] < '0' && x[i]!='-' && x[i]!='+')die(); // more bad data
      if(x[i]=='-')
      { if(data.length() > 0)break;
        else isNegative=!isNegative;
      }
      else if(x[i]!='+')data+=x[i];
    }else if(x[i] == '\0')break;
  unzero(this->data);
  if(data == "") data+='0';
  if(data=="0")isNegative=0;
}
ostream&operator << (ostream&out, BigInt const&arg){
  return out<<(arg.isNegative?"-":"")<<arg.data;
}
std::istream& operator >> (std::istream& in, BigInt& arg){
  char c;
  in>>c;
  bool neg = false;
    if ( c =='-') { neg=true; in.get(c); }
    if ( c < '0' || c > '9') {
      in.putback( c );
      if(neg)in.putback('-');
      in.setstate(std::ios_base::failbit);
      return in;
    }
    arg.isNegative = neg;
    arg.data=c;
    while(in.get(c)&&c>='0'&&c<='9')arg.data+=c;
    if ( c < '0' || c > '9') in.putback( c );
  return in;
}
//unsigned int string addition
string string_addition(string const&a,string const&b){
  string ans;
  char
    aa,
    bb
  ;
  unsigned int
    c,//catch
    cc = 0,//carry
    i,
    slen = a.length() < b.length() ? a.length() : b.length()
  ;
  for(i = 1; i <= slen; i++){
    aa = a[a.size()-i];
    bb = b[b.size()-i];
    c = aa-'0'+bb-'0';
    c+=cc;
    cc = c/10;
    ans.push_back('0'+c%10);
  }
  if(a.size() > slen)
    for(;i <= a.size(); i++){
      aa = a[a.size()-i];
      c = aa-'0'+cc;
      cc = c/10;
      ans.push_back('0'+c%10);
    }
  else if(b.size() > slen)
    for(;i <= b.size(); i++){
      bb = b[b.size()-i];
      c = bb-'0'+cc;
      cc = c/10;
      ans.push_back('0'+c%10);
    }
  if(cc!=0)ans.push_back(cc+'0');
  return flip(ans);
}
const BigInt operator + (BigInt const&a, BigInt const&b){
  string sign;
  if(a.isNegative&&b.isNegative){
    sign = "-";
  }
  else if(a.isNegative){
    BigInt temp(a);
    temp.isNegative = 0;
    return b-temp;
  }
  else if(b.isNegative){
    BigInt temp(b);
    temp.isNegative = 0;
    return a-temp;
  }
  else sign = "";
  return BigInt(sign+string_addition(a.data,b.data));
}

//this function works under the required asumption that a.magnatude > b.magnatude
string& string_subtraction(string& a,string const& b){

/**********\
case 1:

  1234567
  - 98768
  12x
   14x
    36x
     510xx
      099
 ______0____
  1135099


case 2:

  -1234567
  - -98768
  -12x
    14x
     36x
      510xx
       099
 __________
  -1135099
\**********/
  unsigned int
    i,
    j,
    ofset = a.size()-b.size()
  ;
  for(i = ofset; i < a.size(); i++)
  {
    if(a[i] < b[i-ofset]){//borrow
      j = i;
      while(a[--j]=='0');
      a[j++]--;
      while(j<i)a[j++]='9';
      a[i]+=10;
    }
    a[i] -= b[i-ofset]-'0';
  }
  return a;
}
const BigInt operator - (BigInt const&a, BigInt const&b){
  string
    ans
  ;  
  if(a.isNegative&&b.isNegative){
    BigInt temp(b);
    temp.isNegative = 0;
    return temp-a;
  }
  else if(a.isNegative){
    BigInt temp(b);
    temp.isNegative = 1;
    return a+temp;
  }
  else if(b.isNegative){
    BigInt temp(b);
    temp.isNegative = 0;
    return a+temp;
  }
  //ans is the largest string
  if(a<b){
    ans.push_back('-');
    for(unsigned int i = 0; i < b.data.length();i++)ans.push_back(b.data[i]);
    string_subtraction(ans,a.data);
  }
  else{
    if(a.data==b.data)return BigInt();
    for(unsigned int i = 0; i < a.data.length();i++)ans.push_back(a.data[i]);
    string_subtraction(ans,b.data);
  } 
  return BigInt(ans);
}
string string_multiply(string const& a, string const& b){
/************\
     123
    *456
    ____
    123*400
    +123*50
     +123*6
    
\************/
  string r,s,t="0";
  unsigned int i,j;
  char k;
  for(j = 0;j < b.length();j++)
  if(b[j]!='0'){ 
    r=a;
    s=r;
    for(k = 2;k <= b[j]-'0';k++)s=string_addition(s,r);
    for(i = j;i < b.length() - 1;i++)s+='0';
    t=string_addition(t,s);
  }
  return t;
}
const BigInt operator * (BigInt const& a, BigInt const& b){
  return BigInt((a.isNegative != b.isNegative ? "-" : "")+string_multiply(a.data,b.data));
}
string string_divide(const string& a, const string& b){
/***********\
      0
       0*
        3
     ___37
  123|4567
      4
      45*
      456
{4/1=4:.123*4=492 492<456=false:.4--:.123*3=369}
                     ^
                     <=
      456
       87*
       877
{8/1=8:.123*8=984 984<877=false:.8--:.123*7=861}
                     ^
                     <=

//termanate here because int division
      -861
       (16):mod_value
(rational(16,123))

case2:
    
    0*
   __30
  4|123
    1*
    12
{1/4=0:.12/4=3:.3*4=12?12<12=false..........}
                         ^
                       12<=12=true:.3 -_-
   -12
     0
      3
{3/4=0:.30...! 0 -_-}
     -0
     (3):mod_value -_-

\***********/
  string
    ans,
    con
  ;
  char
    c
  ;
  unsigned int
    i
  ;
  for(i = 0; i < a.size(); i++){
    con += a[i];
    c = 0;
    
    while(!string_less_than(con,b)){
      unzero(string_subtraction(con,b));
      if(++c > 9)throw BigIntDivisionError();
    }
    if(con == "0")con="";
    if (c == 0 && (ans.size() || i == a.size()-1) ) ans += '0';
    else ans += c + '0';
  }
  return ans;
}
const BigInt operator / (BigInt const& a, BigInt const& b){
  if(string_less_than(a.data,b.data))return BigInt();
  return BigInt((a.isNegative != b.isNegative ? "-" : "") + string_divide(a.data,b.data));
}
string string_mod(const string& a, const string& b){
  string
    con
  ;
  char
    c
  ;
  unsigned int
    i
  ;
  for(i = 0; i < a.size(); i++){
    con += a[i];
    c = 0;
    while(!string_less_than(con,b)&&con!="0"){
      unzero(string_subtraction(con,b));
      if(++c > 9)throw BigIntDivisionError();
    }
    if(con == "0")con="";
  }
  return con;
}
const BigInt operator % (BigInt const& a, BigInt const& b){
  if(string_less_than(a.data,b.data))return a;
  return BigInt((a.isNegative != b.isNegative ? "-" : "") + string_mod(a.data,b.data));
}

bool operator < (const BigInt& a,const BigInt& b){
  if(a.isNegative&&!b.isNegative)return 1;
  if(!a.isNegative&&b.isNegative)return 0;
  if(!a.isNegative&&!b.isNegative){
    if(a.data.length()<b.data.length())return 1;
    if(a.data.length()>b.data.length())return 0;
    return a.data<b.data;
  }
  else{//if(a.isNegative&&b.isNegative){
    if(a.data.length()<b.data.length())return 0;
    if(a.data.length()>b.data.length())return 1;
    return a.data>b.data;
  }
  return 0;
}
bool operator > (BigInt const& a, BigInt const& b){
  return b<a;
}
bool operator <= (BigInt const&a, BigInt const& b){
  return a==b||a<b;
}
bool operator >= (BigInt const&a, BigInt const& b){
  return a==b||b<a;
}

bool operator == (BigInt const& a, BigInt const& b){
  return a.isNegative==b.isNegative&&b.data==a.data;
}
bool operator != (BigInt const& a, BigInt const& b){
  return a.isNegative!=b.isNegative||b.data!=a.data;
}
