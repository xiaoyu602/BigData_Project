#pragma once
#include <iostream>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

using namespace std;

#define UN_INIT 0xcccccccccccccccc
#define MAX_NUM 0x7FFFFFFFFFFFFFFF
#define MIN_NUM 0x8000000000000000

typedef long long INT64;


class BigData
{
	friend ostream& operator<<(ostream& os,BigData& bigData);
public:
	BigData(INT64 value = UN_INIT)
		:_value(value)
	{}
	BigData(char* pData);
	BigData operator+(const BigData& bigData);
	BigData operator-(const BigData& bigData);
	BigData operator*(const BigData& bigData);
	BigData operator/(const BigData& bigData);
public:
	bool IsINT64Overflow() const;   //�Ƿ����
	void INT64toString();           //����ת��Ϊ�ַ���
	bool IsLeftBig(char* pLeft,int LSize,char* pRight,int RSize); //pLeft>pRight
	int SubLoop(char* &pLeft,int &LSize,char* pRight,int RSize); //��
	string ADD(string left,string right);
	string SUB(string left,string right);
	string MUL(string left,string right);
	string DIV(string left,string right);

private:
	INT64 _value;
	string _strData;
};


 //"123" "+111" "-222" "1vj2" "+anb123" "0000123" 
BigData::BigData(char* pData)
{
	assert(pData);
	char Symbol = 0; //����λ
	if(pData[0] >= '0' && pData[0] <= '9')
	{
		Symbol = '+';
	}
	else if(pData[0] == '+' || pData[0] == '-')
	{
		Symbol = pData[0];
		pData++;
	}
	else
	{
		return;
	}
	while(*pData == 0)
	{
		pData++;
	}
	_strData.resize(strlen(pData)+1);// �࿪��һ���ռ�����λ
	_strData[0] = Symbol;
	_value = 0;
	int index = 1;
	while(*pData >= '0' && *pData <= '9')
	{
		_strData[index++] = *pData;
		_value = _value * 10 + *pData - '0';
		pData++;
	}
	if(_strData[0] == '-')
	{
		_value = 0 - _value;
	}
	if(isalpha(*pData))  //���Ϊ�ַ�
	{
		return;
	}
	_strData.resize(index);//���µ����ռ䣬��ֹ�ռ��˷�
}

bool BigData::IsINT64Overflow() const  //�Ƿ����
{
	string tmp("+9223372036854775807");
	if(_strData[0] == '-')
	{
		tmp = "-9223372036854775807";
	}
	if(_strData.size() < tmp.size())  //�����
	{
		return false;
	}
	else if(_strData.size() == tmp.size())  //�����
	{
		char* c_strData = (char*)_strData.c_str();
		char* ctmp = (char*)tmp.c_str();
		int ret = strcmp(c_strData,ctmp);
		if(ret <= 0)
		{
			return false;
		}
	}
	return true;
}

void BigData::INT64toString()//����ת��Ϊ�ַ���
{
	char Symbol = '+';
	if(_value < 0)
	{
		Symbol = '-';
	}
	if(_value != UN_INIT && _value >= -9223372036854775807 && _value <= 9223372036854775807)
	{
		INT64 num = _value;
		char ret = 0;
		int index = 1;
		//itoa(_value,ret,10); //������ת��Ϊ�ַ���string
		_strData.append(1,Symbol);//���Ƿ���λ
		while(num)
		{
			if(num < 0)//��ֵΪ��
			{
				num = 0 - num;
			}
			ret = num % 10 + '0';//ȡ����
			_strData.append(1,ret);
			num = num/10;
		}
		//_strData�е��ַ�����Ҫ����
		char* left = (char*)_strData.c_str()+1;
		char* right = left+_strData.size()-2;   
		while(left < right)
		{
			char tmp = *left;
			*left = *right;
			*right = tmp;
			left++;
			right--;
		}
		
	}
}

bool BigData::IsLeftBig(char* pLeft,int LSize,char* pRight,int RSize) //���
{
	if((LSize < RSize)|| ((LSize == RSize) && (strncmp(pLeft,pRight,LSize) < 0)))
	{
		return false;
	}
	return true;
}

