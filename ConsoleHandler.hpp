#pragma once
#include<iostream>
#include <fstream>
#include "List.hpp"
#include "Pair.hpp"
#include "String.hpp"
#include "Course.hpp"
#include "Program.hpp"
#include "Student.hpp"

class ConsoleHandler
{
	String filepath;
	String command;
	std::fstream stream;

	void getCommmand();
	void open();
	void close();
	void help();

	void enroll(); //signs given student as a freshman
	void advance();	//signs given students for the next year
	void change(); //changes one of the following: program, group, year
	void graduate(); //sets the student as graduated
	void interrupt(); //removes student rights
	void resume(); //grants yet again the student rights
	void print(); //prints given students's info
	void printall(); //prints the info of all the students in a given program and year
	void enrollin(); //signs a student in a course
	void addgrade();
	void protocol(); //for a given course prints a protocol for all students
	void report(); //prints all the passed and all the failed exams of a student

	void changeProgram(unsigned fn);
	void changeGroup(unsigned fn);
	void changeYear(unsigned fn);

	void printHelper(unsigned fn);
	void printReport(unsigned fn, const List<Course>& passed, const List<Course>& failed, double average);

	void protocolAddHelper(unsigned fn, const String& major, double grade, List<Pair<String, List<Pair<unsigned, double>>>>& protocol_of_some_year);
	void protocolCreator(unsigned year, List<Pair<String, List<Pair<unsigned, double>>>>& list);
	Student getStudent(unsigned fn);

public:
	ConsoleHandler();
	void begin();

};

