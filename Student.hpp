#pragma once
#include <iostream>
#include <iomanip>
#include <fstream>
#include "String.hpp"
#include "List.hpp"
#include "Program.hpp"
#include "Course.hpp"

enum class AcadYear {
    FRESHMAN = 0,
    SOPHOMORE,
    JUNIOR,
    SENIOR,

    UNKNOWN = -1
};

enum class Status {
    ENROLLED = 0,
    DROPPED,
    GRADUATED,

    UNKNOWN = -1
};

class Student {
    const static char* STUDENTS_FOLDER;
    const static char* GRADES_FOLDER;

    String m_name;
    unsigned m_fn;

    Program m_program;
    AcadYear m_year;
    unsigned m_group;
    Status m_status;
    double m_avr;

public:
    Student(String name = "", unsigned fn = 0, Program program = Program(), unsigned group = 0,
        AcadYear year = AcadYear::FRESHMAN, Status status = Status::ENROLLED, double avr = 2.0f);
    Student(String name, unsigned fn, String program, unsigned group,
        unsigned year, char status, double avr = 2.0f);
    Student(unsigned fn);
    
    String getFilename() const;
    String getGradesFilename() const;

    String getProgramName() const;
    inline int getYearToWrite() const;
    inline char getStatusToWrite() const;
    double getAverageGrade() const;

    void advanceYearInFile(); //throws std::exception
    void changeProgramInFile(const String& new_program); //throws std::exception
    void changeYearInFile(unsigned year); //throws std::exception
    void changeGroupInFile(unsigned group); //throws std::exception
    void updateAverage(double new_grade); //throws std::exception
    void updateGrade(double new_grade, const String& course); //throws std::exception
    double findGrade(const String& program_name); //throws std::exception
    void addCourseInGradesFile(const String& course_name); //throws std::exception
    void printStudent(); 

    void updateStatus(char new_status); //throws std::exception
    bool hasRights(); //throws std::exception
    bool hasPassedAllExams(); //throws std::exception
    void save(); //throws std::exception

    Pair< List<Course>, List<Course> > splitTakenAndFailedCourses() const; //throws std::exception
};
