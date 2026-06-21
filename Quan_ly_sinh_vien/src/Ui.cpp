#include "../include/Ui.h"
#include "../include/DataStore.h"
#include "../include/Services.h"
#include "../include/GuiGenerator.h"
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

static string cutStr(string s, int n) {
    if ((int)s.length() <= n) return s;
    if (n <= 3) return s.substr(0, n);
    return s.substr(0, n - 3) + "...";
}

static void line(char ch, int n) {
    for (int i = 0; i < n; i++) cout << ch;
    cout << "\n";
}

void printMainMenu() {
    cout << "\n";
    line('=', 78);
    cout << "          HE THONG QUAN LY SINH VIEN VA DIEM SO - MI3310 KTLT\n";
    line('=', 78);
    cout << "  1. Hien thi danh sach sinh vien\n";
    cout << "  2. Them sinh vien\n";
    cout << "  3. Cap nhat thong tin sinh vien\n";
    cout << "  4. Xoa sinh vien\n";
    cout << "  5. Hien thi danh sach mon hoc\n";
    cout << "  6. Nhap / cap nhat diem theo MSSV va ma mon\n";
    cout << "  7. Tim kiem va hien thi bang diem cua mot sinh vien\n";
    cout << "  8. In bang diem cua lop hoc phan\n";
    cout << "  9. Sap xep danh sach sinh vien theo MSSV / ten\n";
    cout << " 10. Bao cao in de nop va ky danh sach thi\n";
    cout << " 11. Xuat giao dien GUI Dashboard dang HTML\n";
    cout << "  0. Luu du lieu va ket thuc\n";
    line('-', 78);
    cout << "Nhap lua chon: ";
}

void printStudents(LinkedList<Student>& students, LinkedList<Grade>& grades, LinkedList<Course>& courses) {
    cout << "\nDANH SACH SINH VIEN\n";
    line('-', 118);
    cout << left << setw(13) << "MSSV" << setw(25) << "Ho ten" << setw(12) << "Lop"
         << setw(13) << "Ngay sinh" << setw(9) << "Gioi" << setw(11) << "TC"
         << setw(10) << "TB10" << setw(8) << "GPA4" << "Xep loai\n";
    line('-', 118);
    Node<Student>* cur = students.getHead();
    while (cur != 0) {
        Student s = cur->data;
        double avg10 = calculateAverage10(grades, courses, s.id);
        double gpa4 = calculateGPA4(grades, courses, s.id);
        int credits = countCreditsOfStudent(grades, courses, s.id);
        cout << left << setw(13) << s.id << setw(25) << cutStr(s.fullName, 24) << setw(12) << s.className
             << setw(13) << s.birthDate << setw(9) << s.gender << setw(11) << credits
             << setw(10) << fixed << setprecision(2) << avg10 << setw(8) << gpa4 << classifyByAverage10(avg10) << "\n";
        cur = cur->next;
    }
    line('-', 118);
}

void printCourses(LinkedList<Course>& courses) {
    cout << "\nDANH SACH MON HOC\n";
    line('-', 70);
    cout << left << setw(15) << "Ma mon" << setw(42) << "Ten mon hoc" << "So TC\n";
    line('-', 70);
    Node<Course>* cur = courses.getHead();
    while (cur != 0) {
        cout << left << setw(15) << cur->data.id << setw(42) << cutStr(cur->data.name, 41) << cur->data.credits << "\n";
        cur = cur->next;
    }
    line('-', 70);
}

void printTranscriptOfStudent(LinkedList<Student>& students, LinkedList<Course>& courses, LinkedList<Grade>& grades, const string& studentId) {
    Student* s = findStudent(students, studentId);
    if (s == 0) {
        cout << "Khong tim thay sinh vien co MSSV " << studentId << "\n";
        return;
    }
    cout << "\nBANG DIEM CA NHAN\n";
    line('=', 100);
    cout << "MSSV: " << s->id << " | Ho ten: " << s->fullName << " | Lop: " << s->className << "\n";
    line('-', 100);
    cout << left << setw(12) << "Ma mon" << setw(34) << "Ten mon" << setw(6) << "TC"
         << setw(9) << "QT" << setw(9) << "GK" << setw(9) << "CK" << setw(9) << "Tong" << "He4\n";
    line('-', 100);
    Node<Grade>* cur = grades.getHead();
    while (cur != 0) {
        if (cur->data.studentId == studentId) {
            Course* c = findCourse(courses, cur->data.courseId);
            string courseName = (c != 0) ? c->name : "Khong ro";
            int credits = (c != 0) ? c->credits : 0;
            cout << left << setw(12) << cur->data.courseId << setw(34) << cutStr(courseName, 33)
                 << setw(6) << credits << setw(9) << fixed << setprecision(1) << cur->data.processScore
                 << setw(9) << cur->data.midtermScore << setw(9) << cur->data.finalScore
                 << setw(9) << fixed << setprecision(2) << cur->data.total10 << convertToScale4(cur->data.total10) << "\n";
        }
        cur = cur->next;
    }
    line('-', 100);
    double avg10 = calculateAverage10(grades, courses, studentId);
    double gpa4 = calculateGPA4(grades, courses, studentId);
    cout << "Diem trung binh he 10: " << fixed << setprecision(2) << avg10
         << " | GPA he 4: " << gpa4 << " | Xep loai: " << classifyByAverage10(avg10) << "\n";
}

