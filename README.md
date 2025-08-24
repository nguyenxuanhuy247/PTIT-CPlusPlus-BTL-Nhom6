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

### 3. Nguyễn Anh Khoa

    - Phục trách code phần Tải dữ liệu và danh sách menu
    - Phục trách code phần Ví, giao dịch và Lịch sử giao dịch

## CÁC CHỨC NĂNG CHÍNH

### 1. Đăng ký tài khoản

-   Có 2 cách đăng ký tài khoản :
    -   Cách 1 : Người dùng tự đăng ký bằng cách cung cấp Tên tài khoản, số điện thoại, mật khẩu
    -   Cách 2 : Người quản lý sẽ tạo tài khoản hộ người dùng và cung cấp cho người dùng Tên tài khoản và mật khẩu
-   Khi đăng ký, hệ thống tự động kiểm tra để tránh trùng lặp tên đăng nhập (username) hoặc số điện thoại (phone number), đảm bảo mỗi tài khoản là duy nhất.
-   Có 2 loại tài khoản :
    -   Người dùng thường (User): Người dùng thông thường, chỉ thực hiện các thao tác cá nhân.
    -   Người quản lý (Manager): Quản lý, có thể tạo mới, chỉnh sửa, hoặc quản lý các tài khoản khác.
-   Dữ liệu tài khoản được lưu trữ dưới dạng file JSON giúp dễ dàng thao tác, đồng bộ, và mở rộng.
-   Hệ thống sẽ tự động sao lưu dữ liệu mỗi khi có cập nhật, phòng tránh mất mát thông tin.

### 2. Đăng nhập tài khoản
- Xác minh thông qua tài khoản + chuỗi bảo mật
- Chuỗi bảo mật được hash theo SHA-256 (triển khai trong hash.cpp)
- Với chuỗi bảo mật tự phát sinh → ép buộc đổi mới sau lần đăng nhập đầu
- Tương thích với xác thực OTP (output hiển thị terminal)

### 3. Quản lý ví điểm thưởng và giao dịch

-    Mỗi người dùng có một bộ dữ liệu về số điểm (ví-wallet) duy nhất (id tự sinh) phân biệt với các ví còn lại.
-    Giao dịch chuyển điểm từ ví A → B bao gồm thao tác:
    -   Kiểm tra số dư.
    -   OTP xác thực giao dịch.
    -   Cập nhật điểm đồng thời (atomic).
    -   Ghi log giao dịch (transaction log).    
-    Người dùng có thể xem lịch sử giao dịch cá nhân, lịch sử giao dịch (và trạng thái thực hiện giao dịch) .

### 4. Phân chia người dùng và chức năng

-   Khi người dùng đăng nhập bằng tài khoản của mình, chương trình phải tìm kiếm trong dữ liệu đã lưu trữ và xác định liệu thông tin đăng nhập đó có tồn tại và hợp lệ không. Nếu hợp lệ, cho phép người dùng vào hệ thống và sử dụng các chức năng tương ứng.
-   Nhóm người dùng gồm: người dùng thông thường và người dùng quản lý.
    -   Người dùng thông thường: có quyền cập nhật tên, mật khẩu và chuyển điểm, xem giao dịch; không có quyền tạo tài khoản và thông kê hệ thống.
    -   Người dùng quản lý có tất cả các quyền: tạo, cập nhật tài khoản theo yêu cầu của chủ tài khoản (làm hộ) ; quản lý toàn hệ thống.

Lưu ý: Không được phép thay đổi tên tài khoản đăng nhập.


---

## LUỒNG NGHIỆP VỤ

-   Khi chạy chương trình, giao diện dòng lệnh sẽ xuất hiện với 3 lựa chọn:
    -   1. Đăng nhập
    -   2. Đăng ký
    -   0. Thoát
-   Khi người dùng đăng nhập thành công.
    -   Hệ thống tải dữ liệu mới nhất.
    -   Kiểm tra vai trò: nếu Manager → vào menu quản lý; nếu User thường → vào menu cá nhân.
    -   Người dùng thao tác trong menu cho đến khi chọn đăng xuất.
    -   Khi thoát menu, hệ thống reset vai trò về Guest để bảo mật.
