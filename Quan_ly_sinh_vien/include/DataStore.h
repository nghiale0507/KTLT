#ifndef DATA_STORE_H
#define DATA_STORE_H

#include "LinkedList.h"
#include "Models.h"
#include <string>
using std::string;

int splitPipe(const string& line, string parts[], int maxParts);
string safeField(string s);

void loadStudents(LinkedList<Student>& students, const string& filePath);
void loadCourses(LinkedList<Course>& courses, const string& filePath);
void loadSections(LinkedList<ClassSection>& sections, const string& filePath);
void loadGrades(LinkedList<Grade>& grades, const string& filePath);

bool saveStudents(const LinkedList<Student>& students, const string& filePath);
bool saveCourses(const LinkedList<Course>& courses, const string& filePath);
bool saveSections(const LinkedList<ClassSection>& sections, const string& filePath);
bool saveGrades(const LinkedList<Grade>& grades, const string& filePath);

void loadAll(LinkedList<Student>& students,
             LinkedList<Course>& courses,
             LinkedList<ClassSection>& sections,
             LinkedList<Grade>& grades);

bool saveAll(const LinkedList<Student>& students,
             const LinkedList<Course>& courses,
             const LinkedList<ClassSection>& sections,
             const LinkedList<Grade>& grades);

#endif
