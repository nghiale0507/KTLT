# -*- coding: utf-8 -*-
"""
Student Manager Web Backend
- App backend chạy riêng, không phụ thuộc src/include của project C++ gốc.
- Đọc/ghi trực tiếp các file .txt trong thư mục ../data.
- Chỉ dùng thư viện chuẩn Python, không cần cài Flask.
"""

from http.server import ThreadingHTTPServer, BaseHTTPRequestHandler
from urllib.parse import urlparse, parse_qs, unquote
from pathlib import Path
import json
import time
import shutil
import mimetypes
import traceback

ROOT_DIR = Path(__file__).resolve().parent
PROJECT_DIR = ROOT_DIR.parent
DATA_DIR = PROJECT_DIR / "data"
STATIC_DIR = ROOT_DIR / "static"
BACKUP_DIR = ROOT_DIR / "backups"

HOST = "127.0.0.1"
PORT = 5050


def split_line(line):
    return [part.strip() for part in line.rstrip("\n").split("|")]


def read_lines(path):
    if not path.exists():
        return []
    with path.open("r", encoding="utf-8") as f:
        return [line.strip() for line in f if line.strip()]


def backup_file(path):
    if not path.exists():
        return
    BACKUP_DIR.mkdir(exist_ok=True)
    stamp = time.strftime("%Y%m%d_%H%M%S")
    shutil.copy2(path, BACKUP_DIR / f"{path.stem}_{stamp}{path.suffix}")


def write_lines(path, lines):
    path.parent.mkdir(exist_ok=True)
    backup_file(path)
    text = "\n".join(lines)
    if text:
        text += "\n"
    with path.open("w", encoding="utf-8") as f:
        f.write(text)


def to_float(value, default=0.0):
    try:
        return float(value)
    except Exception:
        return default


def to_int(value, default=0):
    try:
        return int(value)
    except Exception:
        return default


def round2(x):
    return int(x * 100 + 0.5) / 100.0


def calculate_total10(process, midterm, final_score):
    return round2(process * 0.2 + midterm * 0.3 + final_score * 0.5)


def convert_to_scale4(score10):
    if score10 >= 9.0:
        return 4.0
    if score10 >= 8.5:
        return 3.7
    if score10 >= 8.0:
        return 3.5
    if score10 >= 7.0:
        return 3.0
    if score10 >= 6.5:
        return 2.5
    if score10 >= 5.5:
        return 2.0
    if score10 >= 5.0:
        return 1.5
    if score10 >= 4.0:
        return 1.0
    return 0.0


def classify_by_average10(avg10):
    if avg10 >= 9.0:
        return "Xuất sắc"
    if avg10 >= 8.0:
        return "Giỏi"
    if avg10 >= 6.5:
        return "Khá"
    if avg10 >= 5.0:
        return "Trung bình"
    if avg10 >= 4.0:
        return "Yếu"
    return "Kém"


def validate_score(score):
    return 0.0 <= score <= 10.0


def validate_date(date_text):
    if not isinstance(date_text, str) or len(date_text) != 10:
        return False
    if date_text[4] != "-" or date_text[7] != "-":
        return False
    y, m, d = date_text[:4], date_text[5:7], date_text[8:10]
    if not (y.isdigit() and m.isdigit() and d.isdigit()):
        return False
    month = int(m)
    day = int(d)
    return 1 <= month <= 12 and 1 <= day <= 31


def load_students():
    students = []
    for line in read_lines(DATA_DIR / "students.txt"):
        p = split_line(line)
        if len(p) >= 7:
            students.append({
                "id": p[0],
                "fullName": p[1],
                "className": p[2],
                "birthDate": p[3],
                "gender": p[4],
                "phone": p[5],
                "email": p[6],
            })
    return students


def save_students(students):
    students = sorted(students, key=lambda x: x.get("id", ""))
    lines = ["|".join([
        s.get("id", "").strip(),
        s.get("fullName", "").strip(),
        s.get("className", "").strip(),
        s.get("birthDate", "").strip(),
        s.get("gender", "").strip(),
        s.get("phone", "").strip(),
        s.get("email", "").strip(),
    ]) for s in students]
    write_lines(DATA_DIR / "students.txt", lines)


