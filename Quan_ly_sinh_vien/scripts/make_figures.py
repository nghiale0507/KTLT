from PIL import Image, ImageDraw, ImageFont
from pathlib import Path
import textwrap
ROOT = Path(__file__).resolve().parents[1]
FIG = ROOT/'docs'/'figures'
LOG = ROOT/'logs'
FIG.mkdir(parents=True, exist_ok=True)

font_path = '/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf'
font_bold_path = '/usr/share/fonts/truetype/dejavu/DejaVuSansMono-Bold.ttf'
sans_path = '/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf'
sans_bold_path = '/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf'
font = ImageFont.truetype(font_path, 20)
small_mono = ImageFont.truetype(font_path, 18)
bold = ImageFont.truetype(font_bold_path, 22)
title_font = ImageFont.truetype(sans_bold_path, 30)
sans = ImageFont.truetype(sans_path, 21)
sans_small = ImageFont.truetype(sans_path, 18)
sans_bold = ImageFont.truetype(sans_bold_path, 23)
sans_title = ImageFont.truetype(sans_bold_path, 34)


def terminal_image(logfile, outfile, max_lines=48, title=None, width=1800):
    text = (LOG/logfile).read_text(encoding='utf-8', errors='ignore')
    lines = text.splitlines()
    if len(lines) > max_lines:
        lines = lines[:max_lines]
        lines.append('...')
    pad = 34
    line_h = 27
    title_h = 50 if title else 0
    height = pad*2 + title_h + line_h * max(1, len(lines))
    im = Image.new('RGB', (width, height), '#f7f8fb')
    d = ImageDraw.Draw(im)
    d.rounded_rectangle([16,16,width-16,height-16], radius=18, fill='#1e1e1e', outline='#c7c7c7', width=2)
    # window dots
    d.ellipse([36,32,52,48], fill='#ff5f56')
    d.ellipse([60,32,76,48], fill='#ffbd2e')
    d.ellipse([84,32,100,48], fill='#27c93f')
    y = 58
    if title:
        d.text((120,28), title, fill='#ffffff', font=bold)
        y += title_h - 46
    for line in lines:
        # Crop very long lines visually but keep meaning
        if len(line) > 150:
            line = line[:147] + '...'
        d.text((36,y), line, fill='#edf2f4', font=font)
        y += line_h
    im.save(FIG/outfile)

for log, out, ml, title in [
    ('cli_menu.txt','cli_menu.png',24,'Giao diện menu chính CLI'),
    ('list_students.txt','list_students.png',34,'Danh sách sinh viên và GPA'),
    ('transcript_student.txt','transcript_student.png',36,'Bảng điểm cá nhân'),
    ('section_scoreboard.txt','section_scoreboard.png',42,'Bảng điểm lớp học phần'),
    ('sort_students_name.txt','sort_students_name.png',38,'Sắp xếp danh sách theo tên'),
    ('update_grade.txt','update_grade.png',32,'Nhập/Cập nhật điểm có kiểm tra dữ liệu'),
    ('exam_signature_report.txt','exam_signature_report.png',45,'Báo cáo in và ký danh sách thi'),
    ('gui_export.txt','gui_export.png',27,'Xuất giao diện GUI Dashboard HTML'),
    ('test_runner.txt','test_runner.png',26,'Kết quả kiểm thử tự động'),
]:
    if (LOG/log).exists():
        terminal_image(log, out, ml, title)


def arrow(d, x1,y1,x2,y2, fill='#222', width=3):
    d.line([x1,y1,x2,y2], fill=fill, width=width)
    if abs(x2-x1) >= abs(y2-y1):
        if x2 >= x1:
            pts=[(x2,y2),(x2-13,y2-7),(x2-13,y2+7)]
        else:
            pts=[(x2,y2),(x2+13,y2-7),(x2+13,y2+7)]
    else:
        if y2 >= y1:
            pts=[(x2,y2),(x2-7,y2-13),(x2+7,y2-13)]
        else:
            pts=[(x2,y2),(x2-7,y2+13),(x2+7,y2+13)]
    d.polygon(pts, fill=fill)


def rounded_box(d, xy, title, body='', fill='#ffffff', outline='#34495e'):
    d.rounded_rectangle(xy, radius=16, fill=fill, outline=outline, width=3)
    x1,y1,x2,y2=xy
    d.text((x1+18,y1+12), title, fill='#1f2d3d', font=sans_bold)
    if body:
        y=y1+48
        for line in textwrap.wrap(body, width=max(18, int((x2-x1)/12))):
            d.text((x1+18,y), line, fill='#263238', font=sans_small)
            y += 24

