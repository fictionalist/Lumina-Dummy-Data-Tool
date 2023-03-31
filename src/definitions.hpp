#pragma once

const unsigned int VersionMajor = 1;
const unsigned int VersionMinor = 0;

const wchar_t GameWindowName[] = L"MELTY BLOOD: TYPE LUMINA ";
const wchar_t GameExecutableName[] = L"MBTL.exe";
const char MeltyBloodUpdateDatecode[] = "2023/02/10";

const unsigned int DummyDataMaxNumFrame = 1798;
const unsigned int DummyDataMaxSize = DummyDataMaxNumFrame * sizeof(int);

const unsigned int DummyData1_SizeAddress = 0x17C0884;
const unsigned int DummyData2_SizeAddress = 0x17C08E4;
const unsigned int DummyData3_SizeAddress = 0x17C0944;
const unsigned int DummyData4_SizeAddress = 0x17C09A4;
const unsigned int DummyData5_SizeAddress = 0x17C0A04;

const unsigned int DummyData1_BlockStartAddress = 0x17C0BEC;
const unsigned int DummyData1_BlockEndAddress = DummyData1_BlockStartAddress + DummyDataMaxSize;

const unsigned int DummyData2_BlockStartAddress = 0x17C3F1F;
const unsigned int DummyData2_BlockEndAddress = DummyData2_BlockStartAddress + DummyDataMaxSize;

const unsigned int DummyData3_BlockStartAddress = 0x17C7252;
const unsigned int DummyData3_BlockEndAddress = DummyData3_BlockStartAddress + DummyDataMaxSize;

const unsigned int DummyData4_BlockStartAddress = 0x17CA585;
const unsigned int DummyData4_BlockEndAddress = DummyData4_BlockStartAddress + DummyDataMaxSize;

const unsigned int DummyData5_BlockStartAddress = 0x17CD8B8;
const unsigned int DummyData5_BlockEndAddress = DummyData5_BlockStartAddress + DummyDataMaxSize;