#ifndef INTERPOLATE_HPP
#define INTERPOLATE_HPP
#include<sstream>
#include <cxxabi.h>
#include <stdexcept>
#include<string>
#include<cstring>
#include<stdarg.h>  
#include<iostream>
#include<typeinfo>
#include<vector>
#include <iomanip>
std::vector<std::string> formatString;
	int VectorElementsCounter;
	int tempArgumentCounter;
	int counter;
	int totalNumberOfArguments;
	int argumentCounter;
	int countOfPercent;
	bool flushFlag;
	template<int index, typename... Ts>
	struct print_tuple {
     void operator() (std::ostream& outputStream,std::tuple<Ts...>& t) {
         print_tuple<index - 1, Ts...>{}(outputStream,t);
		std::vector<std::string> s;
		std::string temp = "P";
		if(typeid(std::get<index>(t)).name() == temp.append(typeid(std::boolalpha).name()) && totalNumberOfArguments>0)
		{
		  outputStream <<std::get<index>(t);
		  totalNumberOfArguments--;
		}
		else if(typeid(std::get<index>(t)).name() == typeid(std::resetiosflags(std::ios_base::basefield)).name() && totalNumberOfArguments>0)
		{
		  outputStream <<std::get<index>(t);
		  totalNumberOfArguments--;
		}
		else if(typeid(std::get<index>(t)).name() == typeid(std::setiosflags(std::ios_base::hex)).name() && totalNumberOfArguments>0)
		{
		  outputStream <<std::get<index>(t);
		  totalNumberOfArguments--;
		
		}
		else if(typeid(std::get<index>(t)).name() == typeid(std::setbase(16)).name() && totalNumberOfArguments>0)
		{
		  outputStream <<std::get<index>(t);
		  totalNumberOfArguments--;
		}
		else if(typeid(std::get<index>(t)).name() == typeid(std::setw(13)).name() && totalNumberOfArguments>0)
		{
		  outputStream <<std::get<index>(t);
		  totalNumberOfArguments--;
		}
		else if(typeid(std::get<index>(t)).name() == typeid(std::setprecision(10)).name() && totalNumberOfArguments>0)
		{
		  outputStream <<std::get<index>(t);
		  totalNumberOfArguments--;
		}
		else if(typeid(std::get<index>(t)).name() == typeid(std::setfill('-')).name() && totalNumberOfArguments>0)
		{
		  outputStream <<std::get<index>(t);
		  totalNumberOfArguments--;
		}
		else
		{
			if((unsigned)VectorElementsCounter<(unsigned)formatString.size() && totalNumberOfArguments>0)
			{
				outputStream <<formatString.at(VectorElementsCounter)<<std::get<index>(t);
				totalNumberOfArguments--;
				VectorElementsCounter++;
			}
			
		}
			
         
     }
 };

 template<typename... Ts>
 struct print_tuple<0, Ts...> {
     void operator() (std::ostream& outputStream,std::tuple<Ts...>& t) 
	 {
		{
			 if(formatString.size()==0)
				outputStream <<std::get<0>(t);
		}
     }
 };
 template<typename... Ts>
 void print(std::ostream& outputStream,std::tuple<Ts...>& t) {
     const auto size = std::tuple_size<std::tuple<Ts...>>::value;
     print_tuple<size - 1, Ts...>{}(outputStream,t);
 }

template<typename... U>
std::ostream& operator<<(std::ostream& outputStream,std::tuple<U...> inputStream)
{
	print(outputStream,inputStream);
	if(formatString.size()>0 && (unsigned)VectorElementsCounter<(unsigned)formatString.size())
		outputStream<<formatString.at(VectorElementsCounter);
	return outputStream;
}
std::ostream& (&ffr(std::ostream&(&f)(std::ostream&)))(std::ostream&)
{	
if (f == static_cast<std::ostream& (*)(std::ostream&)>(std::flush))
{
	flushFlag = true;
}

	return f;
}