void printSectionScoreboard(LinkedList<Student>& students, LinkedList<Course>& courses, LinkedList<ClassSection>& sections, LinkedList<Grade>& grades, const string& sectionId) {
    ClassSection* sec = findSection(sections, sectionId);
    if (sec == 0) {
        cout << "Khong tim thay lop hoc phan " << sectionId << "\n";
        return;
    }
    Course* course = findCourse(courses, sec->courseId);
    cout << "\nBANG DIEM LOP HOC PHAN\n";
    line('=', 112);
    cout << "Lop HP: " << sec->id << " | Mon: " << sec->courseId << " - " << (course ? course->name : "Khong ro")
         << " | GV: " << sec->lecturer << "\n";
    line('-', 112);
    cout << left << setw(6) << "STT" << setw(13) << "MSSV" << setw(27) << "Ho ten" << setw(10) << "Lop"
         << setw(9) << "QT" << setw(9) << "GK" << setw(9) << "CK" << setw(9) << "Tong" << "Ket qua\n";
    line('-', 112);
    int stt = 1;
    Node<Grade>* cur = grades.getHead();
    while (cur != 0) {
        if (cur->data.sectionId == sectionId) {
            Student* s = findStudent(students, cur->data.studentId);
            cout << left << setw(6) << stt++ << setw(13) << cur->data.studentId
                 << setw(27) << (s ? cutStr(s->fullName, 26) : "Khong ro")
                 << setw(10) << (s ? s->className : "-")
                 << setw(9) << fixed << setprecision(1) << cur->data.processScore
                 << setw(9) << cur->data.midtermScore << setw(9) << cur->data.finalScore
                 << setw(9) << fixed << setprecision(2) << cur->data.total10
                 << (cur->data.total10 >= 4.0 ? "Dat" : "Khong dat") << "\n";
        }
        cur = cur->next;
    }
    line('-', 112);
}

void printExamSignatureReport(LinkedList<Student>& students, LinkedList<Course>& courses, LinkedList<ClassSection>& sections, LinkedList<Grade>& grades, const string& sectionId) {
    ClassSection* sec = findSection(sections, sectionId);
    if (sec == 0) {
        cout << "Khong tim thay lop hoc phan " << sectionId << "\n";
        return;
    }
    Course* c = findCourse(courses, sec->courseId);
    cout << "\nTRUONG DAI HOC BACH KHOA HA NOI\n";
    cout << "KHOA TOAN - TIN\n";
    line('=', 118);
    cout << "DANH SACH SINH VIEN DU DIEU KIEN DU THI / KY TEN\n";
    cout << "Mon hoc: " << sec->courseId << " - " << (c ? c->name : "Khong ro") << " | Lop HP: " << sec->id << "\n";
    cout << "Hoc ky: " << sec->semester << " | Nam hoc: " << sec->academicYear << " | Giang vien: " << sec->lecturer << "\n";
    line('-', 118);
    cout << left << setw(6) << "STT" << setw(13) << "MSSV" << setw(28) << "Ho ten" << setw(12) << "Lop"
         << setw(12) << "Diem QT" << setw(12) << "Diem tong" << setw(18) << "Trang thai" << "Ky ten\n";
    line('-', 118);
    int stt = 1;
    Node<Grade>* cur = grades.getHead();
    while (cur != 0) {
        if (cur->data.sectionId == sectionId) {
            Student* s = findStudent(students, cur->data.studentId);
            cout << left << setw(6) << stt++ << setw(13) << cur->data.studentId
                 << setw(28) << (s ? cutStr(s->fullName, 27) : "Khong ro")
                 << setw(12) << (s ? s->className : "-")
                 << setw(12) << fixed << setprecision(1) << cur->data.processScore
                 << setw(12) << fixed << setprecision(2) << cur->data.total10
                 << setw(18) << (cur->data.processScore >= 4.0 ? "Du dieu kien" : "Can xem xet")
                 << "................\n";
        }
        cur = cur->next;
    }
    line('-', 118);
    cout << "Ha Noi, ngay .... thang .... nam 2026\n";
    cout << right << setw(40) << "Can bo coi thi" << setw(40) << "Giang vien phu trach" << setw(35) << "Dai dien lop\n";
}

