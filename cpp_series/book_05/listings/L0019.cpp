#ifndef HARBOR_SCALE_HPP
#define HARBOR_SCALE_HPP
#if defined(_WIN32) && defined(HARBOR_SHARED)
# if defined(HARBOR_BUILDING)
#  define HARBOR_API __declspec(dllexport)
# else
#  define HARBOR_API __declspec(dllimport)
# endif
#elif defined(__GNUC__) && defined(HARBOR_SHARED)
# define HARBOR_API __attribute__((visibility("default")))
#else
# define HARBOR_API
#endif
namespace harbor { HARBOR_API int scale(int value, int factor); }
#endif
