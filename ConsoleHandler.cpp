#include "ConsoleHandler.hpp"

ConsoleHandler::ConsoleHandler()
	:filepath(String()), command(String()) { }

void ConsoleHandler::begin()
{	
	std::cout << "Welcome to UniSys! \n" << std::endl;

	while (true) {
		getCommmand(); //the value is given to the "command" field

		if (command == "open") open();
		else if (command == "close") close();
		else if (command == "help") help();
		// . . .

		else if (command == "enroll") enroll();	//the student's info is entered into the database
		else if (command == "advance") advance(); //year++
		else if (command == "change") change(); //either student's group, program or year are changed
		else if (command == "graduate") graduate();
		else if (command == "interrupt") interrupt(); //removes student's rights
		else if (command == "resume") resume(); //grants student's rights again
		else if (command == "print") print(); //student's info is printed
		else if (command == "printall") printall(); //prints the info of students from given course and year
		else if (command == "enrollin") enrollin(); //signs a student in a course
		else if (command == "addgrade") addgrade();
		else if (command == "protocol") protocol();
		else if (command == "report") report();
		else if (command == "exit") {
			if (stream.is_open()) stream.close();
			break;
		}
		else std::cout << "Unknown command! Enter help for more information :)" << std::endl;
			
	}
	return;
}

void ConsoleHandler::getCommmand()
{
	std::cout << "Enter a command: ";
	std::cin >> std::skipws >> command;
}

void ConsoleHandler::open()
{
	String filepath;
	std::cin >> filepath;

	stream.open(filepath.ptr(), std::ios::binary | std::ios::in | std::ios::out);
	if (!stream.is_open()) {
		std::cout << "File cannot be found! :(" << std::endl;
	}
}

void ConsoleHandler::close()
{
	if (stream.is_open()) stream.close();
	else std::cout << "No file to close!" << std::endl;
}

void ConsoleHandler::help()
{
	std::cout << "These are all the functions in the program: \n" << std::endl;

	std::cout << "enroll <fn> <program> <group> <name>" << std::endl;
	std::cout << "advance <fn>" << std::endl;
	std::cout << "change <fn> <option> <value>" << std::endl;
	std::cout << "graduate <fn>" << std::endl;
	std::cout << "interrupt <fn>" << std::endl;
	std::cout << "resume <fn>" << std::endl;
	std::cout << "print <fn>" << std::endl;
	std::cout << "printall <program> <year>" << std::endl;
	std::cout << "enrollin <fn> <course>" << std::endl;
	std::cout << "addgrade <fn> <course> <grade>" << std::endl;
	std::cout << "protocol <course>" << std::endl;
	std::cout << "report <fn>" << std::endl;
}

void ConsoleHandler::enroll()
{
	String name, program_name;
	unsigned fn;
	unsigned group;

	std::cin >> fn;
	std::cin.get();
	std::cin >> program_name;
	std::cin >> group;
	std::cin.get();
	name.getln();

	Student student = Student(name, fn, program_name, group);
	Program program(program_name);
	try {
		program.createEntry(fn);
		student.save();
		std::cout << "Student with faculty number:" << fn << " just signed in their first year!" << std::endl;
	}
	catch (std::exception& e) {
		std::cout << "An error occured: " << e.what() << std::endl;
	}
}

void ConsoleHandler::advance()
{
	unsigned fn;
	std::cin >> fn;

	try {
		Student student = getStudent(fn);
		Program program = student.getProgramName();
		if (!student.hasRights()) {
			std::cout << "The student with that faculty number does not have student the right to advance years! :( " << std::endl;
			std::cin.get();
			return;
		}

		student.advanceYearInFile();
		program.advanceEntry(fn);
		std::cin.get();
	}
	catch (std::exception& e) {
		std::cout << "An error occured: " << e.what() << std::endl;
	}
}

