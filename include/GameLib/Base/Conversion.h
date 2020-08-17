#ifndef INCLUDED_GAMELIB_CONVERSION_H
#define INCLUDED_GAMELIB_CONVERSION_H


// 문자열로의 변환
//char면 최대 4자, short면 6자, int면 11자, float이면 16자, double이면 30전후(예정)
// 사이즈의 버퍼를 건네야 한다.
// 반환값은 기입 크기이다.범위 밖에 넘쳐나지 않는지 체크하자.
namespace GameLib {

	int toString10(char* out, char); //문자가 아니라 단순한 정수로서 취급하다
	int toString10(char* out, unsigned char);
	int toString10(char* out, short);
	int toString10(char* out, unsigned short);
	int toString10(char* out, int);
	int toString10(char* out, unsigned);

	int toString16(char* out, char); //문자가 아니라 단순한 정수로서 취급하다
	int toString16(char* out, unsigned char);
	int toString16(char* out, short);
	int toString16(char* out, unsigned short);
	int toString16(char* out, int);
	int toString16(char* out, unsigned);

	int toString(char* out, float, int precision = 6);
	int toString(char* out, double, int precision = 6);

} //namespace GameLib

#endif