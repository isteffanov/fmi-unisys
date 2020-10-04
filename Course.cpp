#include "Course.hpp"

const char* Course::COURSES_FOLDER = "courses\\";

Course::Course(String name, double grade)
    :m_name(name), m_grade(grade) { }

bool Course::operator==(Course const& other)
{
    return (this->m_name == other.m_name);
}

String Course::getFilepath() const
{
	return (String("courses\\") + m_name + String(".bin"));
}

String Course::getGradesFilepath(unsigned fn) const
{
	return (String("grades\\") + String(fn) + String(".bin"));
}

String Course::getName() const
{
	return m_name;
}

double Course::getGrade() const
{
	return m_grade;
}

void Course::addCourseInCoursesFile(const String& course, unsigned fn, unsigned year)
{	
	std::fstream stream(getFilepath().ptr(), std::ios::binary | std::ios::in | std::ios::out | std::ios::app);
	if (stream.is_open()) {
		if (stream.good()) {
			stream.write((const char*)&fn, sizeof(unsigned));
			stream.write((const char*)&year, sizeof(unsigned));
		}

		stream.close();
	}
	else {
		std::ofstream ofile(getFilepath().ptr(), std::ios::binary);
		if (ofile.is_open()) {
			if (ofile.good()) {
				ofile.write((const char*)&fn, sizeof(unsigned));
				ofile.write((const char*)&year, sizeof(unsigned));
			}
			ofile.close();
		}
		else throw std::exception("Could not open the course file :(");
	}
}

std::fstream& operator<<(std::fstream& stream, const Course& course)
{
	if (stream.good()) {
		unsigned len = course.m_name.len();
		char* c_course = new char[len + 1];
		strcpy(c_course, course.m_name.ptr());

		stream.write((const char*)&len, sizeof(unsigned));
		stream.write(c_course, len);
		stream.write((const char*)&course.m_grade, sizeof(double));

		delete[] c_course;
	}
	
	return stream;
}

std::fstream& operator>>(std::fstream& stream, Course& course)
{
	if (stream.good()) {
		unsigned len;
		char* c_course;
		double grade;

		stream.read((char*)&len, sizeof(unsigned));
		c_course = new char[len + 1];
		stream.read(c_course, len);
		c_course[len] = '\0';

		stream.read((char*)&grade, sizeof(double));

		course = Course(c_course, grade);
		delete[] c_course;
	}
	else throw std::exception("Could not read from the file!");
}

std::ofstream& operator<<(std::ofstream& stream, const Course& course)
{
	if (stream.good()) {
		unsigned len = course.m_name.len();
		char* c_course = new char[len + 1];
		strcpy(c_course, course.m_name.ptr());

		stream.write((const char*)&len, sizeof(unsigned));
		stream.write(c_course, len);
		stream.write((const char*)&course.m_grade, sizeof(double));

		delete[] c_course;
	}

	return stream;
}

std::ostream& operator<<(std::ostream& out, Course const& course)
{
    out << course.m_name << " - " << std::setprecision(2) << course.m_grade;
    return out;
}
