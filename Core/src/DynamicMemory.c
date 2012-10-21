#include "./../include/DynamicMemory.h"

//-----------------------------------------------------------------------------
/* OPalloc(..) - Platform independent means to dynamically allocate memory.
 *	Parameters:
 *		uint bytes - Number of consecutive bytes to be allocated.
 *	Returns:
 *		void* - Address of allocated memory.
 */
void* OPalloc(OPuint bytes){
	// allocate memory (for each platform)
	#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64) || defined(OPIFEX_ANDROID)
	// android specific for malloc
	return malloc(bytes);
	#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64)
	// Windows specific for malloc
	return malloc(bytes);
	#endif
}
//-----------------------------------------------------------------------------
/* OPrealloc(..) - Platform independent means to dynamically reallocate memory.
 *	Parameters:
 *		void* ptr - Existing segment of allocated memory
 *		uint  bytes - Number of consecutive bytes to be allocated.
 *	Returns:
 *		void* - Address of allocated memory.
 */
void* OPrealloc(void* ptr, OPuint bytes){
	// allocate memory (for each platform)
	#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64) || defined(OPIFEX_ANDROID)
	// android specific for malloc
	return realloc(ptr, bytes);
	#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64)
	// Windows specific for malloc
	return realloc(ptr, bytes);
	#endif
}
//-----------------------------------------------------------------------------
/* OPfree(..) - Platform independent means do deallocate dynamically
 *		allocated memory. If a null pointer is passed no action
 *		is taken.
 *	Parameters:
 *		void* ptr - Address to the memory segment to deallocate
 *	Returns:
 *		Nothing...
 */
void OPfree(void* ptr){
	// free allocated memory (for each platform)
	#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64) || defined(OPIFEX_ANDROID)
	// android specific for malloc
	if(ptr) free(ptr);
	#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64)
	// Windows specific for malloc
	if(ptr) free(ptr);
	#endif
}
