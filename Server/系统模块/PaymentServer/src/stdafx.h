#ifndef _STDAFX_H_
#define _STDAFX_H_

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <assert.h>
#include <vector>
#include <map>
#include <array>
#include <memory>
#include <utility>
#include <fstream>
#include <sstream>
#include <signal.h>

//////////////////////////////////////////////////////////////////////////

#include <boost/bind.hpp>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/system/error_code.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>

#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio.hpp>
#include <boost/asio.hpp>

#include <boost/thread.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>

//////////////////////////////////////////////////////////////////////////

#include "CMD_PaymentServer.h"

//////////////////////////////////////////////////////////////////////////

#include "json/json.h"
#include "Tools/Util.hpp"
#include "Tools/MD5.h"

//////////////////////////////////////////////////////////////////////////

#include <curl/curl.h>

//////////////////////////////////////////////////////////////////////////

#pragma warning(disable:4996)

//////////////////////////////////////////////////////////////////////////////////

//MFC 文件引用windows版本

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif

#ifndef WINVER
#define WINVER 0x0500
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#endif						

#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS 0x0410
#endif

#ifndef _WIN32_IE
#define _WIN32_IE 0x0400
#endif

#endif

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////