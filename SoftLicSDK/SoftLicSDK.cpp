#include "SoftLicSDK.h"
#include "base64.h"
#include "xxtea.h"
#include "json/json.h"
#include <time.h>

namespace SoftLicSDK {
	bool Check(const std::string& licpath, const std::string& softid, const std::string& pwd, std::string& msg)
	{
		FILE* pFile = NULL;
		long lSize = 0;
		unsigned char* buffer = NULL;
		size_t result;
		msg.clear();

		pFile = fopen(licpath.c_str(), "rb");
		if (pFile == NULL)
		{
			msg = "open file err.";
			return false;
		}
		// Get file size.
		fseek(pFile, 0, SEEK_END);
		lSize = ftell(pFile);
		rewind(pFile);

		buffer = new unsigned char[sizeof(char) * lSize];

		// Read file content to buffer.
		result = fread(buffer, 1, lSize, pFile);
		if (result != lSize)
		{
			msg = "read file failed.";
			return false;
		}

		fclose(pFile);

		// Decode base64 to decode_buffer;
		unsigned char* decode_buffer = NULL;
		int outLength = cocos2d::base64Decode(buffer, lSize * sizeof(char), &decode_buffer);
		
		// Decrypt buffer by xxtea.
		xxtea_long ret_length = 0;
		unsigned char* decrypt_buff = NULL;
		decrypt_buff = xxtea_decrypt(decode_buffer, outLength, (unsigned char*)pwd.c_str(), pwd.length(), &ret_length);


		Json::Value jsLic;
		Json::Reader reader;	

		if (!reader.parse(std::string((char*)decrypt_buff), jsLic))
		{
			msg = "license format err.";
			return false;
		}
		std::string _softid = jsLic["softid"].asCString();
		std::string _start_time = jsLic["start_time"].asCString();
		std::string _end_time = jsLic["end_time"].asCString();

		if (softid != _softid)
		{
			msg = "softid err.";
			return false;
		}

		// Compare _start_data with _end_date.
// 		struct tm tmStart, tmEnd;
// 		memset(&tmStart, 0, sizeof(tmStart));
// 		memset(&tmEnd, 0, sizeof(tmEnd));
// 
// 		sscanf(_start_time.c_str(), "%4d-%2d-%2d %2d:%2d:%2d", 
// 			 &tmStart.tm_year, &tmStart.tm_mon, &tmStart.tm_mday, &tmStart.tm_hour, &tmStart.tm_min, &tmStart.tm_sec);
// 		sscanf(_end_time.c_str(), "%4d-%2d-%2d %2d:%2d:%2d",
// 			 &tmEnd.tm_year, &tmEnd.tm_mon, &tmEnd.tm_mday, &tmEnd.tm_hour, &tmEnd.tm_min, &tmEnd.tm_sec);
// 
// 		time_t ttStart = mktime(&tmStart);
// 		time_t ttEnd = mktime(&tmEnd);

		time_t ttStart = 0, ttEnd = 0;
		ttStart = _atoi64(_start_time.c_str());
		ttEnd = _atoi64(_end_time.c_str());

		if (ttStart > ttEnd)
		{
			msg = "the license has expred.";
			return false;
		}

		delete buffer;
		free(decode_buffer);
		free(decrypt_buff);

		return true;
	}

	inline struct tm timet2tm(time_t time)
	{
		struct tm* ptm;
		struct tm _tm;
		ptm = localtime(&time); 
		memcpy(&_tm, ptm, sizeof(tm));
		return _tm;
	}

	bool Build(const LicInfo& lic, const std::string& outpath)
	{
		// Build json format license.
		Json::Value jsLic;
		jsLic["softid"] = lic.softid;

// 		struct tm tmStart, tmEnd;
// 		tmStart = timet2tm(lic.start_time);
// 		tmEnd = timet2tm(lic.end_time);

		// Convert time_t to string buffer.
		char buffStart[_MAX_PATH] = { 0 };
		char buffEnd[_MAX_PATH] = { 0 };

// 		sprintf(buffStart, "%04d-%02d-%02d %d:%d:%d", tmStart.tm_year + 1900,
// 			tmStart.tm_mon + 1, tmStart.tm_mday, tmStart.tm_hour, tmStart.tm_min, tmStart.tm_sec);
// 
// 		sprintf(buffEnd, "%04d-%02d-%02d %d:%d:%d", tmEnd.tm_year + 1900,
// 			tmEnd.tm_mon + 1, tmEnd.tm_mday, tmEnd.tm_hour, tmEnd.tm_min, tmEnd.tm_sec);

		_i64toa(lic.start_time, buffStart, 10);
		_i64toa(lic.end_time, buffEnd, 10);

		jsLic["start_time"] = buffStart;
		jsLic["end_time"] = buffEnd;

		// Encrpy json buffer.
		xxtea_long ret_length = 0;
		unsigned char* encrypt_buff = NULL;
		encrypt_buff = xxtea_encrypt((unsigned char*)jsLic.toStyledString().c_str(), jsLic.toStyledString().length(),
			(unsigned char*)lic.pasword.c_str(), lic.pasword.length(), &ret_length);

		// Encode buffer by base64.
		char* encode_buff = NULL;
		int outLength = cocos2d::base64Encode(encrypt_buff, ret_length, &encode_buff);
		
		// Write to file.
		FILE* pFile = NULL;
		pFile = fopen(outpath.c_str(), "w+");
		if (pFile == NULL)
			return false;

		size_t result = fwrite(encode_buff, sizeof(char), outLength, pFile);
		if (result != outLength)
			return false;

		fclose(pFile);

		free(encrypt_buff);
		free(encode_buff);

		return true;
	}
}

