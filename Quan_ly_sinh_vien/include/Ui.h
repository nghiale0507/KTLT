#ifndef UI_H
#define UI_H

#include "LinkedList.h"
#include "Models.h"

void runProgram();
void printMainMenu();
void printStudents(LinkedList<Student>& students, LinkedList<Grade>& grades, LinkedList<Course>& courses);
void printCourses(LinkedList<Course>& courses);
void printTranscriptOfStudent(LinkedList<Student>& students, LinkedList<Course>& courses, LinkedList<Grade>& grades, const std::string& studentId);
void printSectionScoreboard(LinkedList<Student>& students, LinkedList<Course>& courses, LinkedList<ClassSection>& sections, LinkedList<Grade>& grades, const std::string& sectionId);
void printExamSignatureReport(LinkedList<Student>& students, LinkedList<Course>& courses, LinkedList<ClassSection>& sections, LinkedList<Grade>& grades, const std::string& sectionId);

#endif
