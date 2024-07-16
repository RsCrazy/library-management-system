#define _CRT_SECURE_NO_WARNINGS 1
#include "interface.h"
//#include "BookManagement.h"
//void test()
//{
//	BookManagement bma;
//	bma.pushnewtype("小说");
//	bma.pushnewtype("散文");
//	Manager herui("何锐杰", "123");
//	herui.push(bma, "小说", { "老人与海","小说","海明威" });
//	herui.push(bma, "散文", { "繁星与春水","散文","冰心" });
//	Users heui;
//	const BookInfor* temp = heui.usersearch(bma, "小说", "ren人与海");
//	temp = herui.usersearch(bma, "散文", "繁星与春水");
//
//}
//
//int main()
//{
//	
//	return 0;
//}
void test()
{
	Connection con;
	con.InitBookManagement("D:\\bit class\\test.c\\BookManagement\\book.txt");
	Users& usr = con.InitUser("heruijie", "123");
	const BookInfor* ret = con.search_by_usr(usr, string("史诗"),string("《神曲》"));

}
int main()
{
	test();
	return 0;
}