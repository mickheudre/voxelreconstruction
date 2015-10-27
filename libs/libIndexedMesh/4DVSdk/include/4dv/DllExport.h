#ifndef _DLL_EXPORT_H_
#define _DLL_EXPORT_H_

#if defined _WIN32 || defined WIN32
	#ifdef DLL_EXPORT_4DVSDK
		#define DllImportExport  __declspec( dllexport )
	#else
		#define DllImportExport  __declspec( dllimport )
	#endif
#else
	#define DllImportExport
#endif

#endif