def load_courses():
    courses = []
    for line in read_lines(DATA_DIR / "courses.txt"):
        p = split_line(line)
        if len(p) >= 3:
            courses.append({"id": p[0], "name": p[1], "credits": to_int(p[2])})
    return courses


def save_courses(courses):
    courses = sorted(courses, key=lambda x: x.get("id", ""))
    lines = ["|".join([
        c.get("id", "").strip(),
        c.get("name", "").strip(),
        str(to_int(c.get("credits", 0))),
    ]) for c in courses]
    write_lines(DATA_DIR / "courses.txt", lines)


def load_sections():
    sections = []
    for line in read_lines(DATA_DIR / "sections.txt"):
        p = split_line(line)
        if len(p) >= 5:
            sections.append({
                "id": p[0],
                "courseId": p[1],
                "semester": p[2],
                "academicYear": p[3],
                "lecturer": p[4],
            })
    return sections


def save_sections(sections):
    sections = sorted(sections, key=lambda x: x.get("id", ""))
    lines = ["|".join([
        s.get("id", "").strip(),
        s.get("courseId", "").strip(),
        s.get("semester", "").strip(),
        s.get("academicYear", "").strip(),
        s.get("lecturer", "").strip(),
    ]) for s in sections]
    write_lines(DATA_DIR / "sections.txt", lines)


def load_grades():
    grades = []
    for line in read_lines(DATA_DIR / "grades.txt"):
        p = split_line(line)
        if len(p) >= 8:
            grades.append({
                "id": p[0],
                "studentId": p[1],
                "courseId": p[2],
                "sectionId": p[3],
                "processScore": to_float(p[4]),
                "midtermScore": to_float(p[5]),
                "finalScore": to_float(p[6]),
                "total10": to_float(p[7]),
            })
    return grades


def save_grades(grades):
    grades = sorted(grades, key=lambda x: (x.get("studentId", ""), x.get("courseId", "")))
    lines = []
    for g in grades:
        process = to_float(g.get("processScore", 0))
        midterm = to_float(g.get("midtermScore", 0))
        final_score = to_float(g.get("finalScore", 0))
        total = calculate_total10(process, midterm, final_score)
        g["total10"] = total
        lines.append("|".join([
            g.get("id", "").strip(),
            g.get("studentId", "").strip(),
            g.get("courseId", "").strip(),
            g.get("sectionId", "").strip(),
            f"{process:g}",
            f"{midterm:g}",
            f"{final_score:g}",
            f"{total:g}",
        ]))
    write_lines(DATA_DIR / "grades.txt", lines)


def build_maps():
    students = load_students()
    courses = load_courses()
    sections = load_sections()
    grades = load_grades()
    return {
        "students": students,
        "courses": courses,
        "sections": sections,
        "grades": grades,
        "student_map": {s["id"]: s for s in students},
        "course_map": {c["id"]: c for c in courses},
        "section_map": {s["id"]: s for s in sections},
    }


def enrich_grades(grades=None):
    data = build_maps()
    if grades is None:
        grades = data["grades"]
    enriched = []
    for g in grades:
        student = data["student_map"].get(g.get("studentId", ""), {})
        course = data["course_map"].get(g.get("courseId", ""), {})
        section = data["section_map"].get(g.get("sectionId", ""), {})
        item = dict(g)
        item["studentName"] = student.get("fullName", "")
        item["className"] = student.get("className", "")
        item["courseName"] = course.get("name", "")
        item["credits"] = course.get("credits", 0)
        item["lecturer"] = section.get("lecturer", "")
        item["semester"] = section.get("semester", "")
        item["scale4"] = convert_to_scale4(item.get("total10", 0))
        item["classification"] = classify_by_average10(item.get("total10", 0))
        enriched.append(item)
    return enriched


