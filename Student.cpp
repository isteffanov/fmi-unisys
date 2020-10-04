#include <iostream>
#include "Student.hpp"

const char* Student::STUDENTS_FOLDER = "students\\";
const char* Student::GRADES_FOLDER = "grades\\";


Student::Student(String name, unsigned fn, Program program, unsigned group, AcadYear year, Status status, double avr)
    :m_name(name), m_fn(fn), m_program(program), m_group(group), m_year(year), m_status(status), m_avr(avr) {}

Student::Student(String name, unsigned fn, String program, unsigned group, unsigned year, char status, double avr)
    :m_name(name), m_fn(fn), m_program(Program(program)), m_group(group), m_avr(avr)
{
    switch (year) {
    case 1: m_year = AcadYear::FRESHMAN;
        break;
    case 2: m_year = AcadYear::SOPHOMORE;
        break;
    case 3: m_year = AcadYear::JUNIOR;
        break;
    case 4: m_year = AcadYear::SENIOR;
        break;
    default:
        m_year = AcadYear::UNKNOWN;
    }

    switch (status)
    {
    case 'e': m_status = Status::ENROLLED;
        break;
    case 'd': m_status = Status::DROPPED;
        break;
    case 'g' : m_status = Status::GRADUATED;
        break;
    default:
        m_status = Status::UNKNOWN;
    }
}

Student::Student(unsigned fn) 
{
    *this = Student();
    m_fn = fn;
}

String Student::getFilename() const
{
    return (String(STUDENTS_FOLDER) + String(m_fn) + String(".bin"));
}

String Student::getGradesFilename() const
{
    return (String(GRADES_FOLDER) + String(m_fn) + String(".bin"));
}

String Student::getProgramName() const
{
    return (m_program.getName());
}

inline int Student::getYearToWrite() const
{
    switch (m_year) {
    case AcadYear::FRESHMAN: return 1;
        break;
    case AcadYear::SOPHOMORE: return 2;
        break;
    case AcadYear::JUNIOR: return 3;
        break;
    case AcadYear::SENIOR: return 4;
        break;
    default:
        return 0;
    };
}

char Student::getStatusToWrite() const 
{
    switch (m_status)
    {
    case Status::ENROLLED: return 'e';
        break;
    case Status::DROPPED: return 'd';
        break;
    case Status::GRADUATED: return 'g';
        break;
    default:
        return 'u';
    }
}

double Student::getAverageGrade() const 
{
    return m_avr;
}

void Student::updateAverage(double new_grade)
{
    std::fstream stream(getFilename().ptr(), std::ios::binary | std::ios::in | std::ios::out);
    if (stream.is_open()) {
        int pos = 0;
        pos += 3 * sizeof(unsigned);
        stream.seekp(pos, std::ios::beg);

        double avr;
        unsigned grade_count;
        stream.read((char*)&avr, sizeof(double));
        stream.read((char*)&grade_count, sizeof(unsigned));

        if (grade_count == 0) avr = new_grade; //compute new average grade and increment the grade counter
        else avr = ((avr*grade_count + new_grade) / (grade_count + 1));
        grade_count++;

        stream.seekp(pos, std::ios::beg); //write them again in the file
        stream.write((const char*)&avr, sizeof(double));
        stream.write((const char*)&grade_count, sizeof(unsigned));

        stream.close();
    }
    else throw std::exception("Could not find student with that faculty number :(");
}

void Student::updateGrade(double new_grade, const String& course)
{
    std::fstream stream(getGradesFilename().ptr(), std::ios::binary | std::ios::in | std::ios::out);
    if (stream.is_open()) {
        try {
            bool found = false;
            unsigned len, pos;
            while (stream.good()) {
                char* cur_course;
                double cur_grade;

                stream.read((char*)&len, sizeof(unsigned));
                if (len == course.len()) {
                    cur_course = new char[len + 1];
                    stream.read(cur_course, len);
                    cur_course[len] = '\0';

                    if (course == cur_course) {
                        delete[] cur_course;
                        found = true;
                        pos = stream.tellg();

                        break;
                    }
                    else stream.seekg(sizeof(double), std::ios::cur);
                    delete[] cur_course;
                }
                else {
                    stream.seekg(len + sizeof(double), std::ios::cur);
                }
            }
            if (!found) throw std::exception("The course was not signed!");

            std::fstream file(getGradesFilename().ptr(), std::ios::binary | std::ios::in | std::ios::out);
            file.seekp(pos, std::ios::beg);
            file.write((const char*)&new_grade, sizeof(double)); // here we add the grade
            file.close();
            std::cout << "Grade added!" << std::endl;

            return;
        }
        catch (...) {
            std::cout << "The student with faculty number " << m_fn << " has not been signed to " << m_name << " :(" << std::endl;
            return;
        }

        stream.close();
    }
    else throw std::exception("The student with that faculty number has not been signed to the course :(");
}

