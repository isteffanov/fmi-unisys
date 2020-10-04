#pragma once
#include <fstream>
#include "String.hpp"
#include "List.hpp"

struct Program
{
private:
	const static char* PROGRAMS_FOLDER;

	String programs_name;
	
public:
	Program(const String& name = "");

	String getFilename() const;
	String getFilename(const String& program) const;
	String getName() const;

	void createEntry(unsigned fn); //throws std::exception
	void advanceEntry(unsigned fn); //throws std::exception
	void moveEntry(unsigned fn, Program& new_program); //throws std::exception
	void changeYearInProgramsFile(unsigned fn, unsigned year); //throws std::exception
	List<unsigned> getOneYear(unsigned year); //throws std::exception
};

