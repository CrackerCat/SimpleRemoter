#pragma once
#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <wincrypt.h>


// �����ɷ���˹��ܽ��м���

std::string getHardwareID();

std::string hashSHA256(const std::string& data);

std::string getFixedLengthID(const std::string& hash);

std::string deriveKey(const std::string& password, const std::string& hardwareID);