def calculate_student_summary(student_id):
    data = build_maps()
    total_credits = 0
    sum10 = 0.0
    sum4 = 0.0
    for g in data["grades"]:
        if g.get("studentId") != student_id:
            continue
        course = data["course_map"].get(g.get("courseId", ""))
        if not course:
            continue
        credits = to_int(course.get("credits"))
        total_credits += credits
        sum10 += g.get("total10", 0) * credits
        sum4 += convert_to_scale4(g.get("total10", 0)) * credits
    if total_credits == 0:
        return {"credits": 0, "average10": 0, "gpa4": 0, "classification": "Chưa có điểm"}
    avg10 = round2(sum10 / total_credits)
    gpa4 = round2(sum4 / total_credits)
    return {
        "credits": total_credits,
        "average10": avg10,
        "gpa4": gpa4,
        "classification": classify_by_average10(avg10),
    }



def exam_report_data(section_id):
    data = build_maps()
    section = data["section_map"].get(section_id)
    if not section:
        raise ValueError("Không tìm thấy lớp học phần.")
    course = data["course_map"].get(section.get("courseId", ""), {})
    rows = []
    for g in data["grades"]:
        if g.get("sectionId") != section_id:
            continue
        student = data["student_map"].get(g.get("studentId", ""), {})
        rows.append({
            "index": len(rows) + 1,
            "studentId": g.get("studentId", ""),
            "studentName": student.get("fullName", "Không rõ"),
            "className": student.get("className", "-"),
            "processScore": g.get("processScore", 0),
            "total10": g.get("total10", 0),
            "status": "Đủ điều kiện" if g.get("processScore", 0) >= 4.0 else "Cần xem xét",
            "signature": "",
        })
    return {
        "school": "TRƯỜNG ĐẠI HỌC BÁCH KHOA HÀ NỘI",
        "faculty": "KHOA TOÁN - TIN",
        "title": "DANH SÁCH SINH VIÊN ĐỦ ĐIỀU KIỆN DỰ THI / KÝ TÊN",
        "section": section,
        "course": course,
        "rows": rows,
        "summary": {
            "total": len(rows),
            "eligible": sum(1 for r in rows if r["status"] == "Đủ điều kiện"),
            "review": sum(1 for r in rows if r["status"] == "Cần xem xét"),
        }
    }

def dashboard_data():
    data = build_maps()
    students = data["students"]
    grades = data["grades"]
    enriched_grades = enrich_grades(grades)
    student_rows = []
    distribution = {"Xuất sắc": 0, "Giỏi": 0, "Khá": 0, "Trung bình": 0, "Yếu": 0, "Kém": 0, "Chưa có điểm": 0}
    for s in students:
        summary = calculate_student_summary(s["id"])
        row = dict(s)
        row.update(summary)
        student_rows.append(row)
        distribution[summary["classification"]] = distribution.get(summary["classification"], 0) + 1
    top_students = sorted(student_rows, key=lambda x: x.get("average10", 0), reverse=True)[:5]
    top_grades = sorted(enriched_grades, key=lambda x: x.get("total10", 0), reverse=True)[:8]
    avg_system = round2(sum([s.get("average10", 0) for s in student_rows]) / len(student_rows)) if student_rows else 0
    return {
        "counts": {
            "students": len(students),
            "courses": len(data["courses"]),
            "sections": len(data["sections"]),
            "grades": len(grades),
        },
        "averageSystem10": avg_system,
        "distribution": distribution,
        "topStudents": top_students,
        "topGrades": top_grades,
    }


def ok(payload=None, status=200):
    if payload is None:
        payload = {}
    return status, payload


def fail(message, status=400):
    return status, {"ok": False, "message": message}


def parse_json_body(handler):
    length = int(handler.headers.get("Content-Length", "0") or "0")
    if length <= 0:
        return {}
    raw = handler.rfile.read(length).decode("utf-8")
    try:
        return json.loads(raw)
    except json.JSONDecodeError:
        raise ValueError("Body JSON không hợp lệ.")


def clean_text(value):
    return str(value or "").replace("|", " ").strip()


