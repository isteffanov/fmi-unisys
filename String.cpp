#include "String.hpp"

String::String(const char* string)
{
	init(string);
}

String::String(int num)
{
	m_length = (unsigned)(log10(num)) + 1;
	m_string = new char[m_length + 1];

	for (int i = m_length - 1; i >= 0; --i){
		m_string[i] = num % 10 + '0';
		num = num / 10;
	}

	m_string[m_length] = '\0';
}

String::String(String const& other)
{
	if (this != &other) 
		init(other.m_string);
}

String& String::operator=(String const& other)
{
	if (this != &other) {
		delete[] m_string;
		init(other.m_string);
	}
	return *this;
}

String::~String()
{
	delete[] m_string;
	m_length = 0;
}

void String::init(const char* string)
{
	m_length = strlen(string);
	m_string = new char[m_length + 1];
	strcpy(m_string, string);
	m_string[m_length] = '\0';
}

char* String::ptr() const
{
	return m_string;
}

unsigned String::len() const
{
	return m_length;
}

void String::add(char c)
{
	m_length++;
	char* temp = new char[m_length + 1];
	strcpy(temp, m_string);
	temp[m_length - 1] = c;
	temp[m_length] = '\0';

	delete[] m_string;
	m_string = temp;
}

void String::getln()
{
	int size = 0;
	unsigned cap = 4;
	char* buf = new char[cap];

	char c;
	std::cin.get(c);
	while (c != '\n') {
		if (std::cin.good()) {
			if (size < cap) {
				buf[size] = c;
				size++;
				std::cin.get(c);
			}
			else {
				cap *= 2;
				char* temp = new char[cap];
				strncpy(temp, buf, size);
				delete[] buf;
				buf = temp;
				temp = nullptr;
				buf[size] = c;
				size++;
				std::cin.get(c);
			}
		}
		else break;
	}

	char* rtrn = new char[size + 1];
	strncpy(rtrn, buf, size);
	delete[] buf;
	rtrn[size] = '\0';

	delete[] m_string;

	m_string = rtrn;
	m_length = size;
}

void String::getln(std::istream& stream)
{
	int size = 0;
	unsigned cap = 4;
	char* buf = new char[cap];

	char c;
	stream.get(c);
	while (c != '\n') {
		if (stream.good()) {
			if (size < cap) {
				buf[size] = c;
				size++;
				stream.get(c);
			}
			else {
				cap *= 2;
				char* temp = new char[cap];
				strncpy(temp, buf, size);
				delete[] buf;
				buf = temp;
				temp = nullptr;
				buf[size] = c;
				size++;
				stream.get(c);
			}
		}
		else break;
	}

	char* rtrn = new char[size + 1];
	strncpy(rtrn, buf, size);
	delete[] buf;
	rtrn[size] = '\0';

	delete[] m_string;

	m_string = rtrn;
	m_length = size;
}

void String::getln(std::fstream& stream)
{
	int size = 0;
	unsigned cap = 4;
	char* buf = new char[cap];

	char c;
	stream.get(c);
	while (c != '\n') {
		if (stream.good()) {
			if (size < cap) {
				buf[size] = c;
				size++;
				stream.get(c);
			}
			else {
				cap *= 2;
				char* temp = new char[cap];
				strncpy(temp, buf, size);
				delete[] buf;
				buf = temp;
				temp = nullptr;
				buf[size] = c;
				size++;
				stream.get(c);
			}
		}
		else break;
	}

	char* rtrn = new char[size + 1];
	strncpy(rtrn, buf, size);
	delete[] buf;
	rtrn[size] = '\0';

	delete[] m_string;

	m_string = rtrn;
	m_length = size;
}

List<String> String::strip(char token)
{
	List<String> list;
	int str_index = 0;
	int sub_index = 0;

	while (str_index < m_length) {
		while (str_index < m_length && m_string[str_index] != token) {
			sub_index++;
			str_index++;
		};

		String s;
		for (int i = 0; i < sub_index; ++i)
			s.add(m_string[str_index - sub_index + i]);
		list.add(s);

		sub_index = 0;
		str_index++;
	}

	return list;
}

String String::operator+(String const& other)
{
	m_length += other.m_length;
	char* temp = new char[m_length + 1];
	strcpy(temp, m_string);
	strcat(temp, other.m_string);
	
	return String(temp);
}

String& String::operator+=(String const& other)
{
	return (*this = *this + other);
}

bool String::operator==(String const& other) const
{
	return (!strcmp(m_string, other.m_string));
}

bool String::operator!=(String const& other)
{
	return (strcmp(m_string, other.m_string));
}

bool String::operator==(const char* someliteral) const
{
	return (!strcmp(m_string, someliteral));
}

bool String::operator!=(const char* someliteral)
{
	return (strcmp(m_string, someliteral));
}

char String::operator[](unsigned index)
{
	if (index < m_length)
		return m_string[index];
	else
		throw std::out_of_range("Invalid index!");
}

std::istream& operator>>(std::istream& stream, String& string)
{
	//few tests done
	int size = 0;
	int cap = 8;
	char c;
	char* buf = new char[cap];

	while (true) {
		if (stream.good()) {
			stream.get(c);
			if (c != '\n' && c != '\t' && c != ' ') {	//the operator reads to NL or whitespace
				if (size < cap) {
					buf[size] = c;
					size++;
				}
				else {									
					cap *= 2;							//if the buffer is full, create new with double the size
					char* temp = new char[cap];			//and copy the old one there
					strncpy(temp, buf, size);

					delete[] buf;
					buf = temp;
					temp = nullptr;
						
					buf[size] = c;						//and put the next character there
					size++;
				}
			}
			else break;
		}
		else break;
	}

	delete[] string.m_string;
	string.m_length = size;
	string.m_string = new char[string.m_length + 1];
	strncpy(string.m_string, buf, size);
	delete[] buf;
	string.m_string[string.m_length] = '\0';   // copying the buffer to the string safely with no excess memory									

	return stream;
}

std::ostream& operator<<(std::ostream& stream, String const& string)
{
	//few tests done
	if (stream.good()) stream.write(string.m_string, string.m_length);

	return stream;
}

std::ifstream& operator>>(std::ifstream& stream, String& string)
{
	//not tested, should work on theory
	int size = 0;
	int cap = 8;
	char c;
	char* buf = new char[cap];

	while (true) {
		if (stream.good()) {
			stream.get(c);
			if (c != '\n' && c != '\t' && c != ' ') {
				if (size < cap) {
					buf[size] = c;
					size++;
				}
				else {
					cap *= 2;
					char* temp = new char[cap];
					strncpy(temp, buf, size);

					delete[] buf;
					buf = temp;
					temp = nullptr;

					buf[size] = c;
					size++;
				}
			}
			else break;
		}
		else break;
	}

	delete[] string.m_string;
	string.m_length = size;
	string.m_string = new char[string.m_length + 1];
	strncpy(string.m_string, buf, size);
	delete[] buf;
	string.m_string[string.m_length] = '\0';

	return stream;
}

std::ofstream& operator<<(std::ofstream& stream, String const& string)
{
	//not tested, should work on theory
	if (stream.good()) stream.write(string.m_string, string.m_length);

	return stream;
}
