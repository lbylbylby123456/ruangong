//#pragma once

//�ĸ�����
//�������� 
#include <iostream>

//#include <typeinfo>

#include <string>
#include <typeindex>
//��������������ǳ�virtual 

//using namespace std;

using std::cout;

//is a���̳�ģ�� 
class value_holder 
{

public:

	virtual const std::type_index & type() const= 0;//���󣨹�ͬ�����ܽ���� 
//  ������type_index 
// noexcept ������� 
//��������/�ڴ������ͷš����������쳣�� 
	virtual value_holder * copy() const= 0;
	
	value_holder(const value_holder &a){}
	
	value_holder()=default;//Ĭ�Ϲ��캯��default 
	
	virtual value_holder&operator=(value_holder const &a){} //��ֵ���� 
	
	virtual ~value_holder(){}//�������� 
	




};

template<typename T>
//oopȥ��ģ����
//ʹ�ü̳�ʵ�ֲ��� 
//effective modern C++
class holder : public value_holder 
{

public:
	
	T data;
	
    /*T getdata()
    {
    	return this->data;
	}*/

	holder(const T &a):data(a){}


	virtual const std::type_index & type() const
	{

		return typeid(T);

	}

	virtual value_holder *copy() const override  //��д�����麯��Ҫдoverride 
	{

		return new holder(data);

	}//��Ҫ�հ�stl 

};

class any final 
{
//��������������� Ĭ�Ϲ��캯�� ���ƹ��캯�� ��ֵ���� �Զ��幹�캯�� �����ƶ����� 

//private:
//value_holder * value = nullptr;

public:

    value_holder * value = nullptr;

	template<typename T>
	any(const T &a):value(new holder<T>(a)){}
	
	any()=default; 
    
	~any()
    //���ᱻ�̳еģ����ü�virtual��д�����������ʧ 
	{
		delete value; 
	}

     
	any &operator=(any const &a) //��ֵ���� 
	{

		if(this!=&a)//����Ը�ֵ		
		delete value;
        
        //��ֹ�� �ж�ָ������� 
        
		value ? a.value->copy() : nullptr;

		return *this;

	}

	const std::type_index & type() 
	{

		return value ? value->type() : typeid(void);

	}
 

	any reset() 
	{
		delete value;
		value = nullptr;
		return *this;
	}

	any swap(any &obj) 
	{

		std::swap(value, obj.value);

		return *this;

	}

	bool has_value() 
	{

		return value != nullptr;

	}

};

template<typename T>

any make_any(T &value) 
{

	return any(value);

}

class bad_any_cast :public std::bad_cast
{

public:

	virtual const char *what() {

		return "bad cast happened!";

	}

};

template<typename T>

T any_cast(any &a) 
{
	if (a.type() != typeid(T))
		throw bad_any_cast();
	return dynamic_cast<holder<T>*>(a.value)->data;
}

int main()

{
	any a = 1;
	cout << a.type().name() << "\n";
	a = 3.14;
	cout << a.type().name() << "\n";
	if (a.has_value())
		cout << "has value!\n";
	a.reset();
	if (!a.has_value())
		cout << "no value!\n";
	a = 2.5;
	try
	{
		double b = any_cast<double>(a);
		cout << b << "\n"; 
	}
	catch (bad_any_cast &e)

	{

		cout << e.what() << "\n";

	}

}