void ConsoleHandler::change()
{
	unsigned fn;
	String option;
	std::cin >> fn;
	std::cin.get(); //TODO: fix operator>>
	std::cin >> option;

	try {
		Student student = getStudent(fn);
		if (!student.hasRights()) {
			std::cout << "The student with that faculty number does not have student rigth and can not change their program, group or year! :( " << std::endl;
			std::cin.get();
			return;
		}
	}
	catch (std::exception& e) {
		std::cout << "An error occured: " << e.what() << std::endl;
	}
	

	if (option == "program") changeProgram(fn);
	else if (option == "group") changeGroup(fn);
	else if (option == "year") changeYear(fn);
	else std::cout << "Allowed change options are only 'program', 'group' and 'year'! [change <fn> <option> <value>]" << std::endl;

}

void ConsoleHandler::graduate()
{
	unsigned fn;
	std::cin >> fn;
	
	try {
		Student student = getStudent(fn);
		if (!student.hasPassedAllExams()) {
			std::cout << "The student with that faculty number has failed one of their exams and cannot graduate yet! :(" << std::endl;
			std::cin.get();
			return;
		}

		student.updateStatus('g');
		std::cout << "Student with faculty number:" << fn << " just graduated!" << std::endl;
		std::cin.get();
	}
	catch (std::exception& e) {
		std::cout << "An error occured: " << e.what() << std::endl;
	}
}

void ConsoleHandler::interrupt()
{
	unsigned fn;
	std::cin >> fn;

	try {
		Student student = getStudent(fn);
		student.updateStatus('d');
		std::cout << "Student with faculty number:" << fn << " just dropped!" << std::endl;
		std::cin.get();
	}
	catch (std::exception& e) {
		std::cout << "An error occured: " << e.what() << std::endl;
	}
}

void ConsoleHandler::resume()
{
	unsigned fn;
	std::cin >> fn;

	try {
		Student student = getStudent(fn);
		student.updateStatus('e');
		std::cout << "Student with faculty number:" << fn << " continued their study!" << std::endl;
		std::cin.get();
	}
	catch (std::exception& e) {
		std::cout << "An error occured: " << e.what() << std::endl;
	}
}

void ConsoleHandler::print()
{
	unsigned fn;
	std::cin >> fn;
	
	printHelper(fn);
	std::cin.get();
}

void ConsoleHandler::printall()
{
	String program;
	unsigned year;

	std::cin >> program >> year;
	
	Program p(program);
	try {
		List<unsigned> fns = p.getOneYear(year);
		for (int i = 0; i < fns.size(); ++i)
			printHelper(fns[i]);
		std::cin.get();
	}
	catch (std::exception& e) {
		std::cout << "An error occured: " << e.what() << std::endl;
	}
}

void ConsoleHandler::enrollin()
{
	unsigned fn;
	String course;
	std::cin >> fn;
	std::cin.get();
	course.getln();
	
	Course course_obj(course);
	try {
		Student student = getStudent(fn);
		if (!student.hasRights()) {
			std::cout << "This student does not have rights to enroll in courses!" << std::endl;
			return;
		}

		student.addCourseInGradesFile(course);
		course_obj.addCourseInCoursesFile(course, fn, student.getYearToWrite());
		//student.updateAverage(2.00f);

		std::cout << "Student with faculty number:" << fn << " successfully enrolled in " << course << "!" << std::endl;
	}
	catch (std::exception& e) {
		std::cout << "An error occured: " << e.what() << std::endl;
	}
}

void ConsoleHandler::addgrade()
{
	unsigned fn;
	String course;
	double grade;
	std::cin >> fn;
	std::cin >> grade;
	std::cin.get();
	course.getln();

	if (grade <= 2.00 || grade > 6.00) {
		std::cout << "Invalid grade!" << std::endl;
		return;
	}

	try {
		Student student = getStudent(fn);
		if (!student.hasRights()) {
			std::cout << "The student does not have rights to visit exams!" << std::endl;
			return;
		}

		student.updateAverage(grade);
		student.updateGrade(grade, course);
	}
	catch (std::exception& e) {
		std::cout << "An error occured: " << e.what() << std::endl;
		return;
	}	
}

