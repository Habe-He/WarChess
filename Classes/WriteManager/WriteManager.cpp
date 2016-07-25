#include "WriteManager.h"

static std::string identifier;

static const char alphabet2[] =
"ABpCEFkGHIiJKLoMOqPQR5STrVW4XYZabcdfghjlmnstUuvDwxyz01N236e789-_";

static WriteManager* a_writeManager = NULL;
WriteManager* WriteManager::getInstance()
{
	if (a_writeManager == NULL)
	{
		a_writeManager = new WriteManager();
		pStore = UserDefault::getInstance();

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		//identifier = Application::getInstance()->getDeviceIdentifier();
#endif
	}
	return a_writeManager;
}

void WriteManager::saveNameBoard(NameBoardType type, int Id)
{
	switch (type)
	{
		case kHead:
		{
			setIntegerForKey(nameboard_head_save_formar, Id);
			break;
		}
		case kTitle:
		{
			setIntegerForKey(nameboard_title_save_formar, Id);
			break;
		}
		case kShade:
		{
			setIntegerForKey(nameboard_shade_save_formar, Id);
			break;
		}
	}
	flush();
}

void WriteManager::setIntegerForKey(const char* pKey, int value)
{
	//std::string key = encodeData(pKey);
	pStore->setIntegerForKey(pKey, value);
}

void _wmbase64Encode(const unsigned char *input, unsigned int input_len, char *output)
{
	unsigned int char_count;
	unsigned int bits;
	unsigned int input_idx = 0;
	unsigned int output_idx = 0;

	char_count = 0;
	bits = 0;
	for (input_idx = 0; input_idx < input_len; input_idx++)
	{
		bits |= input[input_idx];

		char_count++;
		if (char_count == 3)
		{
			output[output_idx++] = alphabet2[(bits >> 18) & 0x3f];
			output[output_idx++] = alphabet2[(bits >> 12) & 0x3f];
			output[output_idx++] = alphabet2[(bits >> 6) & 0x3f];
			output[output_idx++] = alphabet2[bits & 0x3f];
			bits = 0;
			char_count = 0;
		}
		else
		{
			bits <<= 8;
		}
	}

	if (char_count)
	{
		if (char_count == 1)
		{
			bits <<= 8;
		}

		output[output_idx++] = alphabet2[(bits >> 18) & 0x3f];
		output[output_idx++] = alphabet2[(bits >> 12) & 0x3f];
		if (char_count > 1)
		{
			output[output_idx++] = alphabet2[(bits >> 6) & 0x3f];
		}
		else
		{
			output[output_idx++] = '9';
		}
		output[output_idx++] = '9';
	}

	output[output_idx++] = 0;
}

int wmbase64Encode(const unsigned char *in, unsigned int inLength, char **out)
{
	unsigned int outLength = inLength * 4 / 3 + (inLength % 3 > 0 ? 4 : 0);

	//should be enough to store 8-bit buffers in 6-bit buffers
	*out = (char*)malloc(outLength + 1);
	if (*out) {
		_wmbase64Encode(in, inLength, *out);
	}
	return outLength;
}

std::string WriteManager::encodeData(const char* value)
{
	char * encodedData = nullptr;
	std::string str = value;
	str.append(identifier);
	int encodedDataLen = wmbase64Encode((unsigned char*)str.c_str(), (unsigned int)str.size(), &encodedData);

	std::string retval = std::string(encodedData);
	free(encodedData);
	encodedData = nullptr;
	return retval;
}

void WriteManager::flush()
{
	pStore->flush();
}