-   Manager đăng nhập thành công → Hiển thị menu quản lý.
    -   Người dùng nhập số lựa chọn.
    -   Hệ thống xử lý: thống kê người dùng; thống kê điểm; thống kê giao dịch; tạo tài khoản; xem ví tổng.
    -   Quay lại menu cho đến khi chọn 0 → đăng xuất.
-   Người dùng đăng nhập thành công → Menu hiển thị liên tục cho đến khi người dùng chọn đăng xuất.
    -   Các chức năng chính:
        -   Xem thông tin cá nhân → chỉ hiển thị dữ liệu.
        -   Thay đổi tên hiển thị → yêu cầu tên mới, cập nhật và lưu.
        -   Đổi mật khẩu → Khi điều chỉnh xong, hệ thống gửi một mã OTP đến chủ tài khoản cùng thông báo các nội dung sẽ thay đổi để chủ tài khoản xác nhận thao tác cập nhật. Nếu mã OTP do chủ tài khoản nhập vào hợp lệ, hệ thống tiến hành cập nhật thông tin của người dùng.
        -   Quản lý ví điểm → mở menu ví cá nhân.
    -   Mọi thay đổi về thông tin cá nhân được lưu ngay bằng DataStore::syncUser.
    -   Khi đăng xuất → lưu dữ liệu lần cuối và thoát.
-    Quản lý hoạt động của ví:
    -    Chỉ Manager (adminUser.isManager()) mới được thao tác với ví tổng.
    -    Manager có thể:
        -    Cấp điểm cho một người dùng cụ thể.
            -    Yêu cầu OTP để xác thực.
            -    Điểm sẽ được trừ từ ví tổng và cộng vào ví user.
            -    Giao dịch được ghi nhận.
        -    Xem tổng số giao dịch của ví tổng.
    -    Người dùng đăng nhập → mở showWalletMenu.
    -    Trong menu, có 2 chức năng chính:
        -    Chuyển điểm: Nhập số điện thoại người nhận → nhập số điểm → xác nhận giao dịch → xác thực OTP → thực hiện chuyển điểm (trừ ví mình, cộng ví người khác, ghi log).
        -    Xem lịch sử giao dịch: Hiển thị toàn bộ các giao dịch liên quan đến ví cá nhân.
-    Giao dịch chuyển điểm
    -    Luồng nghiệp vụ “Chuyển điểm”
        -    User thao tác: Nhập số điện thoại người nhận → Nhập số điểm muốn chuyển → Xác thực OTP.
        -    Hệ thống xử lý:
(1) Tìm User nhận từ DataStore::getAllUsers() dựa theo số điện thoại: Nếu không tìm thấy → báo lỗi “Không tìm thấy người dùng”.
(2) Lấy Wallet của người gửi và người nhận: Nếu một trong hai ví không tồn tại → báo lỗi.
(3) Kiểm tra số dư ví người gửi (deductPoints): Nếu không đủ điểm → báo lỗi “Ví không đủ điểm”.
(4) Thực hiện giao dịch: deductPoints() trừ điểm ví người gửi, addPoints() cộng điểm ví người nhận.
(5) Tạo giao dịch mới → sinh mã định danh duy nhất cảu một giao dịch, ghi nhận thời gian, loại giao dịch, ví gửi, ví nhận, số điểm.
(6) Ghi nhận giao dịch: Cập nhật danh sách giao dịch vào ví người gửi & người nhận, đồng bộ dữ liệu ví và ghi log ra file (lưu lịch sử).
    -    Kết quả cuối cùng:
        -    Điểm được chuyển thành công từ ví A → ví B.
        -    Giao dịch có mã định danh được ghi log trong file để tra cứu sau này.
        -    In ra màn hình: "✅ Chuyển thành công X điểm cho user Y"

*   ---

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
