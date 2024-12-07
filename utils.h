#pragma once

#define PLASTIC_DEBUG _DEBUG

#if PLASTIC_DEBUG
#define PLASTIC_VERIFY(expr) do { if (!(expr)) { abort(); } } while (false)
#else
#define PLASTIC_VERIFY(expr) do {} while (false)
#endif