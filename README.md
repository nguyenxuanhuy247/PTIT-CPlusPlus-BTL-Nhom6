# REWARD WALLET SYSTEM - HỆ THỐNG QUẢN LÝ VÍ ĐIỂM THƯỞNG

## GIỚI THIỆU DỰ ÁN

Hệ thống ví điểm thưởng là một ứng dụng dùng để quản lý điểm được dùng để quy đổi ra hàng hóa và dịch vụ, giữa các ví có thể chuyển điểm cho nhau.

## GIỚI THIỆU THÀNH VIÊN VÀ PHÂN CÔNG CÔNG VIỆC

### 1. Nguyễn Xuân Huy (Trường nhóm)

    - Khởi tạo và dựng cấu trúc dự án
    - Phân công công việc, giám sát tiến độ
    - Viết tài liệu hướng dẫn cài đặt 
    - Phụ trách code phần đăng nhập

### 2. Đức Linh

    - Phục trách code phần Đăng ký

### 3. Khắc Ngọc

    - Phục trách code phần Ví, giao dịch và Lịch sử giao dịch

### 4. Nguyễn Anh Khoa

-   Phục trách code phần Tải dữ liệu và danh sách menu

## CÁC CHỨC NĂNG CHÍNH

### 1. Đăng ký tài khoản
Hệ thống hỗ trợ người dùng tự đăng ký tài khoản hoặc cho phép tài khoản quản lý tạo hộ tài khoản cho nhân viên, đối tác.
Khi đăng ký, hệ thống tự động kiểm tra để tránh trùng lặp tên đăng nhập (username) hoặc số điện thoại (phone number), đảm bảo mỗi tài khoản là duy nhất.
Nếu người được sinh mật khẩu tự động
Phân biệt hai loại tài khoản:

User: Người dùng thông thường, chỉ thực hiện các thao tác cá nhân.

Manager: Quản lý, có thể tạo mới, chỉnh sửa, hoặc quản lý các tài khoản khác.
Dữ liệu tài khoản được lưu trữ dưới dạng file JSON giúp dễ dàng thao tác, đồng bộ, và mở rộng.
Hệ thống sẽ tự động sao lưu dữ liệu mỗi khi có cập nhật, phòng tránh mất mát thông tin.

### 2. Đăng nhập tài khoản

### 3. Quản lý ví điểm thưởng và giao dịch

### 4. Phân chia người dùng và chức năng

---

## HƯỚNG DẪN CÀI ĐẶT

1. Tải CMake : Vào trang https://cmake.org/download/ > Binary distributions > Windows x64 Installer
2. Tải g++ : https://github.com/niXman/mingw-builds-binaries/releases > i686-15.1.0-release-mcf-dwarf-ucrt-rt_v12-rev0.7z > Giải nén > Copy mingw64 vào ổ C
3. Mở Windows > System Properties > Environment Variables... > System variables > Bấm nút New > Variable name : PATH, Variable value : "C:\mingw64\bin" > Nhấn OK
4. Mở VSCode > Nhấn Ctrl + Shift + P > CMake : Edit User-Local CMake Kits > cmake-tools-kits.json > Copy nội dung sau vào file :
 <pre> 
 [
    {
       "name": "GCC 4.9.2 x86_64-w64-mingw32",
       "compilers": {
          "C": "C:/mingw64/bin/gcc.exe",
          "CXX": "C:/mingw64/bin/g++.exe"
       },
       "isTrusted": true,
       "environmentVariables": {
          "CMT_MINGW_PATH": "C:/mingw64/bin"
       },
       "preferredGenerator": {
          "name": "MinGW Makefiles"
       }
    }
 ]
  </pre>

## HƯỚNG DẪN SỬ DỤNG

-   Mỗi lần thay đổi code cần phải build lại ứng dụng
-   Nhấn tổ hợp phím Ctrl + Shift + P, sau đó chọn CMake: Build để tiến hành build lại ứng dụng
-   Trong thư mục build có xuất hiện file reward_wallet.exe là đã build thành công
-   Nhấn Ctrl + ` để mở Terminal của VSCode, sau đó cd ./build vào thư mục build
-   Chạy file reward_wallet.exe để chạy chương trình

Lưu ý : Khi tiến hành build lại, phải xóa thư mục build đi, nếu bị lỗi không xóa được là do ứng dụng vẫn đang chạy file reward_wallet.exe, ta tiến hành như sau:

-   Cách 1 : Hoặc Mở Tab Terminal, nhấn Ctrl + C để thoát lệnh
-   Cách 2 :
    -   Nhấn tổ hợp phím Ctrl + Alt + Delete
    -   Mở Tab Details, chọn reward_wallet.exe, sau đó nhân End task để đóng tiến trình
