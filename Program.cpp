#include "Program.hpp"

const char* Program::PROGRAMS_FOLDER = "programs\\";


Program::Program(const String& name)
	:programs_name(name) {}

String Program::getFilename() const
{
	return (String(PROGRAMS_FOLDER) + programs_name + String(".bin"));
}

String Program::getFilename(const String& program) const
{
	return (String(PROGRAMS_FOLDER) + program + String(".bin"));
}

String Program::getName() const
{
	return programs_name;
}

void Program::createEntry(unsigned fn)
{
	std::fstream file(getFilename().ptr(), std::ios::binary | std::ios::in | std::ios::out | std::ios::app);
	if (file.is_open()) {
		if (file.good()) {
			unsigned year = 1; // the student is first year

			file.write((const char*)&fn, sizeof(unsigned));
			file.write((const char*)&year, sizeof(unsigned));

			file.close();
		}
	}
	else {
		std::ofstream ofile(getFilename().ptr(), std::ios::binary);
		if (ofile.is_open()) {
			unsigned year = 1; // the student is first year

			ofile.write((const char*)&fn, sizeof(unsigned));
			ofile.write((const char*)&year, sizeof(unsigned));

			ofile.close();
		}
		else {
			String error_message = String("Could not enter student with faculty number ") + String(fn) + String(" in the programs file :(");
			throw std::exception(error_message.ptr());
		}
	}
}

void Program::advanceEntry(unsigned fn)
{
	std::fstream stream(getFilename().ptr(), std::ios::binary | std::ios::in | std::ios::out);

	if (stream.is_open()) {
		unsigned cur_fn, cur_year;
		do {
			if (!stream.good()) {
				std::cout << "Could not find student with that faculty number :(" << std::endl;
				return;
			}
			stream.read((char*)&cur_fn, sizeof(unsigned));
			stream.read((char*)&cur_year, sizeof(unsigned));
		} while (cur_fn != fn);

		cur_year++;
		int jump = sizeof(unsigned);
		stream.seekp(-jump, std::ios::cur);
		stream.write((const char*)&cur_year, sizeof(unsigned));

		std::cout << "Student with faculty number:" << fn << " advanced to next year!" << std::endl;
		stream.close();
	}
	else throw std::exception("Could not find that program :(");
}

void Program::moveEntry(unsigned fn, Program& new_program)
{
	List<unsigned> fns; //list of all the file's content before the last record
	List<unsigned> years; //list of all the file's content before the last record
	unsigned year_to_move; //the year in which the student with the given fn is

	std::fstream stream(getFilename().ptr(), std::ios::binary | std::ios::in | std::ios::out); //delete old record in the file

	if (stream.is_open()) {
		while (!stream.eof()) {
			if (stream.good()) {
				unsigned cur_fn, cur_year;
				stream.read((char*)&cur_fn, sizeof(unsigned));
				if (cur_fn == fn) {
					stream.read((char*)&year_to_move, sizeof(unsigned));
					continue;
				}

				fns.add(cur_fn);
				stream.read((char*)&cur_year, sizeof(unsigned));
				years.add(cur_year);
			}
		}
		stream.close();

		stream.open(getFilename().ptr(), std::ios::binary | std::ios::in | std::ios::out | std::ios::trunc); //enter all other records
		if (stream.is_open()) {
			for (int i = 0; i < fns.size(); ++i) {
				unsigned extracted_fn = fns[i];
				unsigned extracted_year = years[i];
				stream.write((const char*)&extracted_fn, sizeof(unsigned));
				stream.write((const char*)&extracted_year, sizeof(unsigned));
			}

			stream.close();
		}

		stream.open(getFilename(new_program.programs_name).ptr(), std::ios::binary | std::ios::in | std::ios::out); //create new record in another file
		if (stream.is_open()) {
			if (stream.good()) {
				stream.write((const char*)&fn, sizeof(unsigned));
				stream.write((const char*)&year_to_move, sizeof(unsigned));
				
				stream.close();
			}
		}
		else {
			std::ofstream stream(getFilename(new_program.programs_name).ptr(), std::ios::binary);

			if (stream.is_open()) {
				stream.write((const char*)&fn, sizeof(unsigned));
				stream.write((const char*)&year_to_move, sizeof(unsigned));

				stream.close();
			}
		}
	}
	else throw std::exception("Could not find that program :(");
}
void Program::changeYearInProgramsFile(unsigned fn, unsigned year)
{
	std::fstream stream(getFilename().ptr(), std::ios::binary | std::ios::in | std::ios::out);

	unsigned dummy, pos = 0;
	if (stream.is_open()) {
		unsigned cur_fn;
		while (!stream.eof()) {
			if (stream.good()) {
				stream.read((char*)&cur_fn, sizeof(unsigned));
				if (cur_fn == fn) {
					pos = stream.tellg();
					stream.close();
					break;
				}
				else stream.read((char*)&dummy, sizeof(unsigned));
			}
		}

		if (pos == 0) throw std::exception("no fn");

		std::fstream stream(getFilename().ptr(), std::ios::binary | std::ios::in | std::ios::out);
		stream.seekp(pos, std::ios::beg);
		stream.write((const char*)&year, sizeof(unsigned));
		stream.close();
	}
	else throw std::exception("Could not find that program :(");
}

List<unsigned> Program::getOneYear(unsigned year)
{
	List<unsigned> fns;

	std::fstream stream(getFilename().ptr(), std::ios::binary | std::ios::in | std::ios::out);
	if (stream.is_open()) {
		unsigned cur_fn, cur_year;
		while (!stream.eof()) {
			stream.read((char*)&cur_fn, sizeof(unsigned));
			stream.read((char*)&cur_year, sizeof(unsigned));
			if (!stream.good()) break;

			if (year == cur_year) fns.add(cur_fn);
		}

		stream.close();
	}
	else throw std::exception("Could not find that program :(");
	
	return fns;
}