int BigData::SubLoop(char* &pLeft,int &LSize,char* pRight,int RSize) //���ؼ��Ĵ�������
{
	assert(pLeft);
	assert(pRight);
	int count  = 0; //�����Ĵ���
	int cStep = 0;//��λ
	while(1)
	{
		if(!IsLeftBig(pLeft,LSize,pRight,RSize))  //pLeft < pRight
		{
			break;
		}
		for(int i=0;i<LSize;++i)
		{
			if(i < RSize)
			{
				int ret = pLeft[LSize-i-1] - '0' - (pRight[RSize-i-1] - '0');
				if(ret < 0)
				{
					pLeft[LSize-i-2] -= 1;   //��λ
					ret = pLeft[LSize-i-1] - '0' + 10 - (pRight[RSize-i-1] - '0');
				}
				pLeft[LSize-i-1] = ret + '0';
			}	
		}
		count++;
		//ȥ��ǰ��0   
		while(*pLeft == '0' && LSize > 0)
		{
			pLeft++;
			LSize--;
		}
	}
	return count;
}
BigData BigData::operator +(const BigData &bigData)
{
	//���������������������
	if(!IsINT64Overflow() && !bigData.IsINT64Overflow())
	{
		if(_strData[0] != bigData._strData[0]) //���
		{
			return BigData(_value+bigData._value);//��Ӳ��ᷢ�����
		}
		else    //ͬ��
		{
			if((_strData[0] == '+' && (MAX_NUM - _value >= bigData._value) && (_value+bigData._value >= 0))
				||(_strData[0] == '-' && (MIN_NUM + _value <= bigData._value)&&(_value+bigData._value <= 0)))
			{
				return BigData(_value+bigData._value);
			}
		}
	}

	//һ���������������
		if(_strData[0] == bigData._strData[0])//ͬ��
		{
		/*	string ret = ADD(_strData,bigData._strData);
			return BigData((char*)ret.c_str());*/

			return BigData((char*)ADD(_strData,bigData._strData).c_str());
		}
		else//���
		{
			return BigData((char*)SUB(_strData,bigData._strData).c_str());
		}
}

BigData BigData::operator-(const BigData& bigData)
{
	//�����������������������
	if(!IsINT64Overflow() && !bigData.IsINT64Overflow())
	{
		if(_strData[0] == bigData._strData[0])  //ͬ����������
		{
			return BigData(_value - bigData._value);
		}
		else         //������   10  3  -8      -10  -3  8
		{
			// ((_value > 0 && MIN_INT64 + _value <= bigData._value) ||       
                    // (_value < 0 && MAX_INT64 + _value >= bigData._value))
			if((_value > 0 && MAX_NUM - _value >= -bigData._value) || (_value < 0 && MIN_NUM + bigData._value <= _value))
			{
				return BigData(_value - bigData._value);
			}
		}
	}
	
	//��һ���������������  
	//"+11111111111111111111111111111"   "-2222222"
	if(_strData[0] == bigData._strData[0]) //ͬ��---->��
	{
		return BigData((char*)SUB(_strData,bigData._strData).c_str());
	}
	else                                   //���---->��
	{
		return BigData((char*)ADD(_strData,bigData._strData).c_str());
	}
}

BigData BigData::operator*(const BigData& bigData)
{
	if(_value == 0 || bigData._value == 0)
	{
		return BigData((INT64)0); //ת��Ϊlong long��
	}
	if(!IsINT64Overflow() && !bigData.IsINT64Overflow())
	{
		// ͬ�� ��+����-�� <= ���ֵ---->�����
		if(_strData[0] == bigData._strData[0])
		{
			if(MAX_NUM/_value >= bigData._value)
			{
				return BigData(_value*bigData._value);
			}
		}
		else   //���  >= ��Сֵ
		{
			if(MIN_NUM/_value <= bigData._value)
			{
				return BigData(_value*bigData._value);
			}
		}	
	}//��һ���������������
	else
	{
		return BigData((char*)MUL(_strData,bigData._strData).c_str());
	}
}

BigData BigData::operator/(const BigData& bigData)
{
	if(bigData._value == 0)//����Ϊ0
	{
		assert(false);
		//return BigData((INT64)0);
	}
	//��������û�����
	if(!IsINT64Overflow() && !bigData.IsINT64Overflow())
	{
		if(_value == 0) //������Ϊ0
		{
			return BigData((INT64)0);
		}
		return BigData(_value/bigData._value);
	}
	
	//��һ�����
	//�������
	//left < right   2
	char* LStr = (char*)_strData.c_str();
	char* RStr = (char*)bigData._strData.c_str();
	if(_strData.size() < bigData._strData.size() 
		|| (_strData.size() == bigData._strData.size() && (strcmp(LStr,RStr) < 0)))
	{
		return BigData((INT64)0);
	}
	//��ֵ�������  ͬ�� ���
	if(strcmp(LStr+1,RStr+1) == 0)
	{
		if(_strData[0] == bigData._strData[0])
		{
			return BigData((INT64)1);
		}
		else
		{
			return BigData((INT64)-1);
		}
	}
	//right == +-1  �����
	if(!bigData.IsINT64Overflow() && (bigData._value == 1 || bigData._value == -1))
	{
		if(bigData._value == 1)
		{
			return *this;
		}
		else
		{
			if(_strData[0] == '+')
			{
				_strData[0] = '-';
			}
			else
			{
				_strData[0] = '+';
			}
		}
		return *this;
	}
	
	return BigData((char*)DIV(_strData,bigData._strData).c_str());

}

