#ifndef INCLUDED_GAMELIB_CONVERSION_H
#define INCLUDED_GAMELIB_CONVERSION_H


// ���ڿ����� ��ȯ
//char�� �ִ� 4��, short�� 6��, int�� 11��, float�̸� 16��, double�̸� 30����(����)
// �������� ���۸� �ǳ׾� �Ѵ�.
// ��ȯ���� ���� ũ���̴�.���� �ۿ� ���ĳ��� �ʴ��� üũ����.
namespace GameLib {

	int toString10(char* out, char); //���ڰ� �ƴ϶� �ܼ��� �����μ� ����ϴ�
	int toString10(char* out, unsigned char);
	int toString10(char* out, short);
	int toString10(char* out, unsigned short);
	int toString10(char* out, int);
	int toString10(char* out, unsigned);

	int toString16(char* out, char); //���ڰ� �ƴ϶� �ܼ��� �����μ� ����ϴ�
	int toString16(char* out, unsigned char);
	int toString16(char* out, short);
	int toString16(char* out, unsigned short);
	int toString16(char* out, int);
	int toString16(char* out, unsigned);

	int toString(char* out, float, int precision = 6);
	int toString(char* out, double, int precision = 6);

} //namespace GameLib

#endif