void ConsoleHandler::protocol()
{
	String course;
	course.getln();

	List<Pair<String, List<Pair<unsigned, double>> >> first_year, second_year, third_year, forth_year;	//the first pair is for each program and all the students in 
																										//the second is for each student in the program - their fn and grade
	unsigned cur_year, cur_fn;

	Course course_obj(course);
	String filename = course_obj.getFilepath();
	stream.open(filename.ptr(), std::ios::binary | std::ios::in | std::ios::out);
	if (stream.is_open()) {

		while (!stream.eof()) {
			stream.read((char*)&cur_fn, sizeof(unsigned));
			stream.read((char*)&cur_year, sizeof(unsigned));
			if (!stream.good()) break;
			try {
				Student student = getStudent(cur_fn);
				String program = student.getProgramName();
				double cur_grade = student.findGrade(program);

				switch (cur_year) {
				case 1:
					protocolAddHelper(cur_fn, program, cur_grade, first_year);
					break;
				case 2:
					protocolAddHelper(cur_fn, program, cur_grade, second_year);
					break;
				case 3:
					protocolAddHelper(cur_fn, program, cur_grade, third_year);
					break;
				case 4:
					protocolAddHelper(cur_fn, program, cur_grade, forth_year);
					break;
				}
			}
			catch (std::exception& e) {
				std::cout << "An error occured: " << e.what() << std::endl;
			}
			catch (...) {
				std::cout << "An unknown error occured!" << std::endl;
			}
		}

		first_year.shrink_to_fit();
		protocolCreator(1, first_year);
		second_year.shrink_to_fit();
		protocolCreator(2, second_year);
		third_year.shrink_to_fit();
		protocolCreator(3, third_year);
		forth_year.shrink_to_fit();
		protocolCreator(4, forth_year);
	}
}


void ConsoleHandler::report()
{
	List<Course> passed, failed;
	Pair<List<Course>, List<Course>> all_courses; // first element is passed courses, second is failed

	unsigned fn;
	std::cin >> fn;
	try {
		Student student = getStudent(fn);
		all_courses = student.splitTakenAndFailedCourses();
		passed = all_courses.first();
		failed = all_courses.second();
		double average = student.getAverageGrade();

		printReport(fn, passed, failed, average); //print all that jazz
	}
	catch (std::exception& e) {
		std::cout << "An error occured: " << e.what() << std::endl;
	}
}

void ConsoleHandler::changeProgram(unsigned fn)
{
	//TODO: value check
	String new_major;
	std::cin >> new_major;

	try {
		Student student = getStudent(fn);
		String old_major = student.getProgramName();
		if (new_major == old_major) {
			std::cout << "The student already studies " << old_major << "!" << std::endl;
			return;
		}
		Program program_old(old_major);
		Program program_new(new_major);

		student.changeProgramInFile(new_major);
		program_old.moveEntry(fn, program_new);
		std::cout << "Student with faculty number:" << fn << " changed programs!" << std::endl;
	}
	catch (std::exception& e) {
		std::cout << "An error occured: " << e.what() << std::endl;
		std::cin.get();
	}
}

void ConsoleHandler::changeGroup(unsigned fn)
{
	unsigned value;
	std::cin >> value;

	try {
		Student student = getStudent(fn);
		student.changeGroupInFile(value);

		std::cout << "Student with faculty number:" << fn << " changed groups!" << std::endl;
		std::cin.get();
	}
	catch (std::exception& e) {
		std::cout << "An error occured: " << e.what() << std::endl;
	}
}

void ConsoleHandler::changeYear(unsigned fn)
{
	unsigned value;
	std::cin >> value;

	if (value == 0 || value > 4) {
		std::cout << "Invalid year, please try again :(" << std::endl;
		return;
	}

	try {
		Student student = getStudent(fn);
		Program program(student.getProgramName());
		if (!student.hasRights()) {
			std::cout << "No student found or the student does not have the rights to change years!" << std::endl;
			return;
		}
		student.changeYearInFile(value);
		program.changeYearInProgramsFile(fn, value);
		std::cout << "Student with faculty number:" << fn << " changed years!" << std::endl;
		std::cin.get();
	}
	catch (std::exception& e) {
		std::cout << "An error occured: " << e.what() << std::endl;
	}
}

