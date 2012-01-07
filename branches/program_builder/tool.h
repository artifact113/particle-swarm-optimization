#ifndef TOOL_H_
#define TOOL_H_

#define MAX_DOUBLE 1.0E+100
#define MIN_DOUBLE 1.0E-6

///	随机类--随机数生成器
class Random {
public:
	/// 构造函数
	explicit Random();

	/// @return [0,1]区间内的随机数
	double NextDouble();

private:
	long int state;

	static const long int A = 48271;
    static const long int M = 2147483647;
    static const long int Q = 44488;
    static const long int R = 3399;
};

#endif // TOOL_H_