double Student::findGrade(const String& program_name)
{
    std::fstream stream(getGradesFilename().ptr(), std::ios::binary | std::ios::in | std::ios::out);
    if (stream.is_open()) {
        Course course;
        while (!stream.eof()) {
            try {
                stream >> course;
                if (program_name == m_program.getName()) return course.getGrade();
            }
            catch (...) {
                break;
            }
        }
    }
    else throw std::exception("Could not open grades file :(");
    
    String error_message = String("Could not find grade for student with faculty number ") + String(m_fn) + String(" :(");
    throw std::exception(error_message.ptr());
}

void Student::addCourseInGradesFile(const String& course_name)
{
    std::fstream stream(getGradesFilename().ptr(), std::ios::binary | std::ios::in | std::ios::out | std::ios::app);
    if (stream.is_open()) {
        if (stream.good()) {
            Course course(course_name);
            stream << course;
        }
        stream.close();
    }
    else throw std::exception("Could not open grades file :(");
}

void Student::changeProgramInFile(const String& new_program)
{
    std::fstream stream(getFilename().ptr(), std::ios::binary | std::ios::in | std::ios::out);

    if (stream.is_open()) {
        int pos = 4 * sizeof(unsigned) + sizeof(double) + sizeof(char); //skips the other student data
        stream.seekg(pos, std::ios::beg);

        unsigned name_len;
        stream.read((char*)&name_len, sizeof(unsigned));
        stream.seekg(name_len, std::ios::cur);

        unsigned len = new_program.len();
        char* major = new_program.ptr();
        stream.write((const char*)&len, sizeof(unsigned));
        stream.write(major, len);

        stream.close();
    }
    else throw std::exception("Could not find student with that faculty number :(");
}

void Student::changeYearInFile(unsigned year)
{
    std::fstream stream(getFilename().ptr(), std::ios::binary | std::ios::in | std::ios::out);

    int pos = 2 * sizeof(unsigned);

    if (stream.is_open() && stream.good()) {
        stream.seekp(pos, std::ios::beg); //skips the other students data
        stream.write((const char*)&year, sizeof(unsigned));

        stream.close();
    }
    else throw std::exception("Could not find student with that faculty number :(");
}

void Student::changeGroupInFile(unsigned group)
{
    std::fstream stream(getFilename().ptr(), std::ios::binary | std::ios::in | std::ios::out);
    if (stream.is_open() && stream.good()) {
        stream.seekp(sizeof(unsigned), std::ios::beg); //skips the other students data
        stream.write((const char*)&group, sizeof(unsigned));

        stream.close();
    }
    else throw std::exception("Could not find student with that faculty number :(");

}

void Student::advanceYearInFile()
{
    unsigned year;
    std::fstream stream(getFilename().ptr(), std::ios::binary | std::ios::in | std::ios::out);

    if (stream.is_open() && stream.good()) {
        stream.seekg(2 * sizeof(unsigned), std::ios::beg);
        stream.read((char*)&year, sizeof(unsigned));

        if (year < 4) year++; //highly simplified, sowwy
        else throw std::exception("The student with that faculty number is in their final year!");

        stream.seekg(2 * sizeof(unsigned), std::ios::beg);
        stream.write((const char*)&year, sizeof(unsigned));

        stream.close();
    }
    else throw std::exception("Could not find student with that faculty number :(");
}

void Student::printStudent()
{    
    std::cout << m_fn << ": " << m_name << " from group " << m_group << ", " << m_program.getName() << ", currently ";

    switch (m_status) {
    case Status::ENROLLED: std::cout << " is enrolled in their ";
        break;
    case Status::DROPPED: std::cout << " has dropped their ";
        break;
    case Status::GRADUATED: std::cout << "has graduated";
        break;
    default:
        std::cout << "is unknown";
    }

    if (m_status != Status::GRADUATED) {
        switch (m_year) {
        case AcadYear::FRESHMAN: std::cout << "first year";
            break;
        case AcadYear::SOPHOMORE: std::cout << "second year";
            break;
        case AcadYear::JUNIOR: std::cout << "third year";
            break;
        case AcadYear::SENIOR: std::cout << "fourth year";
            break;
        default:
            std::cout << "__ year";
        }
    }

    std::cout << " with an average grade of " << std::fixed << std::setprecision(2) << m_avr << std::endl;
}