namespace cs540
{
	typedef std::exception WrongNumberOfArgs;
	/*
		Global Variables
	*/
	WrongNumberOfArgs e;
	
	
	/*
		Function declarations
	*/
	void init();
	std::vector<std::string> split1( std::string &text, char sep) ;
	void getArguments(std::stringstream &outputStream) ;
	template<typename T, typename... U> void getArguments(std::stringstream &outputStream,const T& t, U ... args) ;
	void getCountOfArguments() ;
	template<typename T, typename... U> void getCountOfArguments(const T& t, U ... args) ;
	template < typename... U > 	std::tuple<U...> Interpolate( U ...args);
	
/*
*Author:Aditya Manjrekar
*Date: 05/01/2016
*Description: argument will be output when its corresponding % is encountered in the format string
*/
	template < typename... U >
	std::tuple<U...> Interpolate( U ...args)
	{
		std::stringstream outputStream;
		std::stringstream ss;
		init();
		std::tuple<U...> storedArgsInTuple = std::make_tuple(std::forward<U>(args)...);
		ss<<std::get<0>(storedArgsInTuple);
		std::string temp = ss.str();
		formatString = split1(temp,'%');
		getCountOfArguments(args...);
		totalNumberOfArguments--;
		if(countOfPercent!=argumentCounter)
			throw(e);
		return storedArgsInTuple;//outputStream.str();
	}

/*
*Author:Aditya Manjrekar
*Date: 05/01/2016
*Description: Initialising Global values
*/
	void init()
	{
		formatString.erase(formatString.begin(),formatString.end());
		VectorElementsCounter=0;
		counter=0;
		totalNumberOfArguments = 0;
		tempArgumentCounter=0;
		argumentCounter=0;
		countOfPercent=0;
	}
/*
*Author:Aditya Manjrekar
*Date: 05/01/2016
*Description: split string on pipe symbol
*/
std::vector<std::string> split1(std::string &text, char sep) {
  std::vector<std::string> tokens;
  std::size_t start = 0, counter =0;
  while (counter < text.size()) {
  	if(counter>0)
  	{
	   if(text.at(counter)==sep && text.at(counter-1)=='\\')
			{
	 			text.erase(counter-1,1);
			}
			else
			{
			 if(text.at(counter)==sep)
				{
					 tokens.push_back(text.substr(start, counter-start));
					 start=counter+1;
					 countOfPercent++;
				}				
			}
		}
		else
		{
			 if(text.at(counter)==sep)
			{
			    tokens.push_back(text.substr(start,counter-start));
			    start=counter+1;
				countOfPercent++;			   
			}
		}
    counter = counter + 1;
  }
  if(tokens.size()>0)
  {
  	tokens.push_back(text.substr(start,counter-start));
  }
  else if(tokens.size()==0)
  {
	  tokens.push_back(text);
  }
  return tokens;
}
/*
*Author:Aditya Manjrekar
*Date: 05/01/2016
*Description: Recursive base function that counts the arguments
*/

	void getCountOfArguments() 
	{
		if(tempArgumentCounter>0)
			{
				//argumentCounter++;
				totalNumberOfArguments++;
			}
		tempArgumentCounter++;
	}
	template<typename T, typename... U> void getCountOfArguments(const T& t, U ... args) 
	{
		std::string a("P");
		std::string s(typeid(std::boolalpha).name());
		a.append(s);
		if(tempArgumentCounter>0)
			{
				if(typeid(t).name() == a)
				{
				  ;
				}
				else if(typeid(t).name() == typeid(std::resetiosflags(std::ios_base::basefield)).name())
				{
					;
				}
				else if(typeid(t).name() == typeid(std::setiosflags(std::ios_base::hex)).name())
				{
				  ;
				}
				else if(typeid(t).name() == typeid(std::setbase(16)).name())
				{
				  ;
				}
				else if(typeid(t).name() == typeid(std::setw(13)).name())
				{
				  ;
				}
				else if(typeid(t).name() == typeid(std::setprecision(10)).name())
				{
				  ;
				}
				else if(typeid(t).name() == typeid(std::setfill('-')).name())
				{
				  ;
				}
				else if(flushFlag==true)
				{
					flushFlag=false;
				}
				else
				{
					argumentCounter++;
				}
				totalNumberOfArguments++;
			}
			tempArgumentCounter++;
			getCountOfArguments(args...);

	}

}
#endif
