#pragma once

const unsigned int VersionMajor = 1;
const unsigned int VersionMinor = 1;
const unsigned int VersionPatch = 0;

const wchar_t GameWindowName[] = L"MELTY BLOOD: TYPE LUMINA ";
const wchar_t GameExecutableName[] = L"MBTL.exe";
const char MeltyBloodUpdateDatecode[] = "2023/02/10";

const unsigned int DummyDataMaxNumFrame = 1798;
const unsigned int DummyDataMaxSize = DummyDataMaxNumFrame * sizeof(int);

const unsigned int DummyData1_SizeAddress = 0x17CB744;
const unsigned int DummyData2_SizeAddress = 0x17CB7A4;
const unsigned int DummyData3_SizeAddress = 0x17CB804;
const unsigned int DummyData4_SizeAddress = 0x17CB864;
const unsigned int DummyData5_SizeAddress = 0x17CB8C4;

const unsigned int DummyData1_EnabledFlag = 0x17CB740;
const unsigned int DummyData2_EnabledFlag = 0x17CB7A0;
const unsigned int DummyData3_EnabledFlag = 0x17CB800;
const unsigned int DummyData4_EnabledFlag = 0x17CB860;
const unsigned int DummyData5_EnabledFlag = 0x17CB8C0;

const unsigned int DummyData1_BlockStartAddress = 0x17CBAAC;
const unsigned int DummyData1_BlockEndAddress = DummyData1_BlockStartAddress + DummyDataMaxSize;

const unsigned int DummyData2_BlockStartAddress = 0x17CEDDF;
const unsigned int DummyData2_BlockEndAddress = DummyData2_BlockStartAddress + DummyDataMaxSize;

const unsigned int DummyData3_BlockStartAddress = 0x17D2112;
const unsigned int DummyData3_BlockEndAddress = DummyData3_BlockStartAddress + DummyDataMaxSize;

const unsigned int DummyData4_BlockStartAddress = 0x17D5445;
const unsigned int DummyData4_BlockEndAddress = DummyData4_BlockStartAddress + DummyDataMaxSize;

const unsigned int DummyData5_BlockStartAddress = 0x17D8778;
const unsigned int DummyData5_BlockEndAddress = DummyData5_BlockStartAddress + DummyDataMaxSize;
