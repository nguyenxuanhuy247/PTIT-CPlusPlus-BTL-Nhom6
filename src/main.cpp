#include <iostream>
#include "../models/User.h"
#include "../include/ui/console.h"

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

			print("Vui lòng lựa chọn :", true);
			print("1. Đăng nhập", true);
			print("2. Đăng ký", true);
			print("3. Thêm dữ liệu", true);
			print("0. Thoát", true);

			string choiceStr = input("Lựa chọn của ban là: ");
			try
			{
				choice = stoi(choiceStr);
			}
			catch (...)
			{
				print("Vui lòng chọn đúng só!", true);
				continue;
			}
		}
		else
		{
			// Đã đăng nhập thành công
		}
	} while (choice != 0);

	return 0;
}