def api_get(path, query):
    if path == "/api/health":
        return ok({"ok": True, "message": "Backend đang chạy", "dataDir": str(DATA_DIR)})
    if path == "/api/dashboard":
        return ok({"ok": True, "data": dashboard_data()})
    if path == "/api/students":
        students = load_students()
        keyword = (query.get("q", [""])[0] or "").lower()
        result = []
        for s in students:
            row = dict(s)
            row.update(calculate_student_summary(s["id"]))
            if keyword and keyword not in json.dumps(row, ensure_ascii=False).lower():
                continue
            result.append(row)
        return ok({"ok": True, "data": result})
    if path == "/api/courses":
        return ok({"ok": True, "data": load_courses()})
    if path == "/api/sections":
        sections = load_sections()
        course_map = {c["id"]: c for c in load_courses()}
        result = []
        for s in sections:
            row = dict(s)
            row["courseName"] = course_map.get(s.get("courseId", ""), {}).get("name", "")
            result.append(row)
        return ok({"ok": True, "data": result})
    if path == "/api/grades":
        rows = enrich_grades()
        student_id = query.get("studentId", [""])[0]
        section_id = query.get("sectionId", [""])[0]
        keyword = (query.get("q", [""])[0] or "").lower()
        if student_id:
            rows = [g for g in rows if g.get("studentId") == student_id]
        if section_id:
            rows = [g for g in rows if g.get("sectionId") == section_id]
        if keyword:
            rows = [g for g in rows if keyword in json.dumps(g, ensure_ascii=False).lower()]
        return ok({"ok": True, "data": rows})
    if path == "/api/exam-report":
        section_id = query.get("sectionId", [""])[0]
        if not section_id:
            return fail("Cần chọn lớp học phần.")
        try:
            return ok({"ok": True, "data": exam_report_data(section_id)})
        except ValueError as exc:
            return fail(str(exc), 404)
    return fail("Không tìm thấy API.", 404)


def api_post(path, body):
    if path == "/api/students":
        students = load_students()
        sid = clean_text(body.get("id"))
        if not sid:
            return fail("Mã sinh viên không được để trống.")
        if any(s["id"] == sid for s in students):
            return fail("Mã sinh viên đã tồn tại.")
        birth = clean_text(body.get("birthDate"))
        if birth and not validate_date(birth):
            return fail("Ngày sinh cần có dạng YYYY-MM-DD.")
        new_student = {
            "id": sid,
            "fullName": clean_text(body.get("fullName")),
            "className": clean_text(body.get("className")),
            "birthDate": birth,
            "gender": clean_text(body.get("gender")),
            "phone": clean_text(body.get("phone")),
            "email": clean_text(body.get("email")),
        }
        if not new_student["fullName"]:
            return fail("Họ tên sinh viên không được để trống.")
        students.append(new_student)
        save_students(students)
        return ok({"ok": True, "message": "Đã thêm sinh viên.", "data": new_student}, 201)

    if path == "/api/courses":
        courses = load_courses()
        cid = clean_text(body.get("id"))
        if not cid:
            return fail("Mã môn học không được để trống.")
        if any(c["id"] == cid for c in courses):
            return fail("Mã môn học đã tồn tại.")
        credits = to_int(body.get("credits", 0))
        if credits <= 0:
            return fail("Số tín chỉ phải lớn hơn 0.")
        course = {"id": cid, "name": clean_text(body.get("name")), "credits": credits}
        if not course["name"]:
            return fail("Tên môn học không được để trống.")
        courses.append(course)
        save_courses(courses)
        return ok({"ok": True, "message": "Đã thêm môn học.", "data": course}, 201)

    if path == "/api/sections":
        sections = load_sections()
        courses = load_courses()
        sec_id = clean_text(body.get("id"))
        course_id = clean_text(body.get("courseId"))
        if not sec_id:
            return fail("Mã lớp học phần không được để trống.")
        if any(s["id"] == sec_id for s in sections):
            return fail("Mã lớp học phần đã tồn tại.")
        if not any(c["id"] == course_id for c in courses):
            return fail("Mã môn học không tồn tại.")
        section = {
            "id": sec_id,
            "courseId": course_id,
            "semester": clean_text(body.get("semester")),
            "academicYear": clean_text(body.get("academicYear")),
            "lecturer": clean_text(body.get("lecturer")),
        }
        sections.append(section)
        save_sections(sections)
        return ok({"ok": True, "message": "Đã thêm lớp học phần.", "data": section}, 201)

    if path == "/api/grades":
        return add_or_update_grade(body, creating=True)

    return fail("Không tìm thấy API.", 404)


