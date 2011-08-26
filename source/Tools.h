//////////////////////////////////////////////////////////////////////////  
///		COPYRIGHT NOTICE  
///     Copyright (c) 2011, 武汉大学资源环境科学学院
///     All rights reserved.
///  
/// @file Tools.h  
/// @brief 微粒群优化算法
///  
/// 实现了实现PSO算法的一些工具类的定义
///  
/// @version 1.0
/// @author jingsam
/// @date 2011-07-30
///  
///  
///		修订说明：最初版本
///		本代码是根据Günther M. FOIDL的C#版本Particle swarm optimization
///		for function optimization改写,遵从CDDL-1.0协议
///		详见：http://www.codeproject.com/KB/recipes/particleswarmoptimization.aspx
//////////////////////////////////////////////////////////////////////////

#ifndef TOOLS_H
#define POOLS_H


///	随机类--随机数生成器
class Random
{
	private:
		long int state;

		static const long int A = 48271;
        static const long int M = 2147483647;
        static const long int Q = 44488;
        static const long int R = 3399;

	public:
		/// 构造函数
		explicit Random();

		/// @return [0,1]区间内的随机数
		double NextDouble();
};


#endif