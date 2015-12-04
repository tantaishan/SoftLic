/************************************************************************
 * SoftLicSDK
 * Copyright (c) 2015, Sam Tan
 * All rights reserved
 *
 ************************************************************************/

#ifndef _SOFT_LIC_SDK_H_
#define _SOFT_LIC_SDK_H_

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

typedef struct _LicInfo
{
	std::string softid;
	std::string pasword;
	time_t start_time;
	time_t end_time;
} LicInfo;

namespace SoftLicSDK {

/**
 * Check lincense function.
 * @returns the result of check
 */
bool Check(const std::string& licpath, const std::string& softid, const std::string& pwd, std::string& msg);

/**
 * Build license function.
 * 
 */
bool Build(const LicInfo& lic, const std::string& outpath);

}
#endif