# linked list design
im = Image.new('RGB',(1500,620),'#fbfcfe')
d = ImageDraw.Draw(im)
d.text((40,25),'Thiết kế danh sách liên kết đơn tự cài đặt',fill='#1f2d3d',font=title_font)
rounded_box(d,(55,130,250,240),'head','con trỏ đầu danh sách',fill='#eaf3ff')
for i,x in enumerate([360,640,920]):
    d.rounded_rectangle([x,100,x+220,270], radius=15, fill='#ffffff', outline='#2c3e50', width=3)
    d.rectangle([x+20,135,x+110,205], fill='#d6ecff', outline='#2c3e50', width=2)
    d.rectangle([x+110,135,x+200,205], fill='#ffe8c2', outline='#2c3e50', width=2)
    d.text((x+36,156),f'data {i+1}',fill='#1f2d3d',font=sans_small)
    d.text((x+126,156),'next',fill='#1f2d3d',font=sans_small)
    d.text((x+22,225),['Student','Course','Grade'][i],fill='#1f2d3d',font=sans_bold)
arrow(d,250,185,360,185); arrow(d,560,185,640,185); arrow(d,840,185,920,185); arrow(d,1140,185,1245,185)
d.rounded_rectangle([1250,145,1360,225], radius=12, fill='#f0f0f0', outline='#2c3e50', width=3)
d.text((1275,172),'NULL',fill='#222',font=sans_bold)
rounded_box(d,(110,360,1380,545),'Ý nghĩa kỹ thuật','Mỗi Node chỉ lưu một đối tượng dữ liệu và một con trỏ next. Các thao tác thêm, xóa, tìm kiếm và sắp xếp đều được nhóm tự cài đặt bằng duyệt tuần tự, không dùng list/vector/map/sort có sẵn.',fill='#f8fbff')
im.save(FIG/'linked_list_design.png')

# data model relations
im = Image.new('RGB',(1500,760),'#fbfcfe'); d=ImageDraw.Draw(im)
d.text((40,25),'Mô hình dữ liệu nghiệp vụ quản lý sinh viên',fill='#1f2d3d',font=title_font)
rounded_box(d,(80,120,440,260),'Student','MSSV, họ tên, lớp, ngày sinh, giới tính, liên hệ',fill='#eaf3ff')
rounded_box(d,(585,120,945,260),'Grade','MSSV, mã môn, lớp học phần, điểm QT/GK/CK, điểm tổng',fill='#fff7e6')
rounded_box(d,(1060,120,1420,260),'Course','Mã môn, tên môn, số tín chỉ',fill='#eaf8ed')
rounded_box(d,(585,410,945,550),'ClassSection','Mã lớp học phần, mã môn, học kỳ, năm học, giảng viên',fill='#f4ecff')
arrow(d,440,190,585,190); d.text((465,155),'1 - n',fill='#333',font=sans_bold)
arrow(d,945,190,1060,190); d.text((975,155),'n - 1',fill='#333',font=sans_bold)
arrow(d,765,410,765,260); d.text((785,325),'n - 1',fill='#333',font=sans_bold)
rounded_box(d,(120,620,1380,710),'Khóa liên kết','Grade là bảng trung gian kết nối Student, Course và ClassSection. Từ Grade có thể tính điểm trung bình hệ 10, GPA hệ 4, xếp loại và lập báo cáo thi.',fill='#ffffff')
im.save(FIG/'data_model_relations.png')

