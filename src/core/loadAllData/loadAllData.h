#ifndef LOADDATA_H
#define LOADDATA_H

#include "../models/User.h"     // Dinh nghia struct User và enum UserRole
#include "../menu/admin_menu.h" // Ham showManagerMenu
#include "../menu/user_menu.h"  // Ham showUserMenu

void handleLoggedInUser(struct User *user); // Sau khi thoát khỏi menu (dù là manager hay user), vai trò của user được đặt lại thành Failed.
                                            // Điều này giống như tự động đăng xuất để quay về menu chính.
#endif
