#include "../include/GuiGenerator.h"
#include "../include/Services.h"
#include <fstream>
#include <iomanip>
#include <sstream>
#include <cstdlib>
#include <sys/stat.h>
using namespace std;

static void ensureGuiDir() {
#ifdef _WIN32
    system("if not exist gui mkdir gui");
#else
    mkdir("gui", 0777);
#endif
}

static string htmlEscape(string s) {
    string out = "";
    for (int i = 0; i < (int)s.length(); i++) {
        char c = s[i];
        if (c == '&') out += "&amp;";
        else if (c == '<') out += "&lt;";
        else if (c == '>') out += "&gt;";
        else if (c == '"') out += "&quot;";
        else out += c;
    }
    return out;
}

static string fmt2(double x) {
    ostringstream oss;
    oss << fixed << setprecision(2) << x;
    return oss.str();
}

bool exportGuiDashboard(LinkedList<Student>& students,
                        LinkedList<Course>& courses,
                        LinkedList<ClassSection>& sections,
                        LinkedList<Grade>& grades,
                        const string& outputPath) {
    ensureGuiDir();
    ofstream f(outputPath.c_str());
    if (!f.is_open()) return false;

    int studentCount = students.count();
    int courseCount = courses.count();
    int sectionCount = sections.count();
    int gradeCount = grades.count();
    double sumAvg = 0.0;
    int avgN = 0;
    Node<Student>* sCur = students.getHead();
    while (sCur != 0) {
        double avg = calculateAverage10(grades, courses, sCur->data.id);
        if (avg > 0) { sumAvg += avg; avgN++; }
        sCur = sCur->next;
    }
    double avgSystem = avgN == 0 ? 0.0 : round2(sumAvg / avgN);

    f << "<!DOCTYPE html><html lang='vi'><head><meta charset='UTF-8'>";
    f << "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
    f << "<title>Quan ly sinh vien va diem so</title>";
    f << "<style>";
    f << "*{box-sizing:border-box}body{margin:0;font-family:'Segoe UI',Arial,sans-serif;background:#eef3f8;color:#17202a;}";
    f << ".layout{display:flex;min-height:100vh}.sidebar{width:285px;background:#1f2d3d;color:white;padding:28px 22px;}";
    f << ".brand{font-size:22px;font-weight:700;margin-bottom:8px}.sub{font-size:13px;color:#b8c7d9;margin-bottom:30px}.nav div{padding:13px 15px;border-radius:12px;margin:8px 0;background:#30455f}.nav div.active{background:#2f80ed;}";
    f << ".main{flex:1;padding:30px 34px}.top{display:flex;justify-content:space-between;align-items:center;margin-bottom:22px}.top h1{margin:0;font-size:30px}.badge{background:#eaf3ff;color:#0b4a8b;padding:10px 16px;border-radius:999px;font-weight:600}.cards{display:grid;grid-template-columns:repeat(5,1fr);gap:18px;margin-bottom:22px}.card{background:white;border-radius:18px;padding:22px;box-shadow:0 10px 25px rgba(31,45,61,.08)}.num{font-size:34px;font-weight:800;color:#1f2d3d}.lab{color:#607089;margin-top:5px}.panel{background:white;border-radius:20px;padding:22px;margin-bottom:22px;box-shadow:0 10px 25px rgba(31,45,61,.08)}";
    f << "table{width:100%;border-collapse:collapse;font-size:14px}th{background:#f3f7fb;text-align:left;color:#34495e}th,td{padding:11px 10px;border-bottom:1px solid #e6ebf2}.pill{padding:5px 10px;border-radius:999px;font-weight:600;font-size:12px}.good{background:#eaf8ed;color:#126b34}.warn{background:#fff7e6;color:#8a5a00}.bad{background:#fff0f0;color:#a42222}.grid2{display:grid;grid-template-columns:1fr 1fr;gap:18px}.footer{font-size:13px;color:#6b778d;text-align:right;margin-top:18px}";
    f << "</style></head><body><div class='layout'><aside class='sidebar'><div class='brand'>QL Sinh viên</div><div class='sub'>C++ Project - MI3310 KTLT</div><div class='nav'><div class='active'>Dashboard</div><div>Danh sách sinh viên</div><div>Môn học</div><div>Lớp học phần</div><div>Nhập điểm</div><div>Bảng điểm</div><div>Danh sách thi</div></div></aside><main class='main'>";
    f << "<div class='top'><h1>Dashboard quản lý sinh viên và điểm số</h1><div class='badge'>Dữ liệu từ file text</div></div>";
    f << "<section class='cards'>";
    f << "<div class='card'><div class='num'>" << studentCount << "</div><div class='lab'>Sinh viên</div></div>";
    f << "<div class='card'><div class='num'>" << courseCount << "</div><div class='lab'>Môn học</div></div>";
    f << "<div class='card'><div class='num'>" << sectionCount << "</div><div class='lab'>Lớp học phần</div></div>";
    f << "<div class='card'><div class='num'>" << gradeCount << "</div><div class='lab'>Bản ghi điểm</div></div>";
    f << "<div class='card'><div class='num'>" << fmt2(avgSystem) << "</div><div class='lab'>Điểm TB hệ 10</div></div>";
    f << "</section>";

    f << "<section class='panel'><h2>Danh sách sinh viên và GPA</h2><table><tr><th>MSSV</th><th>Họ tên</th><th>Lớp</th><th>Tín chỉ</th><th>TB10</th><th>GPA4</th><th>Xếp loại</th></tr>";
    sCur = students.getHead();
    while (sCur != 0) {
        Student s = sCur->data;
        double avg10 = calculateAverage10(grades, courses, s.id);
        double gpa4 = calculateGPA4(grades, courses, s.id);
        string cls = avg10 >= 8 ? "good" : (avg10 >= 5 ? "warn" : "bad");
        f << "<tr><td>" << htmlEscape(s.id) << "</td><td>" << htmlEscape(s.fullName) << "</td><td>" << htmlEscape(s.className) << "</td><td>" << countCreditsOfStudent(grades, courses, s.id) << "</td><td>" << fmt2(avg10) << "</td><td>" << fmt2(gpa4) << "</td><td><span class='pill " << cls << "'>" << htmlEscape(classifyByAverage10(avg10)) << "</span></td></tr>";
        sCur = sCur->next;
    }
    f << "</table></section>";

    f << "<section class='grid2'><div class='panel'><h2>Bảng điểm lớp MI3310-01</h2><table><tr><th>MSSV</th><th>Họ tên</th><th>QT</th><th>GK</th><th>CK</th><th>Tổng</th></tr>";
    Node<Grade>* gCur = grades.getHead();
    while (gCur != 0) {
        if (gCur->data.sectionId == "MI3310-01") {
            Student* s = findStudent(students, gCur->data.studentId);
            f << "<tr><td>" << htmlEscape(gCur->data.studentId) << "</td><td>" << htmlEscape(s ? s->fullName : "Khong ro") << "</td><td>" << fmt2(gCur->data.processScore) << "</td><td>" << fmt2(gCur->data.midtermScore) << "</td><td>" << fmt2(gCur->data.finalScore) << "</td><td>" << fmt2(gCur->data.total10) << "</td></tr>";
        }
        gCur = gCur->next;
    }
    f << "</table></div><div class='panel'><h2>Kiểm soát nghiệp vụ</h2><table><tr><th>Ràng buộc</th><th>Trạng thái</th></tr>";
    f << "<tr><td>Không nhập điểm ngoài khoảng 0..10</td><td><span class='pill good'>Đang bật</span></td></tr>";
    f << "<tr><td>Không trùng MSSV khi thêm mới</td><td><span class='pill good'>Đang bật</span></td></tr>";
    f << "<tr><td>Tự tính điểm tổng 20% - 30% - 50%</td><td><span class='pill good'>Tự động</span></td></tr>";
    f << "<tr><td>Đọc/ghi file text dạng pipe-separated</td><td><span class='pill good'>Hoạt động</span></td></tr>";
    f << "</table></div></section>";
    f << "<div class='footer'>Sinh bởi module GuiGenerator.cpp - có thể mở trực tiếp bằng trình duyệt.</div>";
    f << "</main></div></body></html>";
    f.close();
    return true;
}
