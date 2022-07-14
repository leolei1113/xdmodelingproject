#pragma once

#ifdef WIN32
#ifdef XDMODELING_EXPORTS
#define XDMODELING_API __declspec(dllexport)
#else
#define XDMODELING_API __declspec(dllimport)
#endif
#else
#ifdef XDMODELING_EXPORTS
#define XDMODELING_API __attribute__((visibility("default")))
#else
#define XDMODELING_API __attribute__((visibility("default")))
#endif
#endif