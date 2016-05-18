#include "BigData.h"


ostream& operator<<(ostream& os,BigData& bigData)
{
	if(!bigData.IsINT64Overflow())
	{
		os<<bigData._value;
	}
	else
	{
		char* pData = (char*)bigData._strData.c_str();
		if(bigData._strData[0] == '+')
		{
			pData++;
		}
		while(*pData == '0')
		{
			pData++;
		}
		os<<pData;
	}
	return os;
}
void Test1()
{
	//BigData b("11111111111111111111");
	//BigData b1(111111111111111);
	//b1.INT64toString();

	/*BigData left("9223372036854775807");
	BigData right("12");*/

	//BigData left("-9223372036854775807");
	//BigData right("-12");

	/*BigData left(9223372036854775807);
	BigData right(12);
	left.INT64toString();
	right.INT64toString();*/

	BigData left("1111111111111111111111111111111111111111111");
	BigData right("999999999999999");


	cout<<left+right<<endl;
	
}

void Test2()
{
	//BigData left("1000000000000000000000000000002");
	//BigData right("12");

	/*BigData left("111111111111");
	BigData right("222");*/

	/*BigData left("-111111111111");
	BigData right("222");*/

	//BigData left("222");
	//BigData right("-111111111111");

	/*BigData left("-222");
	BigData right("-111111111111");*/

	//BigData left("9223372036854775808");
	//BigData right("12");

	BigData left("-9223372036854775807");
	BigData right("2");

	//BigData left("2");
	//BigData right("9223372036854775808");


	//BigData left("-2");
	//BigData right("9223372036854775808");

	//BigData left("-2");
	//BigData right("9223372036854775808");

	cout<<left-right<<endl;
}


void Test3()
{
	//两个数都不溢出，相乘无溢出
	/*BigData left("132425444");
	BigData right("2442");*/

	/*BigData left("-132425444");
	BigData right("2442");*/

	/*BigData left("132425444326457694336");
	BigData right("2442");*/


	/*BigData left("111111111111111111111111111");
	BigData right("42");*/

	BigData left("-11111111111000000000111111111");
	BigData right("42");
	cout<<left*right<<endl;
}


void Test4()
{
	//两个数都不溢出
	/*BigData left("222222222222");
	BigData right("2222");*/

	/*BigData left("0");
	BigData right("2222");*/

	/*BigData left("2222");
	BigData right("-1");*/

	//BigData left("1");
	//BigData right("2222");

	/*BigData left("2222");
	BigData right("0");*/

	//有一个数溢出
	/*BigData left("0");
	BigData right("1234569864354364547568565764325");*/

	/*BigData left("-33333333333333333333");
	BigData right("121");*/

	//BigData left("22222222222222222222222222222222");
	//BigData right("1");
	//BigData right("-1");

	BigData left("2222222222222222222222222");
	BigData right("333");

	//BigData left("330000000000000000000000000000000");
	//BigData right("33");

	//BigData left("-90222222200002222222222");
	//BigData right("33");

	cout<<left/right<<endl;
}
//int main()
//{
//	/*Test1();*/
//	Test2();
//	//Test3();
//	//Test4();
//	return 0;
//}