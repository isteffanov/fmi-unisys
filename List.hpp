#pragma once
#include <iostream>
#include <fstream>
#include "Pair.hpp"

template<typename Type>
class List
{
private:
	Type* m_list;
	unsigned m_size;
	unsigned m_capacity;

	void init(unsigned capacity);
public:
	List(unsigned capacity = 2);
	List(List<Type> const& other);
	List<Type>& operator=(List<Type> const& other);
	~List();

	Type& operator[](int index);
	const Type& operator[](int index) const;
	inline unsigned size() const;

	void add(Type const thing);
	void remove(Type const& thing);
	void shrink_to_fit(); //frees unused memory
	void print() const; // prints every element of the list

	void swap(unsigned p1, unsigned p2);
	void bubbleSortAsc();  // requires operator <

	template<typename T>
	friend std::fstream& operator<<(std::fstream& stream, const List<T>& list);
	/*template<Pair<unsigned, double>>
	void bubbleSortAsc();*/
};

template<typename type>
inline List<type>::List(unsigned capacity)
{
	m_size = 0;
	init(capacity);
}

template<typename Type>
inline List<Type>::List(List<Type> const& other)
{
	if (this != &other) {
		m_size = other.m_size;
		init(other.m_capacity);
		m_size = other.m_size;
		for (int i = 0; i < m_size; i++)
			m_list[i] = other.m_list[i];
	}
}

template<typename Type>
inline List<Type>& List<Type>::operator=(List<Type> const& other)
{
	if (this != &other) {
		delete[] m_list;
		init(other.m_capacity);
		m_size = other.m_size;
		for (int i = 0; i < m_size; i++)
			m_list[i] = other.m_list[i];
	}

	return *this;
}

template<typename Type>
inline List<Type>::~List()
{
	delete[] m_list;
	m_size = 0;
	m_capacity = 0;
}

template<typename Type>
inline void List<Type>::init(unsigned capacity)
{
	m_list = new Type[capacity];
	m_capacity = capacity;
}

template<class Type>
inline Type& List<Type>::operator[](int index)
{
	if (index < m_size)	return m_list[index];
	else
		throw std::out_of_range("Invalid index!");
}

template<class Type>
inline const Type& List<Type>::operator[](int index) const
{
	if (index < m_size)	return m_list[index];
	else
		throw std::out_of_range("Invalid index!");
}

template<typename Type>
inline unsigned List<Type>::size() const
{
	return m_size;
}

template<typename Type>
inline void List<Type>::add(Type const thing)
{
	if (m_size >= m_capacity) {
		Type* temp = new Type[2 * m_capacity];
		for (int i = 0; i < m_size; ++i)
			temp[i] = m_list[i];

		delete[] m_list;
		m_list = temp;
		m_capacity *= 2;
	}
	m_list[m_size] = thing;
	m_size++;
}


template<typename Type>
inline void List<Type>::remove(Type const& thing)
{
	int index = -1;
	for (int i = 0; i < m_size; ++i) 
		if (m_list[i] == thing) index = i; //requires operator== overload

	if (index == -1) {
		std::cout << "Didnt find anything" << std::endl;
		return;
	}

	for (int i = index; i < m_size-1; ++i)
		m_list[i] = m_list[i + 1]; //requires operator= overload

	m_size--;
}

template<typename Type>
inline void List<Type>::shrink_to_fit()
{
	Type* temp = new Type[m_size];
	for (int i = 0; i < m_size; ++i)
		temp[i] = m_list[i];

	delete[] m_list;
	m_list = temp;
	m_capacity = m_size;
}

template<typename Type>
inline void List<Type>::print() const
{
	for (int i = 0; i < m_size - 1; ++i)
		std::cout << m_list[i] << ", "; // requires operator<< overload
	std::cout << m_list[m_size - 1] << std::endl;
}

template<typename Type>
inline void List<Type>::swap(unsigned p1, unsigned p2)
{
	Type temp = m_list[p1];
	m_list[p1] = m_list[p2];
	m_list[p2] = temp;
}

template<typename Type>
inline void List<Type>::bubbleSortAsc()
{
	bool changed = false;
	unsigned sorted = 0;
	do {
		changed = false;
		for (int i = m_size - 1; i > sorted; --i) {
			if (m_list[i] < m_list[i - 1]) { // requires operator <
				swap(i, i - 1);
				changed = true;
			}
		}
		sorted++;
	} while (changed);
}

template<>
inline void List<Pair<unsigned, double>>::bubbleSortAsc()		//specialzation for the gigantic protocol list, sorts by the first element(the faculty number)
{
	bool changed = false;
	unsigned sorted = 0;
	do {
		changed = false;
		for (int i = m_size - 1; i > sorted; --i) {
			if (m_list[i].first() < m_list[i - 1].first()) { // requires operator <
				swap(i, i - 1);
				changed = true;
			}
		}
		sorted++;
	} while (changed);
}

template<typename T>
inline std::fstream& operator<<(std::fstream& stream, const List<T>& list)
{
	for (int i = 0; i < list.m_size; ++i)
		stream << list[i] << std::endl;
	return stream;
}