# module dependency
im = Image.new('RGB',(1600,930),'#fbfcfe'); d=ImageDraw.Draw(im)
d.text((45,30),'Sơ đồ phụ thuộc giữa các module C++',fill='#1f2d3d',font=title_font)
boxes = {
    'main.cpp':(650,95,950,185),
    'Ui.cpp / Ui.h':(650,250,950,350),
    'Services.cpp / Services.h':(190,455,570,575),
    'DataStore.cpp / DataStore.h':(1010,455,1390,575),
    'GuiGenerator.cpp / GuiGenerator.h':(610,455,990,575),
    'LinkedList.h':(340,710,670,820),
    'Models.h':(780,710,1110,820),
    'data/*.txt + gui/*.html':(1150,710,1510,820)
}
colors=['#eaf3ff','#fff7e6','#eaf8ed','#f4ecff','#fff3cd','#ffffff','#ffffff','#fff0f0']
for (i,(name,xy)) in enumerate(boxes.items()): rounded_box(d,xy,name,'',fill=colors[i])
arrow(d,800,185,800,250)
arrow(d,650,320,470,455)
arrow(d,800,350,800,455)
arrow(d,950,320,1180,455)
arrow(d,380,575,500,710)
arrow(d,800,575,930,710)
arrow(d,1180,575,1000,710)
arrow(d,1180,575,1320,710)
arrow(d,570,515,780,760); arrow(d,990,515,930,710)
rounded_box(d,(90,850,1510,905),'Ghi chú','Tầng UI điều phối menu; Services xử lý nghiệp vụ; DataStore đọc/ghi file; GuiGenerator xuất dashboard HTML; LinkedList và Models là nền tảng dữ liệu dùng chung.',fill='#ffffff')
im.save(FIG/'module_dependency.png')

# workflow
im = Image.new('RGB',(1700,760),'#fbfcfe'); d=ImageDraw.Draw(im)
d.text((45,30),'Luồng xử lý tổng thể của chương trình',fill='#1f2d3d',font=title_font)
steps=[('Khởi động','Nạp dữ liệu từ file'),('Menu','Người dùng chọn tác vụ'),('Nghiệp vụ','Thêm/sửa/xóa/nhập điểm'),('RAM','Danh sách liên kết thay đổi'),('Báo cáo/GUI','In bảng hoặc xuất HTML'),('Lưu file','Ghi lại data/*.txt')]
x=50
for idx,(t,b) in enumerate(steps):
    rounded_box(d,(x,180,x+235,340),t,b,fill=['#eaf3ff','#fff7e6','#eaf8ed','#f4ecff','#fff3cd','#fff0f0'][idx])
    if idx < len(steps)-1: arrow(d,x+235,260,x+285,260)
    x += 275
rounded_box(d,(170,500,1530,650),'Điểm kiểm soát lỗi','Trước khi cập nhật dữ liệu, chương trình kiểm tra mã sinh viên, mã môn, mã lớp học phần, định dạng ngày sinh và khoảng điểm 0..10. Nếu dữ liệu sai, hệ thống trả thông báo và không thay đổi file dữ liệu.',fill='#ffffff')
im.save(FIG/'workflow.png')

# file organization
im = Image.new('RGB',(1500,780),'#fbfcfe'); d=ImageDraw.Draw(im)
d.text((45,30),'Cây thư mục bàn giao chương trình',fill='#1f2d3d',font=title_font)
code = '''student_management_project/
├── include/          # file header: Models, LinkedList, Services, DataStore, UI, GUI
├── src/              # mã nguồn C++ triển khai module
├── data/             # students.txt, courses.txt, sections.txt, grades.txt
├── gui/              # dashboard.html sinh từ chương trình
├── tests/            # test_runner.cpp kiểm thử tự động
├── docs/figures/     # hình ảnh chụp màn hình đưa vào báo cáo
├── latex/            # main.tex và PDF biên dịch
├── logs/             # transcript kết quả chạy chương trình
├── Makefile
└── README.md'''
y=115
for line in code.splitlines():
    d.text((110,y),line,fill='#1f2d3d',font=ImageFont.truetype(font_path,24)); y += 45
rounded_box(d,(850,130,1400,640),'Nguyên tắc tổ chức','Mã nguồn, dữ liệu, hình ảnh, test và báo cáo được tách thư mục rõ ràng. Khi nộp chỉ cần nén toàn bộ thư mục chương trình và nộp kèm PDF/LaTeX báo cáo.',fill='#ffffff')
im.save(FIG/'file_tree.png')

# GUI dashboard mockups based on actual data
im = Image.new('RGB',(1700,1000),'#eef3f8'); d=ImageDraw.Draw(im)
d.rounded_rectangle([40,40,1660,940],radius=30,fill='#ffffff',outline='#d0d7de',width=2)
d.rounded_rectangle([70,90,360,890],radius=25,fill='#1f2d3d')
d.text((100,125),'QL SINH VIÊN',fill='#ffffff',font=sans_bold)
d.text((100,158),'C++ - MI3310',fill='#b8c7d9',font=sans_small)
for i,item in enumerate(['Dashboard','Danh sách SV','Môn học','Lớp học phần','Nhập điểm','Bảng điểm','Danh sách thi']):
    y=220+i*72
    d.rounded_rectangle([95,y,335,y+45],radius=12,fill='#2f80ed' if i==0 else '#34495e')
    d.text((115,y+10),item,fill='#ffffff',font=sans)
