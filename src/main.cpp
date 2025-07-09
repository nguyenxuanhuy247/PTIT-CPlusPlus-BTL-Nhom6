#include "./models/User.h"
#include "./utils/console.h"
#include "./core/auth/login.h"

using namespace std;

int main()
{
	int choice = -1;
	User user;
	do
	{
		if (user.getRole() == UserRole::Failed)
		{
			printTitle("CHÀO MỪNG BẠN ĐẾN VỚI HỆ THỐNG VÍ ĐIỂM THƯỞNG");

			print("Moi ban lua chon :", true);
			print("1. Dang nhap", true);
			print("2. Dang ky", true);
			print("3. Them du lieu", true);
			print("0. Quay lai", true);

			string choiceStr = input("Lựa chọn của ban là: ");
			try
			{
				choice = stoi(choiceStr);
			}
			catch (...)
			{
				print("Vui long chon dung so!", true);
				continue;
			}

			switch (choice)
			{
			case 1: // Đăng nhập
				user = handleLogin();
				if (user.getRole() == UserRole::Failed)
					pause();
				break;
			case 2: // Đăng ký
				break;
			case 3:
				break;
			case 0:
				print("Goodbye!", true);
				break;
			default:
				print("Wrong choice.", true);
				break;
			}
		}
		else
		{
			// Đã đăng nhập thành công
		}
	} while (choice != 0);

	return 0;
}
