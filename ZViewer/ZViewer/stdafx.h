#pragma once

/// _WIN32_WINNT means NT 4.0. If you want for above Win2000, use 0x0500. Check http://a.tk.co.kr/75
#define _WIN32_WINNT 0x0501

#include <WindowsX.h>
#include <shlobj.h>
#include <tchar.h>
#include <strsafe.h>

#include <atomic>
#include <cassert>
#include <fstream>
#include <map>
#include <mutex>
#include <queue>
#include <string>
#include <unordered_map>

#pragma warning(push)
#pragma warning(disable:4819)
#include <FreeImagePlus.h>
#pragma warning(pop)

#if defined(_M_IX86)
#ifdef _DEBUG
#include "vld/vld.h"
#endif
#elif defined(_M_X64)
// don't use vld
#else
static_assert(false, "unknown architecture");
#endif

#include "CommonDefine.h"
