#pragma once
#include "List.hpp"
#include<fstream>
#include <cstring>
#include <cmath>

class String
{
	char* m_string;
	unsigned m_length;
private:
	void init(const char* string);
public:
	String(const char* string = "");
	String(int num);
	String(String const& other);
	String& operator=(String const& other);
	~String();

	char* ptr() const;		//returns pointer to the string
	unsigned len() const;	//returns the length of the string
	void add(char c);
	void getln(); 
	void getln(std::istream& stream); 
	void getln(std::fstream& stream);
	List<String> strip(char token = ' '); //strtok inplementation, returns list of the substrings

	friend std::istream& operator>>(std::istream& stream, String& string);			//reads until whitespace or \n
	friend std::ostream& operator<<(std::ostream& stream, String const& string);
	friend std::ifstream& operator>>(std::ifstream& stream, String& string);		//reads until whitespace or \n
	friend std::ofstream& operator<<(std::ofstream& stream, String const& string);

	String operator+(String const& other);		//concatenation
	String& operator+=(String const& other);	//concatenation
	bool operator==(String const& other) const;
	bool operator!=(String const& other);
	bool operator==(const char* other) const;
	bool operator!=(const char* other);
	char operator[](unsigned index); // throws std::out_of_range 

};

