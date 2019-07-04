#ifndef LIBSITL_CONFIG_H
#define LIBSITL_CONFIG_H

#if defined(_WIN32) || defined(_WIN64)
#ifdef SITL_EXPORTS
#define SITL_API __declspec(dllexport)
#else
#define SITL_API __declspec(dllimport)
#endif
#else
#define SITL_EXPORTS __attribute__((visibility("default")))
#endif

#endif //LIBSITL_CONFIG_H
