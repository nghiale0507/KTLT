# Quản lý sinh viên và điểm số - KTLT MI3310

## Tác giả

* Đào Văn Tuyến - 202419014
* Lê Nguyễn Hữu Nghĩa - 202418954

---

## 1. Giới thiệu dự án

Dự án **Quản lý sinh viên và điểm số** là chương trình được xây dựng bằng ngôn ngữ **C++** nhằm hỗ trợ quản lý thông tin sinh viên, môn học, lớp học phần và kết quả học tập.

Chương trình cung cấp các chức năng chính như:

* Quản lý danh sách sinh viên.
* Quản lý danh sách môn học.
* Quản lý lớp học phần.
* Nhập và cập nhật điểm sinh viên.
* Tính điểm tổng kết học phần.
* Tính điểm trung bình hệ 10.
* Quy đổi điểm sang GPA hệ 4.
* Xếp loại học lực.
* In bảng điểm cá nhân.
* In bảng điểm lớp học phần.
* Lập báo cáo danh sách sinh viên dự thi.
* Xuất giao diện dashboard HTML.
* Cung cấp giao diện Web Backend để thao tác dữ liệu trực quan.

Project sử dụng **danh sách liên kết đơn tự cài đặt** và các thuật toán tìm kiếm, sắp xếp tuần tự. Chương trình không sử dụng các cấu trúc dữ liệu có sẵn như `list`, `vector`, `map` hay hàm `sort` của thư viện chuẩn.

---

## 2. Công nghệ sử dụng

Project sử dụng các công nghệ chính:

```text
Ngôn ngữ chính: C++
Chuẩn C++: C++11
Lưu trữ dữ liệu: File text
Cấu trúc dữ liệu: Danh sách liên kết đơn tự cài đặt
Giao diện chính: Menu dòng lệnh
Giao diện phụ: HTML Dashboard
Web Backend: Python http.server
Kiểm thử: C++ test runner
```

---

## 3. Cấu trúc thư mục

Cấu trúc project gồm các thư mục và file chính:

```text
Quan_ly_sinh_vien_CPP_GUI/
│
├── include/
│   ├── Models.h
│   ├── LinkedList.h
│   ├── DataStore.h
│   ├── Services.h
│   ├── Ui.h
│   └── GuiGenerator.h
│
├── src/
│   ├── main.cpp
│   ├── DataStore.cpp
│   ├── Services.cpp
│   ├── Ui.cpp
│   └── GuiGenerator.cpp
│
├── data/
│   ├── students.txt
│   ├── courses.txt
│   ├── sections.txt
│   └── grades.txt
│
├── tests/
│   └── test_runner.cpp
│
├── gui/
│   └── dashboard.html
│
├── web_backend_app/
│   ├── server.py
│   ├── Chay_Web_Backend.bat
│   ├── README_WEB_BACKEND.txt
│   └── static/
│       ├── index.html
│       ├── app.css
│       └── app.js
│
├── docs/
│   └── figures/
│
├── report/
│   └── Bao_cao_Quan_ly_sinh_vien_LaTeX.pdf
│
├── Makefile
└── README.md
```

---

## 4. Mô tả các thành phần chính

### 4.1. Thư mục `include/`

Thư mục `include/` chứa các file khai báo cấu trúc dữ liệu, hàm xử lý và giao diện chương trình.

Các file chính:

```text
Models.h        Khai báo các kiểu dữ liệu Student, Course, ClassSection, Grade
LinkedList.h    Cài đặt danh sách liên kết đơn dạng template
DataStore.h     Khai báo các hàm đọc và ghi dữ liệu
Services.h      Khai báo các hàm xử lý nghiệp vụ
Ui.h            Khai báo các hàm giao diện menu console
GuiGenerator.h  Khai báo hàm xuất dashboard HTML
```

---

### 4.2. Thư mục `src/`

Thư mục `src/` chứa phần cài đặt chương trình.

Các file chính:

```text
main.cpp          Điểm bắt đầu chương trình
DataStore.cpp     Xử lý đọc và ghi dữ liệu từ file text
Services.cpp      Xử lý nghiệp vụ như tính điểm, GPA, tìm kiếm, sắp xếp
Ui.cpp            Xử lý giao diện menu dòng lệnh
GuiGenerator.cpp  Xuất dashboard HTML từ dữ liệu hiện có
```

---

### 4.3. Thư mục `data/`

Thư mục `data/` chứa các file dữ liệu của chương trình.

```text
students.txt   Lưu danh sách sinh viên
courses.txt    Lưu danh sách môn học
sections.txt   Lưu danh sách lớp học phần
grades.txt     Lưu danh sách điểm
```

Dữ liệu được lưu dưới dạng text, các trường được phân cách bằng dấu `|`.

---

### 4.4. Thư mục `tests/`

