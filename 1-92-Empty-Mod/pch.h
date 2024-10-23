// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#pragma once
#define WIN32_LEAN_AND_MEAN
#define CURL_STATICLIB
#include <curl/curl.h>

#include "framework.h"
#include <gd.h>
#include <cocos2d.h>
#include <MinHook.h>

//#include <cocoa/CCAffineTransform.h>
//#include <ExtensionMacros.h>W
//#include <GUI/CCControlExtension/CCScale9Sprite.h>
//#include <spine/extension.h>
//#include <GUI/CCControlExtension/CCControlUtils.h>
//#include <GUI/CCControlExtension/CCControlColourPicker.h>
//#include <GUI/CCControlExtension/CCControl.h>

using namespace cocos2d;
using namespace extension;
#endif //PCH_H
