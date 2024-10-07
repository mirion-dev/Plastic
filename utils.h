#pragma once

#ifdef _MSC_VER
#define PLASTIC_DEBUG _DEBUG
#endif

#if PLASTIC_DEBUG
#define PLASTIC_VERIFY(expr) do { if (!(expr)) { abort(); } } while (false)
#else
#define PLASTIC_VERIFY(expr) do {} while (false)
#endif