Thư mục `tests/` chứa file kiểm thử chương trình:

```text
test_runner.cpp
```

File này dùng để kiểm tra các chức năng quan trọng như:

* Tính điểm tổng kết.
* Quy đổi điểm hệ 4.
* Xếp loại học lực.
* Kiểm tra điểm hợp lệ.
* Kiểm tra ngày sinh.
* Thêm, tìm kiếm, xóa node trong danh sách liên kết.
* Sắp xếp sinh viên.
* Tính điểm trung bình và GPA.

---

### 4.5. Thư mục `gui/`

Thư mục `gui/` chứa file dashboard HTML được sinh ra từ chương trình C++:

```text
dashboard.html
```

Dashboard này dùng để xem dữ liệu tổng quan trên trình duyệt.

---

### 4.6. Thư mục `web_backend_app/`

Thư mục `web_backend_app/` chứa giao diện web có backend riêng.

Giao diện này được xây dựng tách biệt với source C++ gốc. Backend đọc và ghi trực tiếp dữ liệu trong thư mục `data/`, giúp người dùng có thể thêm, sửa, xóa dữ liệu thông qua trình duyệt.

---

## 5. Định dạng dữ liệu

### 5.1. File `students.txt`

Lưu thông tin sinh viên.

Định dạng:

```text
MSSV|Họ tên|Lớp|Ngày sinh|Giới tính|SĐT|Email
```

Ví dụ:

```text
202418954|Le Nguyen Huu Nghia|MI2-01|2006-08-12|Nam|0977123456|nghia.lnh@example.edu.vn
```

---

### 5.2. File `courses.txt`

Lưu thông tin môn học.

Định dạng:

```text
Mã môn|Tên môn|Số tín chỉ
```

Ví dụ:

```text
MI3310|Ky thuat lap trinh|3
```

---

### 5.3. File `sections.txt`

Lưu thông tin lớp học phần.

Định dạng:

```text
Mã lớp học phần|Mã môn|Học kỳ|Năm học|Giảng viên
```

Ví dụ:

```text
MI3310-01|MI3310|2024.1|2024-2025|Nguyen Van A
```

---

### 5.4. File `grades.txt`

Lưu thông tin điểm sinh viên.

Định dạng:

```text
Mã điểm|MSSV|Mã môn|Mã lớp HP|Điểm QT|Điểm GK|Điểm CK|Điểm tổng
```

Ví dụ:

```text
G_202418954_MI3310|202418954|MI3310|MI3310-01|8.0|7.5|9.0|8.35
```

---

## 6. Công thức tính điểm

Điểm tổng kết học phần được tính theo công thức:

```text
Điểm tổng = 20% điểm quá trình + 30% điểm giữa kỳ + 50% điểm cuối kỳ
```

Công thức trong chương trình:

```cpp
total10 = processScore * 0.2 + midtermScore * 0.3 + finalScore * 0.5
```

Kết quả được làm tròn đến 2 chữ số sau dấu phẩy.

---

## 7. Quy đổi điểm hệ 10 sang hệ 4

Chương trình quy đổi điểm hệ 10 sang hệ 4 theo bảng sau:

```text
Điểm hệ 10     Điểm hệ 4
>= 9.0         4.0
>= 8.5         3.7
>= 8.0         3.5
>= 7.0         3.0
>= 6.5         2.5
>= 5.5         2.0
>= 5.0         1.5
>= 4.0         1.0
< 4.0          0.0
```

---

## 8. Xếp loại học lực

Chương trình xếp loại học lực theo điểm trung bình hệ 10:

```text
Điểm trung bình     Xếp loại
>= 9.0              Xuất sắc
>= 8.0              Giỏi
>= 6.5              Khá
>= 5.0              Trung bình
>= 4.0              Yếu
< 4.0               Kém
```

---

## 9. Chức năng của chương trình console

Khi chạy chương trình chính, người dùng thao tác thông qua menu dòng lệnh.

Các chức năng chính gồm:

```text
1. Hiển thị danh sách sinh viên
2. Thêm sinh viên
3. Cập nhật thông tin sinh viên
4. Xóa sinh viên
5. Hiển thị danh sách môn học
6. Nhập / cập nhật điểm
7. Tìm kiếm và hiển thị bảng điểm một sinh viên
8. In bảng điểm lớp học phần
9. Sắp xếp sinh viên theo MSSV / tên
10. Báo cáo danh sách thi
11. Xuất giao diện GUI Dashboard dạng HTML
0. Lưu dữ liệu và kết thúc
```

---

## 10. Cách biên dịch và chạy chương trình console

### 10.1. Biên dịch bằng Makefile

Mở terminal tại thư mục gốc project, sau đó chạy:

```bash
make
```

Sau khi biên dịch thành công, chạy chương trình:

```bash
./student_manager
```

Trên Windows, nếu file sinh ra là `.exe`, có thể chạy:

```bash
.\student_manager.exe
```

---

### 10.2. Trường hợp Windows không nhận lệnh `make`

Một số máy Windows không có sẵn lệnh `make`.

Nếu dùng MinGW, có thể thử:

```bash
mingw32-make
```

Sau đó chạy:

```bash
.\student_manager.exe
```

---

### 10.3. Biên dịch thủ công không dùng Makefile

Có thể biên dịch trực tiếp bằng `g++`:

```bash
g++ -std=c++11 -Wall -Wextra -pedantic src/main.cpp src/DataStore.cpp src/Services.cpp src/GuiGenerator.cpp src/Ui.cpp -o student_manager.exe
```

Sau đó chạy:

```bash
.\student_manager.exe
```

---

## 11. Chạy kiểm thử

### 11.1. Chạy bằng Makefile

```bash
make test
```

Hoặc trên Windows với MinGW:

```bash
mingw32-make test
```

---

### 11.2. Biên dịch và chạy test thủ công

Biên dịch file test:

```bash
g++ -std=c++11 -Wall -Wextra -pedantic tests/test_runner.cpp src/Services.cpp -o test_runner.exe
```

Chạy test trên PowerShell:

```bash
.\test_runner.exe
```

Chạy test trên CMD:

```cmd
test_runner.exe
```

Chạy test trên Git Bash hoặc MSYS2:

```bash
./test_runner.exe
```

Nếu bấm đúp trực tiếp vào `test_runner.exe`, cửa sổ có thể tắt ngay vì chương trình chạy xong rất nhanh. Nên chạy file test bằng terminal để xem kết quả.

---

## 12. Xuất giao diện GUI Dashboard HTML

Chương trình console có chức năng số 11 để xuất dashboard HTML từ dữ liệu hiện có.

Cách chạy:

```bash
make
printf "11\n0\n" | ./student_manager
```

Sau đó mở file sau bằng trình duyệt:

```text
gui/dashboard.html
```

Trên Windows, có thể chạy chương trình rồi chọn thủ công chức năng:

```text
11. Xuất giao diện GUI Dashboard dạng HTML
```

Dashboard HTML dùng để hiển thị dữ liệu tổng quan như:

* Tổng số sinh viên.
* Tổng số môn học.
* Tổng số lớp học phần.
* Tổng số bản ghi điểm.
* Điểm trung bình toàn hệ thống.
* Danh sách sinh viên.
* Bảng điểm.
* Thống kê kết quả học tập.

Dashboard HTML là giao diện tĩnh. Nó phù hợp để xem dữ liệu và báo cáo tổng quan, nhưng không trực tiếp ghi dữ liệu mới vào file.

---

## 13. Giao diện Web Backend

Ngoài giao diện console và dashboard HTML, project còn có thêm giao diện Web Backend trong thư mục:

```text
web_backend_app/
```

Giao diện này có backend thật, cho phép người dùng thao tác dữ liệu trực tiếp trên trình duyệt.

Điểm quan trọng:

```text
Không thay đổi source C++ gốc
Không thay đổi cấu trúc thư mục src/
Không thay đổi cấu trúc thư mục include/
Không thay đổi Makefile gốc
Đọc và ghi trực tiếp dữ liệu trong thư mục data/
```

---

## 14. Cách chạy Web Backend

### 14.1. Chạy bằng file `.bat`

Trên Windows, mở thư mục:

```text
web_backend_app/
```

Bấm đúp file:

```text
Chay_Web_Backend.bat
```

Sau đó trình duyệt sẽ mở địa chỉ:

```text
http://127.0.0.1:5050
```

---

### 14.2. Chạy bằng terminal

Mở terminal tại thư mục `web_backend_app/`:

```bash
cd web_backend_app
python server.py
```

Sau đó mở trình duyệt và truy cập:

```text
http://127.0.0.1:5050
```

---

## 15. Chức năng của Web Backend

Giao diện Web Backend hỗ trợ các chức năng chính:

```text
Dashboard thống kê tổng quan
Xem danh sách sinh viên
Thêm sinh viên
Sửa thông tin sinh viên
Xóa sinh viên
Xem danh sách môn học
Thêm môn học
Xem danh sách lớp học phần
Thêm lớp học phần
Xem bảng điểm
Thêm / cập nhật điểm
Xóa bản ghi điểm
Tra cứu sinh viên
Lọc điểm theo lớp học phần
Xuất CSV bảng điểm
In báo cáo danh sách thi
Đổi giao diện sáng / tối
Tìm kiếm nhanh
```

---

## 16. Báo cáo danh sách thi trên Web Backend

Web Backend có tab riêng:

```text
Danh sách thi
```