void Student::updateStatus(char new_status)
{
    std::fstream stream(getFilename().ptr(), std::ios::binary | std::ios::in | std::ios::out);
    if (stream.is_open() && stream.good()) {
        unsigned pos = 0;
        pos += 4 * sizeof(unsigned) + sizeof(double);
        stream.seekp(pos, std::ios::beg);

        stream.write((const char*)&new_status, sizeof(char));
        stream.close();
    }
    else throw std::exception("Could not find student with that faculty number :(");
}

bool Student::hasRights()
{
     std::fstream stream(getFilename().ptr(), std::ios::binary | std::ios::in | std::ios::out);

    if (stream.is_open() && stream.good()) {
        int pos = 4 * sizeof(unsigned) + sizeof(double);
        char status;
        stream.seekg(pos, std::ios::beg);
        stream.read((char*)&status, sizeof(char));

        if (status == 'd') {
            std::cout << "That student has dropped!" << std::endl;
            stream.close();
            return false;
        }
        else if (status == 'g') {
            std::cout << "That student has already graduated!" << std::endl;
            stream.close();
            return false;
        }
        else {
            stream.close();
            return true;
        }
    }
    else throw std::exception("Could not find student with that faculty number :(");
}

bool Student::hasPassedAllExams()
{
    std::fstream stream(getGradesFilename().ptr(), std::ios::binary | std::ios::in | std::ios::out);
    if (stream.is_open()) {
        while (stream.good()) {
            unsigned len;
            stream.read((char*)&len, sizeof(unsigned));
            stream.seekg(len, std::ios::cur);

            double grade;
            stream.read((char*)&grade, sizeof(double)); //find if the student has failed exams
            if (grade < 3.00f) return false;
        
        }

        stream.close();
    }
    else throw std::exception("Could open grades file :(");

    return true;
}

void Student::save()
{
    std::ofstream stream(getFilename().ptr(), std::ios::binary);
    if (stream.good()) {
        unsigned grade_count = 0;
        unsigned year = getYearToWrite();
        char status = getStatusToWrite();

        unsigned namelen = m_name.len();
        char* c_name = new char[namelen + 1];
        strcpy(c_name, m_name.ptr());

        unsigned programlen = m_program.getName().len();
        char* program_name = new char[programlen + 1];
        strcpy(program_name, m_program.getName().ptr());

        stream.write((const char*)&m_fn, sizeof(unsigned));
        stream.write((const char*)&m_group, sizeof(unsigned));
        stream.write((const char*)&year, sizeof(unsigned));
        stream.write((const char*)&m_avr, sizeof(double));
        stream.write((const char*)&grade_count, sizeof(unsigned));
        stream.write((const char*)&status, sizeof(char));
        stream.write((const char*)&namelen, sizeof(unsigned));
        stream.write(c_name, namelen);
        stream.write((const char*)&programlen, sizeof(unsigned));
        stream.write(program_name, programlen);
        
        stream.close();
    }
    else {
        String error_message = String("Could not create personal file for student with faculty number ") + String(m_fn) + String(" :(");
        throw std::exception(error_message.ptr());
    }

    std::ofstream grades(getGradesFilename().ptr(), std::ios::binary);
    if (grades.is_open()) grades.close();
    else {
        String error_message = String("Could not create grades file for student with faculty number ") + String(m_fn) + String(" :(");
        throw std::exception(error_message.ptr());
    }
}

Pair<List<Course>,List<Course>> Student::splitTakenAndFailedCourses() const
{
    List<Course> passed, failed;

    std::fstream stream(getGradesFilename().ptr(), std::ios::binary | std::ios::in | std::ios::out); //get all the signed courses
    if (stream.is_open()) {
        Course course;
        while (!stream.eof()) {
            try {
                stream >> course;
                if (course.getGrade() < 3.00f) failed.add(course);				//if the enrolled course has been failed or the exam hasnt been taken yet the course is marked as failed
                else passed.add(course);							            //however if the grade is greater or equal to 3.00 the course is marked as 
            }
            catch (...) {
                break;
            }
        }
        stream.close();
    }
    else throw std::exception("Could open grades file :(");

    Pair<List<Course>, List<Course>> pair(passed, failed);
    return pair;
}