def add_or_update_grade(body, creating=False):
    students = load_students()
    courses = load_courses()
    sections = load_sections()
    grades = load_grades()
    student_id = clean_text(body.get("studentId"))
    course_id = clean_text(body.get("courseId"))
    section_id = clean_text(body.get("sectionId"))
    if not any(s["id"] == student_id for s in students):
        return fail("Không tồn tại mã sinh viên.")
    if not any(c["id"] == course_id for c in courses):
        return fail("Không tồn tại mã môn học.")
    section = next((s for s in sections if s["id"] == section_id), None)
    if not section:
        return fail("Không tồn tại lớp học phần.")
    if section.get("courseId") != course_id:
        return fail("Lớp học phần không thuộc môn học đã chọn.")
    process = to_float(body.get("processScore"))
    midterm = to_float(body.get("midtermScore"))
    final_score = to_float(body.get("finalScore"))
    if not (validate_score(process) and validate_score(midterm) and validate_score(final_score)):
        return fail("Điểm phải nằm trong khoảng 0..10.")
    gid = f"G_{student_id}_{course_id}"
    grade = {
        "id": gid,
        "studentId": student_id,
        "courseId": course_id,
        "sectionId": section_id,
        "processScore": process,
        "midtermScore": midterm,
        "finalScore": final_score,
        "total10": calculate_total10(process, midterm, final_score),
    }
    idx = next((i for i, g in enumerate(grades) if g.get("studentId") == student_id and g.get("courseId") == course_id), None)
    if idx is None:
        grades.append(grade)
        message = "Đã thêm điểm."
        status = 201
    else:
        grades[idx] = grade
        message = "Đã cập nhật điểm."
        status = 200
    save_grades(grades)
    return ok({"ok": True, "message": message, "data": enrich_grades([grade])[0]}, status)


def api_put(path, body):
    parts = [unquote(p) for p in path.strip("/").split("/")]
    if len(parts) == 3 and parts[0] == "api" and parts[1] == "students":
        sid = parts[2]
        students = load_students()
        idx = next((i for i, s in enumerate(students) if s["id"] == sid), None)
        if idx is None:
            return fail("Không tìm thấy sinh viên.", 404)
        birth = clean_text(body.get("birthDate", students[idx].get("birthDate", "")))
        if birth and not validate_date(birth):
            return fail("Ngày sinh cần có dạng YYYY-MM-DD.")
        students[idx].update({
            "fullName": clean_text(body.get("fullName", students[idx].get("fullName", ""))),
            "className": clean_text(body.get("className", students[idx].get("className", ""))),
            "birthDate": birth,
            "gender": clean_text(body.get("gender", students[idx].get("gender", ""))),
            "phone": clean_text(body.get("phone", students[idx].get("phone", ""))),
            "email": clean_text(body.get("email", students[idx].get("email", ""))),
        })
        save_students(students)
        return ok({"ok": True, "message": "Đã cập nhật sinh viên.", "data": students[idx]})

    if len(parts) == 3 and parts[0] == "api" and parts[1] == "grades":
        body["id"] = parts[2]
        return add_or_update_grade(body, creating=False)

    return fail("Không tìm thấy API.", 404)


