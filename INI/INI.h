#pragma once

//----------------------

namespace txu
{

//----------------------

namespace ini
{

//----------------------

class INI
{
public:
	INI ();
	INI (const INI&  copy);
	INI (      INI&& move);
	INI (const char* filename);

	bool loadFromFile (const char* filename);
	bool saveToFile   (const char* filename);

	bool exists (const char* name);

	Section& addSection    (const char* name);
	Section& addSection    (const Section& section);
	Section& getSection    (const char* name);
	void     removeSection (const char* name);
	void     clear         ();

	Section& operator [] (const char* name);
	Section& operator [] (size_t index);

	size_t         getSectionsCount () const;
	const Section* getData          () const;
	      Section* getData          ();

	INI& operator += (const Section& section);
	INI& operator -= (const char* name);

private:
	std::vector <Section> m_sections;

};

//----------------------

INI::INI ():
	m_sections ()
{}

INI::INI (const INI& copy):
	INI ()
{
	m_sections = copy.m_sections;
}

INI::INI (INI&& move):
	INI ()
{
	std::swap (m_sections, move.m_sections);
}

INI::INI (const char* filename):
	INI ()
{
	loadFromFile (filename);
}

//----------------------

bool INI::loadFromFile (const char* filename)
{
	static char path[_TX_BUFSIZE] = "";
	GetFullPathNameA (filename, _TX_BUFSIZE, path, nullptr);

	// GetPrivateProfileSectionNamesA - �������� ����������� �������. 
	// Ÿ ��������� - double-null-terminated ������, ��������� �� ��� ������ INI �����, ���������� �������� ���������,
	// ������� �������� ����� ������ ���� �� ����� ��� ������ strtok �� ��������� - ��� ��������� ������� ������� ��� ����� ������.
	// ������ ��� �������� ����������� ������.
	
	// ������� ���� ������� ������ ��� ������ �� INI-�����.
	// �������� ������ ��� �������, � �������� GetPrivateProfileSectionNamesA �� ��� ���,
	// ���� � ������������ �������� �� ����� ���������� �� buffsize - 2.
	// �������� ������������, ����� ��������� �������� ����������� ����� �������.

	size_t buffsize = _TX_BUFSIZE;
	char* buffer = new char[buffsize];

	while (GetPrivateProfileSectionNamesA (buffer, buffsize, path) == buffsize-2) 
	{
		// ������ ������� ������� ���. ������ �������� ������, � ��� ���� ������ ����������.

		delete[] (buffer);

		buffsize *= 2; 
		buffer = new char[buffsize];
	}

	// ���������� ������ ����� ����� ��������� ���: section\0section\0...\0\0

	// ������, ����� ��������� ����� �������.
	// ������ ���������� name, ������� ����� ���������� �� ��� ���, ���� �� ���������� ������� ������.

	m_sections.clear ();
	for (const char* name = buffer; *name; name++)
	{
		Section section;
		if (!section.loadFromFile (path, name))
		{
			delete[] (buffer);
			return false;
		}

		m_sections.push_back (section);

		// �������� name �� ��� ���, ���� �� ���������� ������� ������.
		for (name; *name; name++);
	}

	delete[] (buffer);
	return true;
}

bool INI::saveToFile (const char* filename)
{
	static char path[_TX_BUFSIZE] = "";
	GetFullPathNameA (filename, _TX_BUFSIZE, path, nullptr);

	// ������� WritePrivateProfileSectionA �� �������������� INI ����, � ��������� � ���� ����������,
	// �� ����� ����� ������� ����� ������� ����, ���� �� ��� ����������.

	FILE* file = nullptr;
	errno_t err = fopen_s (&file, path, "rb");
	if (file && !err)
	{
		fclose (file);
		remove (path);
	}

	for (size_t i = 0, count = getSectionsCount (); i < count; i++)
		if (!m_sections[i].saveToFile (path)) return false;

	return true;
}

//----------------------

bool INI::exists (const char* name)
{
	size_t count = m_sections.size ();
	for (size_t i = 0; i < count; i++)
		if (!strcmp (m_sections[i].getName (), name)) return true;

	return false;
}

//----------------------

Section& INI::addSection (const char* name)
{
	if (exists (name))
		return  getSection (name) = Section (name);

	m_sections.push_back (Section (name));
	return m_sections.back ();
}

Section& INI::addSection (const Section& section)
{
	if (exists (section.getName ()))
		return getSection (section.getName ()) = section;

	m_sections.push_back (section);
	return m_sections.back ();
}

Section& INI::getSection (const char* name)
{
	size_t count = m_sections.size ();
	for (size_t i = 0; i < count; i++)
	{
		Section& section = m_sections[i];
		if (!strcmp (section.getName (), name)) return section;
	}

	return addSection (name);
}

void INI::removeSection (const char* name)
{
	size_t count = m_sections.size ();
	for (size_t i = 0; i < count; i++)
	{
		if (!strcmp (m_sections[i].getName (), name)) 
		{
			m_sections.erase (m_sections.begin () + i);
			return;
		}
	}
}

void INI::clear ()
{
	m_sections.clear ();
}

//----------------------

Section& INI::operator [] (const char* name)
{
	return getSection (name);
}

Section& INI::operator [] (size_t index)
{
	return m_sections[index];
}

//----------------------

size_t INI::getSectionsCount () const
{
	return m_sections.size ();
}

//----------------------

const Section* INI::getData () const
{
	return m_sections.data ();
}

Section* INI::getData ()
{
	return m_sections.data ();
}

//----------------------

INI& INI::operator += (const Section& section)
{
	addSection (section);
	return *this;
}

INI& INI::operator -= (const char* name)
{
	removeSection (name);
	return *this;
}

//----------------------

} // namespace ini

//----------------------

} // namespace txu

//----------------------