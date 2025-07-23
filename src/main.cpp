#include "./models/User.h"
#include "./utils/console.h"
#include "./core/auth/login.h"
#include "core/auth/register.h"
#include "../data/seedData.h"

using namespace std;

int main()
{

	int choice = -1;
	User user;

	do
	{
		if (user.getRole() == UserRole::Failed)
		{
			printTitle("CHAO MUNG BAN DEN VOI HE THONG VI DIEM THUONG:");

			print("Moi ban lua chon :", true);
			print("1. Dang ky", true);
			print("2. Dang nhap", true);
			print("3. Khoi tao du lieu mau", true);
			print("0. Quay lai", true);

			string choiceStr = input("Lua chon cua ban la: ");
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
			case 1: // Đăng ký
				registerNewUser();
				pause();
				break;
			case 2: // Đăng nhập
				user = handleLogin();
				if (user.getRole() == UserRole::Failed)
					pause();
				break;
			case 3:
				insertSeedData();
				pause();
				break;
			case 0:
				print("Tam biet!", true);
				break;
			default:
				print("Vui long chon lai", true);
				break;
			}
		}
		else
		{
		}
	} while (choice != 0);

	return 0;
}