d.text((420,105),'Dashboard quản lý sinh viên và điểm số',fill='#1f2d3d',font=sans_title)
card_data=[('08','Sinh viên','#eaf3ff'),('06','Môn học','#eaf8ed'),('05','Lớp học phần','#fff7e6'),('14','Bản ghi điểm','#fff0f0'),('7.65','TB hệ 10','#f4ecff')]
for i,(num,lab,col) in enumerate(card_data):
    x=420+i*245
    d.rounded_rectangle([x,175,x+215,285],radius=20,fill=col,outline='#d0d7de')
    d.text((x+22,198),num,fill='#1f2d3d',font=title_font)
    d.text((x+22,245),lab,fill='#334',font=sans_small)
d.rounded_rectangle([420,330,1600,620],radius=20,fill='#fbfcfe',outline='#d0d7de')
d.text((455,355),'Danh sách sinh viên và GPA',fill='#1f2d3d',font=sans_bold)
rows=[('202419014','Dao Van Tuyen','MI2-01','8','8.34','3.57','Giỏi'),('202418954','Le Nguyen Huu Nghia','MI2-01','8','8.08','3.31','Giỏi'),('202400101','Nguyen Minh Anh','MI2-01','6','8.75','3.83','Giỏi'),('202400104','Hoang Gia Bao','MI2-02','3','3.90','0.00','Kém')]
y=410
d.text((455,y),'MSSV          Họ tên                    Lớp       TC    TB10   GPA4   Xếp loại',fill='#607089',font=small_mono); y+=40
for r in rows:
    d.text((455,y),f'{r[0]:<13} {r[1]:<25} {r[2]:<8} {r[3]:<5} {r[4]:<6} {r[5]:<6} {r[6]}',fill='#1f2d3d',font=font); y += 42
d.rounded_rectangle([420,665,990,840],radius=20,fill='#fff7e6',outline='#d0d7de')
d.text((455,690),'Kiểm soát nghiệp vụ',fill='#1f2d3d',font=sans_bold)
for i,t in enumerate(['Điểm chỉ nhận 0..10','Không trùng MSSV','Tự tính tổng 20%-30%-50%']):
    d.text((455,730+i*34),'✓ '+t,fill='#126b34',font=sans)
d.rounded_rectangle([1030,665,1600,840],radius=20,fill='#eaf3ff',outline='#d0d7de')
d.text((1065,690),'Giao diện được xuất từ C++',fill='#1f2d3d',font=sans_bold)
d.text((1065,735),'Module GuiGenerator.cpp đọc dữ liệu trong RAM và sinh file gui/dashboard.html.',fill='#1f2d3d',font=sans_small)
d.text((1065,770),'Có thể mở trực tiếp bằng trình duyệt khi nộp kèm chương trình.',fill='#1f2d3d',font=sans_small)
im.save(FIG/'gui_dashboard.png')

# GUI form mockup
im = Image.new('RGB',(1500,880),'#eef3f8'); d=ImageDraw.Draw(im)
d.rounded_rectangle([60,55,1440,820],radius=30,fill='#ffffff',outline='#d0d7de',width=2)
d.text((110,100),'Form nhập / cập nhật điểm',fill='#1f2d3d',font=sans_title)
labels=['MSSV','Mã môn học','Mã lớp học phần','Điểm quá trình','Điểm giữa kỳ','Điểm cuối kỳ']
values=['202419014','MI3310','MI3310-01','8.5','8.0','9.0']
for i,(l,v) in enumerate(zip(labels,values)):
    x=120 if i<3 else 800; y=180+(i%3)*115
    d.text((x,y),l,fill='#34495e',font=sans_bold)
    d.rounded_rectangle([x,y+35,x+520,y+88],radius=12,fill='#f7f9fc',outline='#d0d7de')
    d.text((x+18,y+49),v,fill='#1f2d3d',font=sans)
