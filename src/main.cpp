#include "./models/User.h"
#include "./utils/console.h"
#include "./core/auth/login.h"
#include "core/auth/register.h"
#include "./data/sampleData.h"
#include "./data/dataStore.h"
#include "./core/menu/admin_menu.h"
#include "./core/menu/user_menu.h"

using namespace std;

int main()
{
	// Cấu hình để hiển thị Tiếng Việt
	SetConsoleOutputCP(65001);
	// Reset màu hệ thống khi nhấn Ctrl + Z để thoát chương trình
	atexit(resetColor);

	initSampleData();

	int yourChoice = -1;
	User user;

	do
	{
		if (user.getRole() == UserRole::Failed)
		{
			printTitle("CHÀO MỪNG ĐẾN VỚI HỆ THỐNG VÍ ĐIỂM THƯỞNG");
			print("1. Đăng ký", true);
			print("2. Đăng nhập", true);
			print("0. Thoát", true);

			print("Vui lòng nhập lựa chọn của bạn : ", false);
			cin >> yourChoice;
			cin.ignore();

			switch (yourChoice)
			{
			case 1: // Đăng ký
				registerNewUser();
				pause();
				break;
			case 2: // Đăng nhập
				user = handleLogin();
				break;
			case 0:
				print("Tạm biệt!", true);
				break;
			default:
				cout << endl;
				print("Lựa chọn không hợp lệ. Vui lòng kiểm tra lại.", true, ColorEnum::Red);
				break;
			}
		}
		else
		{
			// Đăng nhập thành công
			DataStore::loadAllData();
			if (user.getRole() == UserRole::Manager)
			{
				showManagerMenu(user);
			}
			else
			{
				showUserMenu(user);
			}
			user.setRole(UserRole::Failed); // Đăng xuất sau khi thoát menu
			resetColor();
		}
	} while (yourChoice != 0);

	return 0;
}