def api_delete(path):
    parts = [unquote(p) for p in path.strip("/").split("/")]
    if len(parts) == 3 and parts[0] == "api" and parts[1] == "students":
        sid = parts[2]
        students = load_students()
        before = len(students)
        students = [s for s in students if s["id"] != sid]
        if len(students) == before:
            return fail("Không tìm thấy sinh viên.", 404)
        grades = [g for g in load_grades() if g.get("studentId") != sid]
        save_students(students)
        save_grades(grades)
        return ok({"ok": True, "message": "Đã xóa sinh viên và các điểm liên quan."})

    if len(parts) == 3 and parts[0] == "api" and parts[1] == "grades":
        gid = parts[2]
        grades = load_grades()
        before = len(grades)
        grades = [g for g in grades if g.get("id") != gid]
        if len(grades) == before:
            return fail("Không tìm thấy bản ghi điểm.", 404)
        save_grades(grades)
        return ok({"ok": True, "message": "Đã xóa bản ghi điểm."})

    return fail("Không tìm thấy API.", 404)


class Handler(BaseHTTPRequestHandler):
    server_version = "StudentManagerBackend/1.0"

    def log_message(self, fmt, *args):
        print("[%s] %s" % (time.strftime("%H:%M:%S"), fmt % args))

    def send_json(self, status, payload):
        raw = json.dumps(payload, ensure_ascii=False).encode("utf-8")
        self.send_response(status)
        self.send_header("Content-Type", "application/json; charset=utf-8")
        self.send_header("Content-Length", str(len(raw)))
        self.send_header("Access-Control-Allow-Origin", "*")
        self.send_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS")
        self.send_header("Access-Control-Allow-Headers", "Content-Type")
        self.end_headers()
        self.wfile.write(raw)

    def send_static(self, path):
        if path == "/":
            file_path = STATIC_DIR / "index.html"
        else:
            requested = unquote(path.lstrip("/"))
            file_path = STATIC_DIR / requested
        try:
            file_path = file_path.resolve()
            if not str(file_path).startswith(str(STATIC_DIR.resolve())) or not file_path.exists() or not file_path.is_file():
                self.send_error(404, "File not found")
                return
            content_type = mimetypes.guess_type(str(file_path))[0] or "application/octet-stream"
            raw = file_path.read_bytes()
            self.send_response(200)
            self.send_header("Content-Type", content_type + ("; charset=utf-8" if content_type.startswith("text/") else ""))
            self.send_header("Content-Length", str(len(raw)))
            self.end_headers()
            self.wfile.write(raw)
        except Exception:
            self.send_error(500, "Cannot read static file")

    def handle_api(self, method):
        try:
            parsed = urlparse(self.path)
            path = parsed.path
            query = parse_qs(parsed.query)
            if method == "GET":
                status, payload = api_get(path, query)
            elif method == "POST":
                status, payload = api_post(path, parse_json_body(self))
            elif method == "PUT":
                status, payload = api_put(path, parse_json_body(self))
            elif method == "DELETE":
                status, payload = api_delete(path)
            else:
                status, payload = fail("Phương thức không được hỗ trợ.", 405)
            self.send_json(status, payload)
        except Exception as exc:
            traceback.print_exc()
            self.send_json(500, {"ok": False, "message": str(exc)})

    def do_OPTIONS(self):
        self.send_response(204)
        self.send_header("Access-Control-Allow-Origin", "*")
        self.send_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS")
        self.send_header("Access-Control-Allow-Headers", "Content-Type")
        self.end_headers()

    def do_GET(self):
        parsed = urlparse(self.path)
        if parsed.path.startswith("/api/"):
            self.handle_api("GET")
        else:
            self.send_static(parsed.path)

    def do_POST(self):
        self.handle_api("POST")

    def do_PUT(self):
        self.handle_api("PUT")

    def do_DELETE(self):
        self.handle_api("DELETE")


if __name__ == "__main__":
    print("=" * 72)
    print("STUDENT MANAGER WEB BACKEND")
    print(f"Project dir : {PROJECT_DIR}")
    print(f"Data dir    : {DATA_DIR}")
    print(f"Address     : http://{HOST}:{PORT}")
    print("Nhan Ctrl + C de dung server.")
    print("=" * 72)
    server = ThreadingHTTPServer((HOST, PORT), Handler)
    server.serve_forever()
