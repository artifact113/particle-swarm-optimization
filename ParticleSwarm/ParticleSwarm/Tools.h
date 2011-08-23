//////////////////////////////////////////////////////////////////////////  
///		COPYRIGHT NOTICE  
///     Copyright (c) 2011, �人��ѧ��Դ������ѧѧԺ
///     All rights reserved.
///  
/// @file Tools.h  
/// @brief ΢��Ⱥ�Ż��㷨
///  
/// ʵ����ʵ��PSO�㷨��һЩ������Ķ���
///  
/// @version 1.0
/// @author jingsam
/// @date 2011-07-30
///  
///  
///		�޶�˵��������汾
///		�������Ǹ���G��nther M. FOIDL��C#�汾Particle swarm optimization
///		for function optimization��д,���CDDL-1.0Э��
///		�����http://www.codeproject.com/KB/recipes/particleswarmoptimization.aspx
//////////////////////////////////////////////////////////////////////////

#ifndef TOOLS_H
#define POOLS_H


///	�����--�����������
class Random
{
	private:
		long int state;

		static const long int A = 48271;
        static const long int M = 2147483647;
        static const long int Q = 44488;
        static const long int R = 3399;

	public:
		/// ���캯��
		explicit Random();

		/// @return [0,1]�����ڵ������
		double NextDouble();
};


#endif