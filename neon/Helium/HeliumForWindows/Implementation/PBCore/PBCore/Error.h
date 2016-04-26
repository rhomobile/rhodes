#pragma once
/**
 * General error codes
 */
enum errType
{
	SUCCESS = 0,				
	ERR,
	MEM_ALLOC_ERR,			///< error dynamically allocating memory
	FILE_ERR,				
	ERR_DLL_LOAD,			///< error dynamically loading a dll
	DLL_ERR,
	TIMEOUT_ERR,
	NULL_HANDLE_ERR,
	NULL_PTR,
	BAD_PARAM,
	XML_ERR,				///< a XML syntax error
	THREAD_ERR,
	WND_DIM_ERR
};
