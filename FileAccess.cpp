// Implementation of file access class.

#include "stdafx.h"
#include "FileAccess.h"

FileAccess::FileAccess(int argc, char *argv[])
{
	// Check that there is exactly one run time parameter.
	if (argc != 2) {
		cerr << "Usage: Assem <FileName>" << endl;
		exit(1);
	}
	// Open the file.  One might question if this is the best place to open the file.
	// One might also question whether we need a file access class.
	m_sfile.open(argv[1], ios::in);
	cout << argv[1];
	// If the open failed, report the error and terminate.
	if (!m_sfile) {
		cerr << "Source file could not be opened, assembler terminated."
			<< endl;
		exit(1);
	}
}

FileAccess::~FileAccess()
{
	m_sfile.close();
}


bool FileAccess::GetNextLine(string &a_buff)
{
	if (m_sfile.eof()) return false;

	getline(m_sfile, a_buff);

	// Return indicating success.
	return true;
}


void FileAccess::rewind()
{
	// Clean the end of file flag and go back to the beginning of the file.
	m_sfile.clear();
	m_sfile.seekg(0, ios::beg);
}