Chức năng này dùng để lập báo cáo danh sách sinh viên dự thi theo lớp học phần.

Người dùng có thể:

* Chọn lớp học phần.
* Tạo danh sách sinh viên thuộc lớp học phần đó.
* Xem thông tin môn học, học kỳ, năm học và giảng viên.
* Xem điểm quá trình và điểm tổng kết.
* Kiểm tra trạng thái đủ điều kiện dự thi.
* In báo cáo.
* Xuất báo cáo ra CSV.

Quy tắc xét điều kiện dự thi:

```text
Điểm quá trình >= 4.0  → Đủ điều kiện
Điểm quá trình < 4.0   → Cần xem xét
```

Bảng danh sách thi có các cột chính:

```text
STT
MSSV
Họ tên
Lớp
Điểm quá trình
Điểm tổng
Trạng thái
Ký tên
```

---

## 17. Dữ liệu của Web Backend

Web Backend sử dụng chung dữ liệu với chương trình C++:

```text
data/students.txt
data/courses.txt
data/sections.txt
data/grades.txt
```

Khi người dùng thêm, sửa hoặc xóa dữ liệu trên giao diện web, backend sẽ cập nhật trực tiếp vào các file `.txt` tương ứng.

Mỗi lần ghi dữ liệu, backend có thể tạo bản sao lưu trong thư mục:

```text
web_backend_app/backups/
```

Điều này giúp hạn chế mất dữ liệu khi thao tác nhầm.

---

## 18. Yêu cầu môi trường

Để chạy đầy đủ project, máy cần có:

```text
g++       Dùng để biên dịch chương trình C++
make      Dùng để chạy Makefile, không bắt buộc nếu biên dịch thủ công
Python 3  Dùng để chạy Web Backend
Trình duyệt web  Dùng để mở dashboard HTML và Web Backend
```

Kiểm tra `g++`:

```bash
g++ --version
```

Kiểm tra Python:

```bash
python --version
```

Hoặc:

```bash
python3 --version
```

---

## 19. Một số lỗi thường gặp

### 19.1. Lỗi không nhận lệnh `make`

Thông báo thường gặp:

```text
make is not recognized
```

Cách xử lý:

```bash
mingw32-make
```

Hoặc biên dịch thủ công bằng `g++`.

---

### 19.2. Lỗi không nhận lệnh `g++`

Thông báo thường gặp:

```text
g++ is not recognized
```

Nguyên nhân là máy chưa cài MinGW-w64/MSYS2 hoặc chưa thêm `g++` vào PATH.

Cách kiểm tra:

```bash
g++ --version
```

---

### 19.3. File test `.exe` bật lên rồi tắt ngay

Đây không phải lỗi. File test chạy xong rất nhanh nên cửa sổ tự tắt.

Cách chạy đúng là mở terminal rồi chạy:

```bash
.\test_runner.exe
```

---

### 19.4. Web Backend không chạy

Kiểm tra Python:

```bash
python --version
```

Sau đó chạy:

```bash
cd web_backend_app
python server.py
```

Nếu cổng `5050` đang bị dùng, cần đóng tiến trình cũ hoặc đổi cổng trong file `server.py`.

---

## 20. Ghi chú kỹ thuật

Project được thiết kế theo hướng chia module rõ ràng:

```text
Models        Định nghĩa dữ liệu
LinkedList    Cấu trúc danh sách liên kết đơn
DataStore     Đọc và ghi file
Services      Xử lý nghiệp vụ
Ui            Giao diện menu console
GuiGenerator  Xuất dashboard HTML
Web Backend   Giao diện web có backend riêng
```

Các thao tác xử lý danh sách đều dựa trên danh sách liên kết đơn tự cài đặt. Việc tìm kiếm được thực hiện bằng cách duyệt tuần tự, còn sắp xếp được thực hiện bằng thuật toán bubble sort trên danh sách liên kết.

---

## 21. Kết luận

Dự án **Quản lý sinh viên và điểm số - KTLT MI3310** là một chương trình quản lý học tập cơ bản được xây dựng bằng C++.

Project đáp ứng các yêu cầu chính của học phần Kỹ thuật lập trình:

* Có tổ chức chương trình theo nhiều module.
* Có tự cài đặt cấu trúc dữ liệu danh sách liên kết đơn.
* Có xử lý file để lưu trữ dữ liệu.
* Có các chức năng thêm, sửa, xóa, tìm kiếm và sắp xếp.
* Có xử lý nghiệp vụ tính điểm, GPA và xếp loại.
* Có kiểm thử chức năng.
* Có giao diện dashboard HTML.
* Có giao diện Web Backend để thao tác dữ liệu trực quan.

Chương trình phù hợp để sử dụng làm project cuối kỳ môn Kỹ thuật lập trình MI3310.
