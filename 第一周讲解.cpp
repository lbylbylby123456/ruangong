//#pragma once

//四个函数
//两个重载 
#include <iostream>

//#include <typeinfo>

#include <string>
#include <typeindex>
//基类析构函数标记成virtual 

//using namespace std;

using std::cout;

//is a单继承模型 
class value_holder 
{

public:

	virtual const std::type_index & type() const= 0;//抽象（共同东西总结出） 
//  这里用type_index 
// noexcept 提高性能 
//构造析构/内存申请释放——不会有异常？ 
	virtual value_holder * copy() const= 0;
	
	value_holder(const value_holder &a){}
	
	value_holder()=default;//默认构造函数default 
	
	virtual value_holder&operator=(value_holder const &a){} //赋值函数 
	
	virtual ~value_holder(){}//析构函数 
	




};

template<typename T>
//oop去掉模板类
//使用继承实现擦除 
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

	virtual value_holder *copy() const override  //重写基类虚函数要写override 
	{

		return new holder(data);

	}//不要照搬stl 

};

class any final 
{
//五大函数：析构函数 默认构造函数 复制构造函数 赋值函数 自定义构造函数 还有移动构造 

//private:
//value_holder * value = nullptr;

public:

    value_holder * value = nullptr;

	template<typename T>
	any(const T &a):value(new holder<T>(a)){}
	
	any()=default; 
    
	~any()
    //不会被继承的，不用加virtual，写会造成性能损失 
	{
		delete value; 
	}

     
	any &operator=(any const &a) //赋值函数 
	{

		if(this!=&a)//检测自赋值		
		delete value;
        
        //防止空 判断指针可用性 
        
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
