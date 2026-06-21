# Quản lý sinh viên và điểm số - KTLT MI3310

## Tác giả
- Đào Văn Tuyến - 202419014
- Lê Nguyễn Hữu Nghĩa - 202418954

## Cách biên dịch và chạy
```bash
make
./student_manager
```

## Chạy kiểm thử
```bash
make test
```

## Dữ liệu
Các file dữ liệu đặt trong thư mục `data/`:
- `students.txt`: MSSV|Họ tên|Lớp|Ngày sinh|Giới tính|SĐT|Email
- `courses.txt`: Mã môn|Tên môn|Số tín chỉ
- `sections.txt`: Mã lớp học phần|Mã môn|Học kỳ|Năm học|Giảng viên
- `grades.txt`: Mã điểm|MSSV|Mã môn|Mã lớp HP|Điểm QT|Điểm GK|Điểm CK|Điểm tổng

Chương trình sử dụng danh sách liên kết đơn tự cài đặt và các thuật toán tìm kiếm/sắp xếp tuần tự, không dùng list/vector/map/sort có sẵn.

## Xuất giao diện GUI Dashboard
Chương trình có thêm chức năng số 11 để xuất giao diện Dashboard dạng HTML từ dữ liệu hiện có:
```bash
make
printf "11\n0\n" | ./student_manager
# mở file gui/dashboard.html bằng trình duyệt
```
Giao diện này không dùng cơ sở dữ liệu hay thư viện cấu trúc dữ liệu nâng cao; toàn bộ số liệu được lấy từ các danh sách liên kết đơn và các file text trong thư mục data/.
