#ifndef SERVICES_H
#define SERVICES_H

#include "LinkedList.h"
#include "Models.h"
#include <string>
using std::string;

double round2(double x);
double calculateTotal10(double process, double midterm, double finalScore);
double convertToScale4(double score10);
string classifyByAverage10(double avg10);

Course* findCourse(LinkedList<Course>& courses, const string& courseId);
Student* findStudent(LinkedList<Student>& students, const string& studentId);
ClassSection* findSection(LinkedList<ClassSection>& sections, const string& sectionId);
Grade* findGrade(LinkedList<Grade>& grades, const string& studentId, const string& courseId);

int countCreditsOfStudent(LinkedList<Grade>& grades, LinkedList<Course>& courses, const string& studentId);
double calculateAverage10(LinkedList<Grade>& grades, LinkedList<Course>& courses, const string& studentId);
double calculateGPA4(LinkedList<Grade>& grades, LinkedList<Course>& courses, const string& studentId);

void updateAllTotalScores(LinkedList<Grade>& grades);
void sortStudentsById(LinkedList<Student>& students);
void sortStudentsByName(LinkedList<Student>& students);
void sortGradesByTotalDesc(LinkedList<Grade>& grades);

string makeGradeId(const string& studentId, const string& courseId);
bool addOrUpdateGrade(LinkedList<Grade>& grades,
                      LinkedList<Student>& students,
                      LinkedList<Course>& courses,
                      LinkedList<ClassSection>& sections,
                      const string& studentId,
                      const string& courseId,
                      const string& sectionId,
                      double process,
                      double midterm,
                      double finalScore,
                      string& message);

bool validateDate(const string& date);
bool validateScore(double score);

#endif