d.rounded_rectangle([120,560,520,625],radius=16,fill='#2f80ed')
d.text((245,578),'Lưu điểm',fill='#ffffff',font=sans_bold)
d.rounded_rectangle([560,560,960,625],radius=16,fill='#eaf8ed',outline='#2e7d32')
d.text((595,578),'Điểm tổng tự tính: 8.60',fill='#126b34',font=sans_bold)
d.rounded_rectangle([1000,560,1380,625],radius=16,fill='#fff0f0',outline='#c0392b')
d.text((1030,578),'Chặn điểm ngoài 0..10',fill='#a42222',font=sans_bold)
d.rounded_rectangle([120,680,1380,760],radius=18,fill='#fbfcfe',outline='#d0d7de')
d.text((150,705),'Luồng xử lý: kiểm tra MSSV -> kiểm tra mã môn -> kiểm tra lớp học phần -> kiểm tra điểm -> thêm/cập nhật Grade -> ghi file.',fill='#1f2d3d',font=sans)
im.save(FIG/'gui_grade_form.png')

# GUI report/print mockup
im = Image.new('RGB',(1500,900),'#eef3f8'); d=ImageDraw.Draw(im)
d.rounded_rectangle([70,45,1430,850],radius=28,fill='#ffffff',outline='#d0d7de')
d.text((115,88),'Màn hình báo cáo in danh sách thi',fill='#1f2d3d',font=sans_title)
d.rounded_rectangle([115,160,1385,765],radius=12,fill='#fbfcfe',outline='#d0d7de')
d.text((145,190),'ĐẠI HỌC BÁCH KHOA HÀ NỘI - KHOA TOÁN TIN',fill='#1f2d3d',font=sans_bold)
d.text((145,225),'DANH SÁCH SINH VIÊN ĐỦ ĐIỀU KIỆN DỰ THI / KÝ TÊN',fill='#1f2d3d',font=sans_bold)
d.text((145,260),'Môn học: MI3310 - Kỹ thuật lập trình   |   Lớp HP: MI3310-01   |   GV: TS. Vũ Thành Nam',fill='#34495e',font=sans_small)
headers='STT   MSSV        Họ tên                    Lớp       QT     Tổng    Trạng thái       Ký tên'
d.text((145,315),headers,fill='#607089',font=small_mono)
y=360
for row in [('1','202419014','Dao Van Tuyen','MI2-01','8.5','8.60','Đủ điều kiện'),('2','202418954','Le Nguyen Huu Nghia','MI2-01','8.0','8.40','Đủ điều kiện'),('3','202400104','Hoang Gia Bao','MI2-02','3.5','3.90','Cần xem xét')]:
    d.text((145,y),f'{row[0]:<5} {row[1]:<11} {row[2]:<25} {row[3]:<9} {row[4]:<6} {row[5]:<7} {row[6]:<16} ................',fill='#1f2d3d',font=font)
    y+=46
d.text((900,690),'Hà Nội, ngày .... tháng .... năm 2026',fill='#1f2d3d',font=sans_small)
d.text((865,730),'Cán bộ coi thi        Giảng viên phụ trách        Đại diện lớp',fill='#1f2d3d',font=sans_small)
im.save(FIG/'gui_exam_report.png')

# validation logic flow
im = Image.new('RGB',(1600,760),'#fbfcfe'); d=ImageDraw.Draw(im)
d.text((45,30),'Quy trình kiểm tra dữ liệu khi nhập điểm',fill='#1f2d3d',font=title_font)
steps=[('Nhập MSSV','Từ bàn phím hoặc form'),('Kiểm tra SV','findStudent'),('Kiểm tra môn','findCourse'),('Kiểm tra lớp HP','findSection'),('Kiểm tra điểm','0 <= điểm <= 10'),('Ghi Grade','thêm mới hoặc cập nhật')]
x=45
for i,(t,b) in enumerate(steps):
    rounded_box(d,(x,180,x+235,330),t,b,fill=['#eaf3ff','#eaf8ed','#fff7e6','#f4ecff','#fff0f0','#eaf8ed'][i])
    if i<len(steps)-1: arrow(d,x+235,255,x+285,255)
    x += 275
rounded_box(d,(170,500,1450,650),'Bẫy logic','Nếu một bước kiểm tra thất bại, hàm addOrUpdateGrade trả false và thông báo lỗi. Dữ liệu trong danh sách liên kết và file grades.txt không bị thay đổi.',fill='#ffffff')
im.save(FIG/'grade_validation_flow.png')

print('Generated figures in', FIG)
