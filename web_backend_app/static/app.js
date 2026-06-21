const state = {
  view: 'dashboard',
  students: [],
  courses: [],
  sections: [],
  grades: [],
  dashboard: null,
  examReport: null,
  editingStudentId: null,
};

const $ = (id) => document.getElementById(id);
const api = async (url, options = {}) => {
  const res = await fetch(url, {
    headers: { 'Content-Type': 'application/json' },
    ...options,
  });
  const data = await res.json();
  if (!res.ok || data.ok === false) throw new Error(data.message || 'Có lỗi xảy ra.');
  return data;
};

function toast(message, type = 'ok') {
  const el = $('toast');
  el.textContent = message;
  el.className = `toast ${type}`;
  setTimeout(() => el.classList.add('hidden'), 3200);
}

function escapeHtml(value) {
  return String(value ?? '').replace(/[&<>'"]/g, c => ({ '&': '&amp;', '<': '&lt;', '>': '&gt;', "'": '&#39;', '"': '&quot;' }[c]));
}

function formData(form) {
  return Object.fromEntries(new FormData(form).entries());
}

function formatScore(value, digits = 2) {
  const n = Number(value ?? 0);
  return Number.isFinite(n) ? n.toFixed(digits) : '0.00';
}

async function loadAll() {
  const [dashboard, students, courses, sections, grades] = await Promise.all([
    api('/api/dashboard'), api('/api/students'), api('/api/courses'), api('/api/sections'), api('/api/grades')
  ]);
  state.dashboard = dashboard.data;
  state.students = students.data;
  state.courses = courses.data;
  state.sections = sections.data;
  state.grades = grades.data;
  renderAll();
}

function renderAll() {
  renderDashboard();
  renderStudents();
  renderCourses();
  renderSections();
  renderGradeOptions();
  renderGrades();
  renderExamOptions();
  if (state.examReport) renderExamReport(state.examReport);
}

function renderDashboard() {
  const d = state.dashboard;
  if (!d) return;
  const cards = [
    ['Sinh viên', d.counts.students],
    ['Môn học', d.counts.courses],
    ['Lớp học phần', d.counts.sections],
    ['Bản ghi điểm', d.counts.grades],
  ];
  $('statsGrid').innerHTML = cards.map(([name, value]) => `<div class="stat-card"><span>${name}</span><strong>${value}</strong></div>`).join('') +
    `<div class="stat-card"><span>Điểm TB hệ thống</span><strong>${d.averageSystem10}</strong></div>`;

  $('topStudents').innerHTML = d.topStudents.map((s, i) => `
    <div class="rank-item">
      <div class="rank-num">${i + 1}</div>
      <div><div class="rank-name">${escapeHtml(s.fullName)}</div><div class="rank-meta">${escapeHtml(s.id)} • ${escapeHtml(s.className)}</div></div>
      <div class="badge">${s.average10}</div>
    </div>`).join('') || '<p>Chưa có dữ liệu.</p>';

  const max = Math.max(...Object.values(d.distribution), 1);
  $('distribution').innerHTML = Object.entries(d.distribution).map(([label, value]) => `
    <div class="bar-row"><span>${label}</span><div class="bar-line"><div class="bar-fill" style="width:${value / max * 100}%"></div></div><strong>${value}</strong></div>
  `).join('');

  $('topGradesBody').innerHTML = d.topGrades.map(g => `
    <tr><td>${escapeHtml(g.studentName)}</td><td>${escapeHtml(g.courseName)}</td><td>${escapeHtml(g.sectionId)}</td><td><span class="badge">${g.total10}</span></td><td>${g.scale4}</td></tr>
  `).join('');
}

function filtered(rows) {
  const q = $('globalSearch').value.trim().toLowerCase();
  if (!q) return rows;
  return rows.filter(row => JSON.stringify(row).toLowerCase().includes(q));
}

function renderStudents() {
  const rows = filtered(state.students);
  $('studentCount').textContent = `${rows.length} sinh viên`;
  $('studentsBody').innerHTML = rows.map(s => `
    <tr>
      <td>${escapeHtml(s.id)}</td><td><strong>${escapeHtml(s.fullName)}</strong></td><td>${escapeHtml(s.className)}</td>
      <td>${s.average10}</td><td>${s.gpa4}</td><td><span class="badge">${escapeHtml(s.classification)}</span></td>
      <td><button class="btn small secondary" onclick="editStudent('${escapeHtml(s.id)}')">Sửa</button> <button class="btn small danger" onclick="deleteStudent('${escapeHtml(s.id)}')">Xóa</button></td>
    </tr>`).join('');
}

function renderCourses() {
  const rows = filtered(state.courses);
  $('courseCount').textContent = `${rows.length} môn học`;
  $('coursesBody').innerHTML = rows.map(c => `<tr><td>${escapeHtml(c.id)}</td><td><strong>${escapeHtml(c.name)}</strong></td><td>${c.credits}</td></tr>`).join('');
  $('sectionCourseSelect').innerHTML = state.courses.map(c => `<option value="${escapeHtml(c.id)}">${escapeHtml(c.id)} - ${escapeHtml(c.name)}</option>`).join('');
}

function renderSections() {
  const rows = filtered(state.sections);
  $('sectionCount').textContent = `${rows.length} lớp học phần`;
  $('sectionsBody').innerHTML = rows.map(s => `<tr><td>${escapeHtml(s.id)}</td><td>${escapeHtml(s.courseId)} - ${escapeHtml(s.courseName)}</td><td>${escapeHtml(s.semester)}</td><td>${escapeHtml(s.academicYear)}</td><td>${escapeHtml(s.lecturer)}</td></tr>`).join('');
}

function renderGradeOptions() {
  $('gradeStudentSelect').innerHTML = state.students.map(s => `<option value="${escapeHtml(s.id)}">${escapeHtml(s.id)} - ${escapeHtml(s.fullName)}</option>`).join('');
  $('gradeCourseSelect').innerHTML = state.courses.map(c => `<option value="${escapeHtml(c.id)}">${escapeHtml(c.id)} - ${escapeHtml(c.name)}</option>`).join('');
  $('gradeSectionSelect').innerHTML = state.sections.map(s => `<option value="${escapeHtml(s.id)}" data-course="${escapeHtml(s.courseId)}">${escapeHtml(s.id)} - ${escapeHtml(s.courseId)}</option>`).join('');
  $('gradeSectionFilter').innerHTML = '<option value="">Tất cả lớp học phần</option>' + state.sections.map(s => `<option value="${escapeHtml(s.id)}">${escapeHtml(s.id)}</option>`).join('');
}

function renderGrades() {
  const section = $('gradeSectionFilter').value;
  let rows = filtered(state.grades);
  if (section) rows = rows.filter(g => g.sectionId === section);
  $('gradeCount').textContent = `${rows.length} bản ghi`;
  $('gradesBody').innerHTML = rows.map(g => `
    <tr>
      <td>${escapeHtml(g.studentId)}</td><td><strong>${escapeHtml(g.studentName)}</strong></td><td>${escapeHtml(g.courseName)}</td><td>${escapeHtml(g.sectionId)}</td>
      <td>${g.processScore}</td><td>${g.midtermScore}</td><td>${g.finalScore}</td><td><span class="badge">${g.total10}</span></td>
      <td><button class="btn small secondary" onclick="fillGrade('${escapeHtml(g.id)}')">Sửa</button> <button class="btn small danger" onclick="deleteGrade('${escapeHtml(g.id)}')">Xóa</button></td>
    </tr>`).join('');
}

function renderExamOptions() {
  const current = $('examSectionSelect')?.value || '';
  $('examSectionSelect').innerHTML = '<option value="">Chọn lớp học phần</option>' + state.sections.map(s =>
    `<option value="${escapeHtml(s.id)}">${escapeHtml(s.id)} - ${escapeHtml(s.courseId)} - ${escapeHtml(s.courseName || '')}</option>`
  ).join('');
  if (current) $('examSectionSelect').value = current;
}

async function loadExamReport() {
  const sectionId = $('examSectionSelect').value;
  if (!sectionId) {
    toast('Bạn cần chọn lớp học phần trước khi tạo báo cáo.', 'err');
    return;
  }
  const res = await api(`/api/exam-report?sectionId=${encodeURIComponent(sectionId)}`);
  state.examReport = res.data;
  renderExamReport(state.examReport);
  toast('Đã tạo báo cáo danh sách thi.');
}

function renderExamReport(report) {
  if (!report) return;
  const section = report.section || {};
  const course = report.course || {};
  $('reportSchool').textContent = report.school || 'TRƯỜNG ĐẠI HỌC BÁCH KHOA HÀ NỘI';
  $('reportFaculty').textContent = report.faculty || 'KHOA TOÁN - TIN';
  $('reportTitle').textContent = report.title || 'DANH SÁCH SINH VIÊN ĐỦ ĐIỀU KIỆN DỰ THI / KÝ TÊN';
  $('reportCourse').textContent = `Môn học: ${section.courseId || '--'} - ${course.name || 'Không rõ'}`;
  $('reportSection').textContent = `Lớp học phần: ${section.id || '--'} | Học kỳ: ${section.semester || '--'} | Năm học: ${section.academicYear || '--'}`;
  $('reportLecturer').textContent = `Giảng viên: ${section.lecturer || '--'}`;
  $('reportTotal').textContent = `Tổng: ${report.summary?.total ?? 0}`;
  $('reportEligible').textContent = `Đủ ĐK: ${report.summary?.eligible ?? 0}`;
  $('reportReview').textContent = `Cần xem xét: ${report.summary?.review ?? 0}`;
  $('examReportBody').innerHTML = (report.rows || []).map(r => `
    <tr>
      <td>${r.index}</td>
      <td>${escapeHtml(r.studentId)}</td>
      <td><strong>${escapeHtml(r.studentName)}</strong></td>
      <td>${escapeHtml(r.className)}</td>
      <td>${formatScore(r.processScore, 1)}</td>
      <td>${formatScore(r.total10, 2)}</td>
      <td><span class="badge ${r.status === 'Cần xem xét' ? 'warn' : 'ok-badge'}">${escapeHtml(r.status)}</span></td>
      <td class="signature-cell">................</td>
    </tr>`).join('') || '<tr><td colspan="8" class="empty-row">Lớp học phần này chưa có sinh viên trong bảng điểm.</td></tr>';
}

function exportExamReportCsv() {
  const report = state.examReport;
  if (!report) {
    toast('Chưa có báo cáo để xuất CSV.', 'err');
    return;
  }
  const header = ['STT','MSSV','Ho ten','Lop','Diem QT','Diem tong','Trang thai','Ky ten'];
  const rows = (report.rows || []).map(r => [r.index, r.studentId, r.studentName, r.className, formatScore(r.processScore, 1), formatScore(r.total10, 2), r.status, '']);
  const csv = [header, ...rows].map(row => row.map(v => `"${String(v ?? '').replaceAll('"','""')}"`).join(',')).join('\n');
  const blob = new Blob(['\ufeff' + csv], { type: 'text/csv;charset=utf-8' });
  const a = document.createElement('a');
  const sec = report.section?.id || 'lop_hoc_phan';
  a.href = URL.createObjectURL(blob);
  a.download = `danh_sach_thi_${sec}.csv`;
  a.click();
  URL.revokeObjectURL(a.href);
}

function setView(view) {
  state.view = view;
  document.querySelectorAll('.view').forEach(el => el.classList.toggle('active', el.id === view));
  document.querySelectorAll('.nav-btn').forEach(el => el.classList.toggle('active', el.dataset.view === view));
  const titles = { dashboard: 'Tổng quan', students: 'Sinh viên', courses: 'Môn học', sections: 'Lớp học phần', grades: 'Bảng điểm', examReport: 'Báo cáo danh sách thi' };
  $('pageTitle').textContent = titles[view] || 'Student Manager';
}

document.querySelectorAll('.nav-btn').forEach(btn => btn.addEventListener('click', () => setView(btn.dataset.view)));
$('globalSearch').addEventListener('input', renderAll);
$('refreshBtn').addEventListener('click', async () => { await loadAll(); toast('Đã làm mới dữ liệu.'); });
$('themeBtn').addEventListener('click', () => document.body.classList.toggle('dark'));
$('gradeSectionFilter').addEventListener('change', renderGrades);
$('resetStudentForm').addEventListener('click', () => { state.editingStudentId = null; $('studentForm').reset(); $('studentForm').elements.id.disabled = false; });
$('loadExamReport').addEventListener('click', () => loadExamReport().catch(err => toast(err.message, 'err')));
$('printExamReport').addEventListener('click', () => window.print());
$('exportExamReport').addEventListener('click', exportExamReportCsv);
$('examSectionSelect').addEventListener('change', () => { state.examReport = null; });

$('studentForm').addEventListener('submit', async (e) => {
  e.preventDefault();
  const data = formData(e.target);
  try {
    if (state.editingStudentId) {
      await api(`/api/students/${encodeURIComponent(state.editingStudentId)}`, { method: 'PUT', body: JSON.stringify(data) });
      toast('Đã cập nhật sinh viên.');
    } else {
      await api('/api/students', { method: 'POST', body: JSON.stringify(data) });
      toast('Đã thêm sinh viên.');
    }
    state.editingStudentId = null;
    e.target.reset();
    e.target.elements.id.disabled = false;
    await loadAll();
  } catch (err) { toast(err.message, 'err'); }
});

$('courseForm').addEventListener('submit', async (e) => {
  e.preventDefault();
  try { await api('/api/courses', { method: 'POST', body: JSON.stringify(formData(e.target)) }); e.target.reset(); await loadAll(); toast('Đã thêm môn học.'); }
  catch (err) { toast(err.message, 'err'); }
});

$('sectionForm').addEventListener('submit', async (e) => {
  e.preventDefault();
  try { await api('/api/sections', { method: 'POST', body: JSON.stringify(formData(e.target)) }); e.target.reset(); await loadAll(); toast('Đã thêm lớp học phần.'); }
  catch (err) { toast(err.message, 'err'); }
});

$('gradeForm').addEventListener('submit', async (e) => {
  e.preventDefault();
  try { await api('/api/grades', { method: 'POST', body: JSON.stringify(formData(e.target)) }); e.target.reset(); await loadAll(); toast('Đã lưu điểm.'); }
  catch (err) { toast(err.message, 'err'); }
});

window.editStudent = (id) => {
  const s = state.students.find(x => x.id === id);
  if (!s) return;
  const form = $('studentForm');
  state.editingStudentId = id;
  Object.entries(s).forEach(([k, v]) => { if (form.elements[k]) form.elements[k].value = v; });
  form.elements.id.value = id;
  form.elements.id.disabled = true;
  setView('students');
};

window.deleteStudent = async (id) => {
  if (!confirm('Xóa sinh viên này? Các điểm liên quan cũng sẽ bị xóa.')) return;
  try { await api(`/api/students/${encodeURIComponent(id)}`, { method: 'DELETE' }); await loadAll(); toast('Đã xóa sinh viên.'); }
  catch (err) { toast(err.message, 'err'); }
};

window.fillGrade = (id) => {
  const g = state.grades.find(x => x.id === id);
  if (!g) return;
  const form = $('gradeForm');
  form.elements.studentId.value = g.studentId;
  form.elements.courseId.value = g.courseId;
  form.elements.sectionId.value = g.sectionId;
  form.elements.processScore.value = g.processScore;
  form.elements.midtermScore.value = g.midtermScore;
  form.elements.finalScore.value = g.finalScore;
  setView('grades');
};

window.deleteGrade = async (id) => {
  if (!confirm('Xóa bản ghi điểm này?')) return;
  try { await api(`/api/grades/${encodeURIComponent(id)}`, { method: 'DELETE' }); await loadAll(); toast('Đã xóa điểm.'); }
  catch (err) { toast(err.message, 'err'); }
};

$('exportGrades').addEventListener('click', () => {
  const rows = state.grades;
  const header = ['studentId','studentName','courseId','courseName','sectionId','processScore','midtermScore','finalScore','total10'];
  const csv = [header.join(',')].concat(rows.map(r => header.map(h => `"${String(r[h] ?? '').replaceAll('"','""')}"`).join(','))).join('\n');
  const blob = new Blob(['\ufeff' + csv], { type: 'text/csv;charset=utf-8' });
  const a = document.createElement('a');
  a.href = URL.createObjectURL(blob);
  a.download = 'bang_diem.csv';
  a.click();
  URL.revokeObjectURL(a.href);
});

loadAll().catch(err => toast(err.message, 'err'));