void ConsoleHandler::printHelper(unsigned fn)
{
	try {
		Student student = getStudent(fn);
		student.printStudent();
	}
	catch (std::exception& e) {
		std::cout << "An error occured: " << e.what() << std::endl;
	}
}

void ConsoleHandler::printReport(unsigned fn, const List<Course>& passed, const List<Course>& failed, double average)
{
	std::cout << "Faculty number " << fn << ":" << std::endl;

	std::cout << "Passed exams: " << std::endl;
	if (passed.size() == 0) std::cout << "None!" << std::endl;
	passed.print();
	
	std::cout << "Failed exams: " << std::endl;
	if (failed.size() == 0) std::cout << "None!" << std::endl;
	else failed.print();

	std::cout << "With an average grade of " << std::fixed << std::setprecision(2) << average << std::endl;
	std::cin.get();
}

void ConsoleHandler::protocolAddHelper(unsigned fn, const String& major, double grade, List<Pair<String, List<Pair<unsigned, double>> >>& protocol_of_some_year)
{
	bool found = false;
	for (int i = 0; i < protocol_of_some_year.size(); ++i) {
		if (protocol_of_some_year[i].first() == major) {
			Pair<unsigned, double> pair(fn, grade);
			protocol_of_some_year[i].second().add(pair);
			found = true;
			break;
		}
	}
	if (!found) {
		List<Pair<unsigned, double>> fns_n_grades;
		Pair<unsigned, double> fn_n_grade(fn, grade);
		fns_n_grades.add(fn_n_grade);
		Pair<String, List<Pair<unsigned, double>>> pair(major, fns_n_grades);
		protocol_of_some_year.add(pair);
	}
}

void ConsoleHandler::protocolCreator(unsigned year, List<Pair<String, List<Pair<unsigned, double>> >>& list)
{
	String lit_year;
	switch (year) {
	case 1:
		lit_year = String("1");
		break;
	case 2:
		lit_year = String("2");
		break;
	case 3:
		lit_year = String("3");
		break;
	case 4:
		lit_year = String("4");
		break;
	default:
		return;
	}

	for (int i = 0; i < list.size(); ++i) {
		String filename = String("protocols\\") + list[i].first() + String("_") + lit_year + String(".txt");
		std::fstream file(filename.ptr(), std::ios::in | std::ios::out | std::ios::trunc);

		list[i].second().bubbleSortAsc(); // sort the faculty numbers

		if (file.is_open()) {
			for (int j = 0; j < list[i].second().size(); ++j)
				file << "FN: " << list[i].second()[j].first() << ", Grade: " << list[i].second()[j].second() << std::endl;
		}
	}
}

Student ConsoleHandler::getStudent(unsigned fn)
{
	Student student(fn);

	std::ifstream ifile(student.getFilename().ptr(), std::ios::binary);

	if (ifile.is_open()) {
		unsigned fn, group, grade_count, year, majorlen, namelen;
		double avr;
		char status;
		char* major_name, * c_name;

		ifile.read((char*)&fn, sizeof(unsigned));
		ifile.read((char*)&group, sizeof(unsigned));
		ifile.read((char*)&year, sizeof(unsigned));
		ifile.read((char*)&avr, sizeof(double));
		ifile.read((char*)&grade_count, sizeof(unsigned));
		ifile.read((char*)&status, sizeof(char));

		ifile.read((char*)&namelen, sizeof(unsigned));
		c_name = new char[namelen + 1];
		ifile.read(c_name, namelen);
		c_name[namelen] = '\0';

		ifile.read((char*)&majorlen, sizeof(unsigned));
		major_name = new char[majorlen + 1];
		ifile.read(major_name, majorlen);
		major_name[majorlen] = '\0';

		student = Student(c_name, fn, major_name, group, year, status, avr);

		delete[] major_name;
		delete[] c_name;
	}
	else {
		throw std::exception("Could not find student with that faculty number :(");
	}

	return student;
}