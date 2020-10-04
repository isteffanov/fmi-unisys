#pragma once
#include <iostream>
#include <iomanip>
#include <fstream>
#include "String.hpp"
#include "List.hpp"

class Course {
    const static char* COURSES_FOLDER;

    String m_name;
    double m_grade;

public:
    Course(String name = "-EMPTY-", double grade = 2.00);
    bool operator==(Course const& other);
    
    String getFilepath() const;
    String getGradesFilepath(unsigned fn) const;
    inline String getName() const;
    double getGrade() const;

    friend std::fstream& operator>>(std::fstream& stream, Course& course); //throws std::exception
    friend std::fstream& operator<<(std::fstream& stream, const Course& course);
    friend std::ofstream& operator<<(std::ofstream& stream, const Course& course);
    friend std::ostream& operator<<(std::ostream& out, Course const& course);

    void addCourseInCoursesFile(const String& course, unsigned fn, unsigned year);//throws std::exception
};

