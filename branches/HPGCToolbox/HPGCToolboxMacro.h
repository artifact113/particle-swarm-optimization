#ifndef _HPGCTOOLBOXMACRO_H_
#define _HPGCTOOLBOXMACRO_H_

#ifndef HPGCTOOLBOXEXTERN
#ifdef WIN32
#  define HPGCTOOLBOXEXTERN extern "C" __declspec( dllexport )
#  ifdef _MSC_VER
// do not warn about C bindings returing QString
#    pragma warning(disable:4190)
#  endif
#else
#  define HPGCTOOLBOXEXTERN extern "C"
#endif
#endif


enum PLUGINTYPE
{
	ALGORITHM = 1,		//  算法包
	INDICATOR,			//  指标计算包
	FORMULA				//  公式包
};

#endif // _HPGCTOOLBOXMACRO_H_