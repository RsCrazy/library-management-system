#define _CRT_SECURE_NO_WARNINGS 1
#include "interface.h"
//#include "BookManagement.h"
//void test()
//{
//	BookManagement bma;
//	bma.pushnewtype("С˵");
//	bma.pushnewtype("ɢ��");
//	Manager herui("�����", "123");
//	herui.push(bma, "С˵", { "�����뺣","С˵","������" });
//	herui.push(bma, "ɢ��", { "�����봺ˮ","ɢ��","����" });
//	Users heui;
//	const BookInfor* temp = heui.usersearch(bma, "С˵", "ren���뺣");
//	temp = herui.usersearch(bma, "ɢ��", "�����봺ˮ");
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
	const BookInfor* ret = con.search_by_usr(usr, string("ʷʫ"),string("��������"));

}
int main()
{
	test();
	return 0;
}