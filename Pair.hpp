#pragma once
#include <fstream>

template<typename A, typename B>
struct Pair {
private:
	A elem1;
	B elem2;

	static const float EPS;
	static const int PRECISION = 2;

public:
	Pair(A e1 = A(), B e2 = B());
	Pair(const Pair<A, B>& other);

	const A& first() const;
	A& first();
	const B& second() const;
	B& second();
	bool operator==(const Pair<A, B>& other) const;
	bool operator<(const Pair<A, B>& other) const;

	template<typename X, typename Y>
	friend std::fstream& operator<<(std::fstream& stream, const Pair<X, Y>& pair);
};

template<typename A, typename B>
inline Pair<A, B>::Pair(A e1, B e2)
	:elem1(e1), elem2(e2) {}

template<typename A, typename B>
inline Pair<A, B>::Pair(const Pair<A, B>& other)
{
	if (this != &other) {
		elem1 = other.elem1;
		elem2 = other.elem2;
	}
}

template<typename A, typename B>
inline const A& Pair<A, B>::first() const
{
	return elem1;
}

template<typename A, typename B>
inline const B& Pair<A, B>::second() const
{
	return elem2;
}

template<typename A, typename B>
inline A& Pair<A, B>::first()
{
	return elem1;
}

template<typename A, typename B>
inline B& Pair<A, B>::second()
{
	return elem2;
}

template<typename A, typename B>
inline bool Pair<A, B>::operator==(const Pair<A, B>& other) const
{
	return (elem1 == other.elem1 && elem2 == other.elem2);
}

template<typename A, typename B>
inline bool Pair<A, B>::operator<(const Pair<A, B>& other) const
{
	return (elem1 < other.elem1);
}

template<typename X, typename Y>
inline std::fstream& operator<<(std::fstream& stream, const Pair<X, Y>& pair)
{
	stream << pair.elem1 << " : " << pair.elem2;
	return stream;
}
