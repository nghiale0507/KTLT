#include "../include/Services.h"
#include <cmath>
#include <sstream>
using namespace std;

double round2(double x) {
    return floor(x * 100.0 + 0.5) / 100.0;
}

double calculateTotal10(double process, double midterm, double finalScore) {
    return round2(process * 0.2 + midterm * 0.3 + finalScore * 0.5);
}

double convertToScale4(double score10) {
    if (score10 >= 9.0) return 4.0;
    if (score10 >= 8.5) return 3.7;
    if (score10 >= 8.0) return 3.5;
    if (score10 >= 7.0) return 3.0;
    if (score10 >= 6.5) return 2.5;
    if (score10 >= 5.5) return 2.0;
    if (score10 >= 5.0) return 1.5;
    if (score10 >= 4.0) return 1.0;
    return 0.0;
}

string classifyByAverage10(double avg10) {
    if (avg10 >= 9.0) return "Xuat sac";
    if (avg10 >= 8.0) return "Gioi";
    if (avg10 >= 6.5) return "Kha";
    if (avg10 >= 5.0) return "Trung binh";
    if (avg10 >= 4.0) return "Yeu";
    return "Kem";
}

Course* findCourse(LinkedList<Course>& courses, const string& courseId) {
    return courses.findById(courseId);
}

Student* findStudent(LinkedList<Student>& students, const string& studentId) {
    return students.findById(studentId);
}

ClassSection* findSection(LinkedList<ClassSection>& sections, const string& sectionId) {
    return sections.findById(sectionId);
}

Grade* findGrade(LinkedList<Grade>& grades, const string& studentId, const string& courseId) {
    Node<Grade>* cur = grades.getHead();
    while (cur != 0) {
        if (cur->data.studentId == studentId && cur->data.courseId == courseId) return &(cur->data);
        cur = cur->next;
    }
    return 0;
}

int countCreditsOfStudent(LinkedList<Grade>& grades, LinkedList<Course>& courses, const string& studentId) {
    int total = 0;
    Node<Grade>* cur = grades.getHead();
    while (cur != 0) {
        if (cur->data.studentId == studentId) {
            Course* c = findCourse(courses, cur->data.courseId);
            if (c != 0) total += c->credits;
        }
        cur = cur->next;
    }
    return total;
}

double calculateAverage10(LinkedList<Grade>& grades, LinkedList<Course>& courses, const string& studentId) {
    double sum = 0;
    int credits = 0;
    Node<Grade>* cur = grades.getHead();
    while (cur != 0) {
        if (cur->data.studentId == studentId) {
            Course* c = findCourse(courses, cur->data.courseId);
            if (c != 0) {
                sum += cur->data.total10 * c->credits;
                credits += c->credits;
            }
        }
        cur = cur->next;
    }
    if (credits == 0) return 0;
    return round2(sum / credits);
}

double calculateGPA4(LinkedList<Grade>& grades, LinkedList<Course>& courses, const string& studentId) {
    double sum = 0;
    int credits = 0;
    Node<Grade>* cur = grades.getHead();
    while (cur != 0) {
        if (cur->data.studentId == studentId) {
            Course* c = findCourse(courses, cur->data.courseId);
            if (c != 0) {
                sum += convertToScale4(cur->data.total10) * c->credits;
                credits += c->credits;
            }
        }
        cur = cur->next;
    }
    if (credits == 0) return 0;
    return round2(sum / credits);
}

void updateAllTotalScores(LinkedList<Grade>& grades) {
    Node<Grade>* cur = grades.getHead();
    while (cur != 0) {
        cur->data.total10 = calculateTotal10(cur->data.processScore, cur->data.midtermScore, cur->data.finalScore);
        cur = cur->next;
    }
}

static int cmpStudentId(const Student& a, const Student& b) {
    if (a.id < b.id) return -1;
    if (a.id > b.id) return 1;
    return 0;
}

static int cmpStudentName(const Student& a, const Student& b) {
    if (a.fullName < b.fullName) return -1;
    if (a.fullName > b.fullName) return 1;
    return 0;
}

static int cmpGradeTotalDesc(const Grade& a, const Grade& b) {
    if (a.total10 > b.total10) return -1;
    if (a.total10 < b.total10) return 1;
    return 0;
}

void sortStudentsById(LinkedList<Student>& students) { students.bubbleSort(cmpStudentId); }
void sortStudentsByName(LinkedList<Student>& students) { students.bubbleSort(cmpStudentName); }
void sortGradesByTotalDesc(LinkedList<Grade>& grades) { grades.bubbleSort(cmpGradeTotalDesc); }

string makeGradeId(const string& studentId, const string& courseId) {
    return "G_" + studentId + "_" + courseId;
}

bool validateScore(double score) {
    return score >= 0.0 && score <= 10.0;
}

bool validateDate(const string& date) {
    if (date.length() != 10) return false;
    if (date[4] != '-' || date[7] != '-') return false;
    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) continue;
        if (date[i] < '0' || date[i] > '9') return false;
    }
    return true;
}

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
                      string& message) {
    if (findStudent(students, studentId) == 0) {
        message = "Khong ton tai ma sinh vien.";
        return false;
    }
    if (findCourse(courses, courseId) == 0) {
        message = "Khong ton tai ma mon hoc.";
        return false;
    }
    if (findSection(sections, sectionId) == 0) {
        message = "Khong ton tai lop hoc phan.";
        return false;
    }
    if (!validateScore(process) || !validateScore(midterm) || !validateScore(finalScore)) {
        message = "Diem phai nam trong khoang 0..10.";
        return false;
    }
    Grade* existed = findGrade(grades, studentId, courseId);
    if (existed != 0) {
        existed->sectionId = sectionId;
        existed->processScore = process;
        existed->midtermScore = midterm;
        existed->finalScore = finalScore;
        existed->total10 = calculateTotal10(process, midterm, finalScore);
        message = "Cap nhat diem thanh cong.";
        return true;
    }
    Grade g;
    g.id = makeGradeId(studentId, courseId);
    g.studentId = studentId;
    g.courseId = courseId;
    g.sectionId = sectionId;
    g.processScore = process;
    g.midtermScore = midterm;
    g.finalScore = finalScore;
    g.total10 = calculateTotal10(process, midterm, finalScore);
    grades.addBack(g);
    message = "Them diem thanh cong.";
    return true;
}