string BigData::ADD(string left,string right)
{ 
	int LSize = left.size();  //"+1111111111111111"   17
	int RSize = right.size();  //        "+9999999"    8
	string pRet;
	char Symbol = left[0];	
	while(LSize < RSize)//����left���
	{
		swap(left,right);
		swap(LSize,RSize);
	}
	int cStep = 0;
	size_t index = 0;
	pRet.resize(LSize+1);  //�࿪��һ���������յĽ�λ
	for(index=1;index<LSize;++index)
	{
		int num = left[LSize-index] - '0'+ cStep ;
		cStep = 0;
		if(index < RSize)
		{
			num = num + right[RSize-index] - '0';
			cStep = num/10;
		}
		pRet[LSize-index+1] = num%10 + '0';
		
	}
	pRet[0] = Symbol;
	pRet[1] = cStep + '0';
	return pRet;
}

string BigData::SUB(string left,string right)
{   //"+2877777773113131"  "-298364222"
	//"-4235436578690864"  "+232435363"
	int LSize = left.size();   
	int RSize = right.size();
	char Symbol = left[0];//����λ
	char* c_left = (char*)left.c_str();
	char* c_right = (char*)right.c_str();
	int ret = strcmp(c_left,c_right);
	if(LSize < RSize  || (LSize == RSize && ret < 0))  //����left���
	{
		swap(left,right);
		swap(LSize,RSize);
		if(Symbol == '+')   //������ "2"  "14444445545759909876321452"
 		{
			Symbol = '-';
		}
		else
		{
			Symbol = '+';
		}
	}
	string pRet;
	pRet.resize(LSize);
	int index = 1;
	int cStep = 0;
	for(index=1;index<LSize;++index)
	{
		int num = left[LSize-index] - '0'- cStep;
		cStep = 0;
		if(num < 0)
		{
			num = 10 + num;
			cStep = 1; //��λ
		}
		if(index < RSize)
		{
			if(num - (right[RSize-index]- '0') < 0) //num == 0  0-1
			{
				num = 10 + num;
				cStep = 1;
			}
			num = num - (right[RSize-index] - '0');
		}
		pRet[LSize-index] = num + '0';
	}
	pRet[0] = Symbol;
	return pRet;
}


string BigData::MUL(string left,string right)
{
	int LSize = left.size();
	int RSize = right.size();
	char Symbol = 0;
	if(left[0] == right[0])
	{
		Symbol = '+';
	}
	else
	{
		Symbol = '-';
	}
	if(LSize < RSize)   //RSize-->С
	{
		swap(left,right);
		swap(LSize,RSize);
	}
	string pRet;
	int cStep = 0;
	pRet.resize(LSize+RSize-1,'0'); 
	int len = pRet.size();
	int count = 0;//��λ
	for(int i=1;i<RSize;++i)
	{
		int src = right[RSize-i]- '0'; 
		int cStep = 0;//��λ
		if(src == 0)
		{
			count++;
			continue;
		}
		for(int j=1;j<LSize;++j)
		{
			int dst = src*(left[LSize-j]-'0') + cStep;
			dst = dst + (pRet[len-j-count] - '0');
			pRet[len-j-count] = dst%10 + '0';
			cStep = dst/10;//��λ	
		}
		pRet[len-LSize-count] += cStep ;  //ÿ�ε����һ����λ
		count++;
	}
	pRet[0] = Symbol;
	return pRet;
}

string BigData::DIV(string left,string right)
{
	char Symbol = left[0];  //����λ
	int LSize = left.size()-1;  //ȥ������λ�ĳ���
	int RSize = right.size()-1;
	int len = RSize;  //�����������ͬ 
	char* pLeft = (char*)left.c_str()+1;
	char* pRight = (char*)right.c_str()+1;
	if(left[0] == right[0])
	{
		Symbol = '+';
	}
	else
	{
		Symbol = '-';
	}
	string pRet;
	pRet.append(1,Symbol); //�������λ
	for(int index=RSize-1;index < LSize;)
	{
		if(!IsLeftBig(pLeft,len,pRight,RSize))
		{
			pRet.append(1,'0');    //С����Ϊ0
			len++;
			index++;  //�տ�ʼ�����±���ָԪ�صĸ�����len���
			if(len + index -1 > LSize)
			{
				break;
			}
			if(*pLeft == '0')   // "94000000000"   "94"         "00 0000000"
			{
				pLeft++;
				len--;
				continue;
			}
			continue;
		}
		else
		{
			pRet.append(1,SubLoop(pLeft,len,pRight,RSize)+ '0');
		}
		len++;//��SubLoop�����г�������pLeft<pRight   ����  *pLeft-->0  len����Ϊ0���߱�С
		index++;
	}
	return pRet;
}