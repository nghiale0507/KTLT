#ifndef MODELS_H
#define MODELS_H

#include <string>
using std::string;

struct Student {
    string id;
    string fullName;
    string className;
    string birthDate;
    string gender;
    string phone;
    string email;
};

struct Course {
    string id;
    string name;
    int credits;
};

struct ClassSection {
    string id;
    string courseId;
    string semester;
    string academicYear;
    string lecturer;
};

struct Grade {
    string id;
    string studentId;
    string courseId;
    string sectionId;
    double processScore;
    double midtermScore;
    double finalScore;
    double total10;
};

#endif