static void addStudentUI(LinkedList<Student>& students) {
    Student s;
    cout << "Nhap MSSV: "; cin >> s.id; cin.ignore();
    if (students.findById(s.id) != 0) { cout << "MSSV da ton tai.\n"; return; }
    cout << "Nhap ho ten: "; getline(cin, s.fullName);
    cout << "Nhap lop: "; getline(cin, s.className);
    cout << "Nhap ngay sinh (YYYY-MM-DD): "; getline(cin, s.birthDate);
    if (!validateDate(s.birthDate)) { cout << "Ngay sinh khong dung dinh dang.\n"; return; }
    cout << "Nhap gioi tinh: "; getline(cin, s.gender);
    cout << "Nhap so dien thoai: "; getline(cin, s.phone);
    cout << "Nhap email: "; getline(cin, s.email);
    students.addBack(s);
    cout << "Them sinh vien thanh cong.\n";
}

static void updateStudentUI(LinkedList<Student>& students) {
    string id; cout << "Nhap MSSV can cap nhat: "; cin >> id; cin.ignore();
    Student* s = students.findById(id);
    if (s == 0) { cout << "Khong tim thay sinh vien.\n"; return; }
    string tmp;
    cout << "Ho ten moi (bo trong de giu nguyen): "; getline(cin, tmp); if (tmp != "") s->fullName = tmp;
    cout << "Lop moi (bo trong de giu nguyen): "; getline(cin, tmp); if (tmp != "") s->className = tmp;
    cout << "Ngay sinh moi (bo trong de giu nguyen): "; getline(cin, tmp); if (tmp != "") s->birthDate = tmp;
    cout << "Cap nhat thanh cong.\n";
}

static void inputGradeUI(LinkedList<Grade>& grades, LinkedList<Student>& students, LinkedList<Course>& courses, LinkedList<ClassSection>& sections) {
    string sid, cid, secid;
    double p, m, f;
    cout << "Nhap MSSV: "; cin >> sid;
    cout << "Nhap ma mon: "; cin >> cid;
    cout << "Nhap ma lop hoc phan: "; cin >> secid;
    cout << "Nhap diem qua trinh, giua ky, cuoi ky: "; cin >> p >> m >> f;
    string msg;
    bool ok = addOrUpdateGrade(grades, students, courses, sections, sid, cid, secid, p, m, f, msg);
    cout << msg << "\n";
    if (ok) cout << "Diem tong he 10 = " << calculateTotal10(p, m, f) << "\n";
}

void runProgram() {
    LinkedList<Student> students;
    LinkedList<Course> courses;
    LinkedList<ClassSection> sections;
    LinkedList<Grade> grades;
    loadAll(students, courses, sections, grades);
    updateAllTotalScores(grades);

    int choice;
    do {
        printMainMenu();
        cin >> choice;
        if (!cin) { cin.clear(); cin.ignore(10000, '\n'); continue; }
        if (choice == 1) printStudents(students, grades, courses);
        else if (choice == 2) addStudentUI(students);
        else if (choice == 3) updateStudentUI(students);
        else if (choice == 4) {
            string id; cout << "Nhap MSSV can xoa: "; cin >> id;
            cout << (students.removeById(id) ? "Xoa thanh cong.\n" : "Khong tim thay sinh vien.\n");
        }
        else if (choice == 5) printCourses(courses);
        else if (choice == 6) inputGradeUI(grades, students, courses, sections);
        else if (choice == 7) { string id; cout << "Nhap MSSV: "; cin >> id; printTranscriptOfStudent(students, courses, grades, id); }
        else if (choice == 8) { string id; cout << "Nhap ma lop hoc phan: "; cin >> id; printSectionScoreboard(students, courses, sections, grades, id); }
        else if (choice == 9) {
            int k; cout << "1. Sap xep theo MSSV | 2. Sap xep theo ten: "; cin >> k;
            if (k == 1) sortStudentsById(students); else sortStudentsByName(students);
            printStudents(students, grades, courses);
        }
        else if (choice == 10) { string id; cout << "Nhap ma lop hoc phan: "; cin >> id; printExamSignatureReport(students, courses, sections, grades, id); }
        else if (choice == 11) {
            bool ok = exportGuiDashboard(students, courses, sections, grades, "gui/dashboard.html");
            cout << (ok ? "Da xuat GUI Dashboard: gui/dashboard.html\n" : "Khong the xuat GUI Dashboard.\n");
        }
        else if (choice == 0) {
            bool ok = saveAll(students, courses, sections, grades);
            cout << (ok ? "Da luu du lieu. Ket thuc chuong trinh.\n" : "Loi khi luu du lieu.\n");
        }
        else cout << "Lua chon khong hop le.\n";
    } while (choice != 0);
}
