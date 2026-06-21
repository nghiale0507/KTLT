#include "../include/LinkedList.h"
#include "../include/Models.h"
#include "../include/Services.h"
#include <iostream>
#include <cmath>
using namespace std;

static bool almost(double a, double b) { return fabs(a - b) < 0.001; }

int main() {
    int passed = 0, failed = 0;
    auto check = [&](bool cond, const char* name) {
        if (cond) { cout << "[PASS] " << name << "\n"; passed++; }
        else { cout << "[FAIL] " << name << "\n"; failed++; }
    };

    check(almost(calculateTotal10(8.0, 7.0, 9.0), 8.2), "Tinh diem tong = 20% QT + 30% GK + 50% CK");
    check(almost(convertToScale4(8.7), 3.7), "Quy doi diem he 10 sang he 4");
    check(classifyByAverage10(8.1) == "Gioi", "Xep loai hoc luc gioi");
    check(validateDate("2006-05-19") == true && validateDate("19-05-2006") == false, "Kiem tra dinh dang ngay sinh");

    LinkedList<Student> students;
    Student a; a.id = "202400001"; a.fullName = "B Student"; a.className = "IT1"; a.birthDate = "2006-01-01"; a.gender = "Nam";
    Student b; b.id = "202400002"; b.fullName = "A Student"; b.className = "IT1"; b.birthDate = "2006-01-02"; b.gender = "Nu";
    students.addBack(a); students.addBack(b);
    check(students.count() == 2, "Danh sach lien ket addBack va count");
    check(students.findById("202400002") != 0, "Tim kiem tuyen tinh theo MSSV");
    sortStudentsByName(students);
    check(students.getHead()->data.fullName == "A Student", "Sap xep noi bot theo ten sinh vien");
    check(students.removeById("202400001") == true && students.count() == 1, "Xoa node trong danh sach lien ket");

    LinkedList<Course> courses;
    Course c1; c1.id = "MI1010"; c1.name = "Giai tich 1"; c1.credits = 3;
    Course c2; c2.id = "IT1110"; c2.name = "Tin hoc dai cuong"; c2.credits = 2;
    courses.addBack(c1); courses.addBack(c2);

    LinkedList<ClassSection> sections;
    ClassSection sec1; sec1.id = "MI1010-01"; sec1.courseId = "MI1010"; sec1.semester = "2025.1"; sec1.academicYear = "2025-2026"; sec1.lecturer = "TS. A";
    ClassSection sec2; sec2.id = "IT1110-01"; sec2.courseId = "IT1110"; sec2.semester = "2025.1"; sec2.academicYear = "2025-2026"; sec2.lecturer = "TS. B";
    sections.addBack(sec1); sections.addBack(sec2);

    LinkedList<Grade> grades;
    string msg;
    check(addOrUpdateGrade(grades, students, courses, sections, "202400002", "MI1010", "MI1010-01", 8, 8, 8, msg), "Them diem hop le");
    check(addOrUpdateGrade(grades, students, courses, sections, "202400002", "IT1110", "IT1110-01", 9, 9, 9, msg), "Them diem mon thu hai");
    check(almost(calculateAverage10(grades, courses, "202400002"), 8.4), "Tinh diem trung binh co trong so tin chi");
    check(!addOrUpdateGrade(grades, students, courses, sections, "202400002", "MI1010", "MI1010-01", 11, 8, 8, msg), "Chan diem ngoai khoang 0..10");

    cout << "\nTong ket test: " << passed << " PASS, " << failed << " FAIL\n";
    return failed == 0 ? 0 : 1;
}
