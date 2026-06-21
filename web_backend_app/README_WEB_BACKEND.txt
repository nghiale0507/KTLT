STUDENT MANAGER - WEB BACKEND APP
=================================

1. Muc dich
-----------
Thu muc web_backend_app la app web co backend that, duoc dat them vao project
goc nhung khong sua cau truc source C++ ban dau.

Project goc van giu nguyen:
- src/
- include/
- data/
- tests/
- gui/
- Makefile

App backend chi doc/ghi vao cac file:
- ../data/students.txt
- ../data/courses.txt
- ../data/sections.txt
- ../data/grades.txt

2. Cach chay
------------
Mo thu muc web_backend_app va bam dup:

    Chay_Web_Backend.bat

Sau do trinh duyet se mo:

    http://127.0.0.1:5050

3. Yeu cau
----------
Can cai Python 3. Khong can cai Flask hay thu vien ngoai.

Kiem tra Python:

    python --version

4. Chuc nang
------------
- Dashboard thong ke dong
- Xem danh sach sinh vien, mon hoc, lop hoc phan, bang diem
- Them sinh vien
- Sua sinh vien
- Xoa sinh vien va diem lien quan
- Them mon hoc
- Them lop hoc phan
- Them/cap nhat diem
- Xoa ban ghi diem
- Tim kiem nhanh
- Loc bang diem theo lop hoc phan
- Xuat CSV bang diem
- Bao cao in danh sach thi / ky ten theo lop hoc phan
- In bao cao danh sach thi ra A4 ngang
- Xuat CSV danh sach thi
- Che do sang/toi

5. Luu y ve sao luu
-------------------
Moi lan ghi file data, backend tu tao ban backup trong:

    web_backend_app/backups/

Neu nhap sai du lieu, ban co the lay lai file txt cu tu thu muc backups.


6. Bao cao danh sach thi
------------------------
Trong giao dien web, chon tab:

    Danh sach thi

Cach dung:
- Chon lop hoc phan.
- Bam Tao bao cao.
- Bam In bao cao de in danh sach thi / ky ten.
- Bam Xuat CSV de luu danh sach thi.

Quy tac trang thai du thi duoc lay theo ban console goc:
- Diem qua trinh >= 4.0: Du dieu kien.
- Diem qua trinh < 4.0: Can xem xet.
