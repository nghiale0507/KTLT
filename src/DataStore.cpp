#include "../include/DataStore.h"
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <sys/stat.h>
using namespace std;

static void ensureDataDir() {
#ifdef _WIN32
    system("if not exist data mkdir data");
#else
    mkdir("data", 0777);
#endif
}

int splitPipe(const string& line, string parts[], int maxParts) {
    int count = 0;
    string current = "";
    for (int i = 0; i < (int)line.length(); i++) {
        if (line[i] == '|') {
            if (count < maxParts) parts[count++] = current;
            current = "";
        } else {
            current += line[i];
        }
    }
    if (count < maxParts) parts[count++] = current;
    return count;
}

string safeField(string s) {
    for (int i = 0; i < (int)s.length(); i++) {
        if (s[i] == '|') s[i] = '/';
        if (s[i] == '\n' || s[i] == '\r') s[i] = ' ';
    }
    return s;
}

void loadStudents(LinkedList<Student>& students, const string& filePath) {
    ifstream fin(filePath.c_str());
    if (!fin.is_open()) return;
    string line;
    while (getline(fin, line)) {
        if (line.length() == 0) continue;
        string p[10];
        if (splitPipe(line, p, 10) >= 7) {
            Student s;
            s.id = p[0]; s.fullName = p[1]; s.className = p[2];
            s.birthDate = p[3]; s.gender = p[4]; s.phone = p[5]; s.email = p[6];
            students.addBack(s);
        }
    }
    fin.close();
}

void loadCourses(LinkedList<Course>& courses, const string& filePath) {
    ifstream fin(filePath.c_str());
    if (!fin.is_open()) return;
    string line;
    while (getline(fin, line)) {
        if (line.length() == 0) continue;
        string p[6];
        if (splitPipe(line, p, 6) >= 3) {
            Course c;
            c.id = p[0]; c.name = p[1]; c.credits = atoi(p[2].c_str());
            courses.addBack(c);
        }
    }
    fin.close();
}

void loadSections(LinkedList<ClassSection>& sections, const string& filePath) {
    ifstream fin(filePath.c_str());
    if (!fin.is_open()) return;
    string line;
    while (getline(fin, line)) {
        if (line.length() == 0) continue;
        string p[8];
        if (splitPipe(line, p, 8) >= 5) {
            ClassSection sec;
            sec.id = p[0]; sec.courseId = p[1]; sec.semester = p[2];
            sec.academicYear = p[3]; sec.lecturer = p[4];
            sections.addBack(sec);
        }
    }
    fin.close();
}

void loadGrades(LinkedList<Grade>& grades, const string& filePath) {
    ifstream fin(filePath.c_str());
    if (!fin.is_open()) return;
    string line;
    while (getline(fin, line)) {
        if (line.length() == 0) continue;
        string p[12];
        if (splitPipe(line, p, 12) >= 8) {
            Grade g;
            g.id = p[0]; g.studentId = p[1]; g.courseId = p[2]; g.sectionId = p[3];
            g.processScore = atof(p[4].c_str());
            g.midtermScore = atof(p[5].c_str());
            g.finalScore = atof(p[6].c_str());
            g.total10 = atof(p[7].c_str());
            grades.addBack(g);
        }
    }
    fin.close();
}

bool saveStudents(const LinkedList<Student>& students, const string& filePath) {
    ensureDataDir();
    ofstream fout(filePath.c_str());
    if (!fout.is_open()) return false;
    Node<Student>* cur = students.getHead();
    while (cur != 0) {
        Student s = cur->data;
        fout << safeField(s.id) << "|" << safeField(s.fullName) << "|" << safeField(s.className) << "|"
             << safeField(s.birthDate) << "|" << safeField(s.gender) << "|" << safeField(s.phone) << "|"
             << safeField(s.email) << "\n";
        cur = cur->next;
    }
    fout.close();
    return true;
}

bool saveCourses(const LinkedList<Course>& courses, const string& filePath) {
    ensureDataDir();
    ofstream fout(filePath.c_str());
    if (!fout.is_open()) return false;
    Node<Course>* cur = courses.getHead();
    while (cur != 0) {
        Course c = cur->data;
        fout << safeField(c.id) << "|" << safeField(c.name) << "|" << c.credits << "\n";
        cur = cur->next;
    }
    fout.close();
    return true;
}

bool saveSections(const LinkedList<ClassSection>& sections, const string& filePath) {
    ensureDataDir();
    ofstream fout(filePath.c_str());
    if (!fout.is_open()) return false;
    Node<ClassSection>* cur = sections.getHead();
    while (cur != 0) {
        ClassSection s = cur->data;
        fout << safeField(s.id) << "|" << safeField(s.courseId) << "|" << safeField(s.semester) << "|"
             << safeField(s.academicYear) << "|" << safeField(s.lecturer) << "\n";
        cur = cur->next;
    }
    fout.close();
    return true;
}

bool saveGrades(const LinkedList<Grade>& grades, const string& filePath) {
    ensureDataDir();
    ofstream fout(filePath.c_str());
    if (!fout.is_open()) return false;
    Node<Grade>* cur = grades.getHead();
    while (cur != 0) {
        Grade g = cur->data;
        fout << safeField(g.id) << "|" << safeField(g.studentId) << "|" << safeField(g.courseId) << "|"
             << safeField(g.sectionId) << "|" << g.processScore << "|" << g.midtermScore << "|"
             << g.finalScore << "|" << g.total10 << "\n";
        cur = cur->next;
    }
    fout.close();
    return true;
}

void loadAll(LinkedList<Student>& students,
             LinkedList<Course>& courses,
             LinkedList<ClassSection>& sections,
             LinkedList<Grade>& grades) {
    students.clear(); courses.clear(); sections.clear(); grades.clear();
    loadStudents(students, "data/students.txt");
    loadCourses(courses, "data/courses.txt");
    loadSections(sections, "data/sections.txt");
    loadGrades(grades, "data/grades.txt");
}

bool saveAll(const LinkedList<Student>& students,
             const LinkedList<Course>& courses,
             const LinkedList<ClassSection>& sections,
             const LinkedList<Grade>& grades) {
    bool ok1 = saveStudents(students, "data/students.txt");
    bool ok2 = saveCourses(courses, "data/courses.txt");
    bool ok3 = saveSections(sections, "data/sections.txt");
    bool ok4 = saveGrades(grades, "data/grades.txt");
    return ok1 && ok2 && ok3 && ok4;
}
