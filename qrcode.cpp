# include "qrcode.h"
# include <cstdio>
# include <cstdlib>

const int LONG = sizeof(unsigned long) * 8;

/*
 * Each encoding mode has a four-bit mode indicator that identifies it.
 */
vector<unsigned char> QRCode::modeIndicator = {
    /* NUMERIC---ALPHANUMERIC---BYTE-------KANJI */
        0x01,       0x02,       0x04,       0x08
};

/*
 * The character capacity table of the of all versions.
 */
vector<vector<unsigned int>> QRCode::charCapacity = {
    /*     NUMERIC           ALPHANUMERIC            BYTE               KANJI       */
    /* L -- M -- Q -- H    L -- M -- Q -- H    L -- M -- Q -- H    L -- M -- Q -- H */
    {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},
    {  41,  34,  27,  17,  25,  20,  16,  10,  17,  14,  11,   7,  10,   8,   7,   4},
    {  77,  63,  48,  34,  47,  38,  29,  20,  32,  26,  20,  14,  20,  16,  12,   8},
    { 127, 101,  77,  58,  77,  61,  47,  35,  53,  42,  32,  24,  32,  26,  20,  15},
    { 187, 149, 111,  82, 114,  90,  67,  50,  78,  62,  46,  34,  48,  38,  28,  21},
    { 255, 202, 144, 106, 154, 122,  87,  64, 106,  84,  60,  44,  65,  52,  37,  27},
    { 322, 255, 178, 139, 195, 154, 108,  84, 134, 106,  74,  58,  82,  65,  45,  36},
    { 370, 293, 207, 154, 224, 178, 125,  93, 154, 122,  86,  64,  95,  75,  53,  39},
    { 461, 365, 259, 202, 279, 221, 157, 122, 192, 152, 108,  84, 118,  93,  66,  52},
    { 552, 432, 312, 235, 335, 262, 189, 143, 230, 180, 130,  98, 141, 111,  80,  60},
    { 652, 513, 364, 288, 395, 311, 221, 174, 271, 213, 151, 119, 167, 131,  93,  74},
    { 772, 604, 427, 331, 468, 366, 259, 200, 321, 251, 177, 137, 198, 155, 109,  85},
    { 883, 691, 489, 374, 535, 419, 296, 227, 367, 287, 203, 155, 226, 177, 125,  96},
    {1022, 796, 580, 427, 619, 483, 352, 259, 425, 331, 241, 177, 262, 204, 149, 109},
    {1101, 871, 621, 468, 667, 528, 376, 283, 458, 362, 258, 194, 282, 223, 159, 120},
    {1250, 991, 703, 530, 758, 600, 426, 321, 520, 412, 292, 220, 320, 254, 180, 136},
    {1408,1802, 775, 602, 854, 656, 470, 365, 586, 450, 322, 250, 361, 277, 198, 154},
    {1548,1212, 876, 674, 938, 734, 531, 408, 644, 504, 364, 280, 397, 310, 224, 173},
    {1725,1346, 948, 746,1046, 816, 574, 452, 718, 560, 394, 310, 442, 345, 243, 191},
    {1903,1500,1063, 813,1153, 909, 644, 493, 792, 624, 442, 338, 488, 384, 272, 208},
    {2061,1600,1159, 919,1249, 970, 702, 557, 858, 666, 482, 382, 528, 410, 297, 235},
    {2232,1708,1224, 969,1352,1035, 742, 587, 929, 711, 509, 403, 572, 438, 314, 248},
    {2409,1872,1358,1056,1460,1134, 823, 640,1003, 779, 565, 439, 618, 480, 348, 270},
    {2620,2059,1468,1108,1588,1248, 890, 672,1091, 857, 611, 461, 672, 528, 376, 284},
    {2812,2188,1588,1228,1704,1326, 963, 744,1171, 911, 661, 511, 721, 561, 407, 315},
    {3057,2395,1718,1286,1853,1451,1041, 779,1273, 997, 715, 535, 784, 614, 440, 330},
    {3283,2544,1804,1425,1990,1542,1094, 864,1367,1059, 751, 593, 842, 652, 462, 365},
    {3517,2701,1933,1501,2132,1637,1172, 910,1465,1125, 805, 625, 902, 692, 496, 385},
    {3669,2857,2085,1581,2223,1732,1263, 958,1528,1190, 868, 658, 940, 732, 534, 405},
    {3909,3035,2181,1677,2369,1839,1322,1016,1628,1264, 908, 698,1002, 778, 559, 430},
    {4158,3289,2358,1782,2520,1994,1429,1080,1732,1370, 982, 742,1066, 843, 604, 457},
    {4417,3486,2473,1897,2677,2113,1499,1150,1840,1452,1030, 790,1132, 894, 634, 486},
    {4686,3693,2670,2022,2840,2238,1618,1226,1952,1538,1112, 842,1201, 947, 684, 518},
    {4965,3909,2805,2157,3009,2369,1700,1307,2068,1628,1168, 898,1273,1002, 719, 553},
    {5253,4134,2949,2301,3183,2506,1787,1394,2188,1722,1228, 958,1347,1060, 756, 590},
    {5529,4343,3081,2361,3351,2632,1867,1431,2303,1809,1283, 983,1417,1113, 790, 605},
    {5836,4588,3244,2524,3537,2780,1966,1530,2431,1911,1351,1051,1496,1176, 832, 647},
    {6153,4775,3417,2625,3729,2894,2071,1591,2563,1989,1423,1093,1577,1224, 876, 673},
    {6479,5039,3599,2735,3927,3054,2181,1658,2699,2099,1499,1139,1661,1292, 923, 701},
    {6743,5313,3791,2927,4087,3220,2298,1774,2809,2213,1579,1219,1729,1362, 972, 750},
    {7089,5596,3993,3057,4296,3391,2420,1852,2953,2331,1663,1273,1817,1435,1024, 784}
};

/*
 * The legth table of character count indicators of different versions and modes.
 */
vector<vector<unsigned char>> QRCode::countLength = {
    /* NUMERIC---ALPHANUMERIC---BYTE--------KANJI */
      {  10,          9,          8,          8 }, /* v01-09 */
      {  12,         11,         16,         10 }, /* v10-26 */
      {  14,         13,         16,         12 }  /* v27-40 */
};

/*
 * The alphanumeric value table.
 */ 
map<char, unsigned short> QRCode::alphaValue = {
    {'0', 0}, {'1', 1}, {'2', 2}, {'3', 3}, {'4', 4}, {'5', 5}, {'6', 6},
    {'7', 7}, {'8', 8}, {'9', 9}, {'A',10}, {'B',11}, {'C',12}, {'D',13},
    {'E',14}, {'F',15}, {'G',16}, {'H',17}, {'I',18}, {'J',19}, {'K',20},
    {'L',21}, {'M',22}, {'N',23}, {'O',24}, {'P',25}, {'Q',26}, {'R',27},
    {'S',28}, {'T',29}, {'U',30}, {'V',31}, {'W',32}, {'X',33}, {'Y',34},
    {'Z',35}, {' ',36}, {'$',37}, {'%',38}, {'*',39}, {'+',40}, {'-',41},
    {'.',42}, {'/',43}, {':',44}
};

/*
 * The following table lists the number of
 * [0]. error correction codewords per block
 * [1]. number of blocks in group 1
 * [2]. number of data codewords in each of group 1's blocks
 * [3]. number of blocks in group 2
 * [4]. number of data codewords in each of group 2's blocks
 * for each version and error correction level of QR code.
 */
vector<vector<vector<unsigned short>>> QRCode::codewordNumber = {
    /* -------- L -------------------- M -------------------- Q -------------------- H -------- */
    { {  0,  0,  0,  0,  0}, {  0,  0,  0,  0,  0}, {  0,  0,  0,  0,  0}, {  0,  0,  0,  0,  0} },
    { {  7,  1, 19,  0,  0}, { 10,  1, 16,  0,  0}, { 13,  1, 13,  0,  0}, { 17,  1,  9,  0,  0} },
    { { 10,  1, 34,  0,  0}, { 16,  1, 28,  0,  0}, { 22,  1, 22,  0,  0}, { 28,  1, 16,  0,  0} },
    { { 15,  1, 55,  0,  0}, { 26,  1, 44,  0,  0}, { 18,  2, 17,  0,  0}, { 22,  2, 13,  0,  0} },
    { { 20,  1, 80,  0,  0}, { 18,  2, 32,  0,  0}, { 26,  2, 24,  0,  0}, { 16,  4,  9,  0,  0} },
    { { 26,  1,108,  0,  0}, { 24,  2, 43,  0,  0}, { 18,  2, 15,  2, 16}, { 22,  2, 11,  2, 12} },
    { { 18,  2, 68,  0,  0}, { 16,  4, 27,  0,  0}, { 24,  4, 19,  0,  0}, { 28,  4, 15,  0,  0} },
    { { 20,  2, 78,  0,  0}, { 18,  4, 31,  0,  0}, { 18,  2, 14,  4, 15}, { 26,  4, 13,  1, 14} },
    { { 24,  2, 97,  0,  0}, { 22,  2, 38,  2, 39}, { 22,  4, 18,  2, 19}, { 26,  4, 14,  2, 15} },
    { { 30,  2,116,  0,  0}, { 22,  3, 36,  2, 37}, { 20,  4, 16,  4, 17}, { 24,  4, 12,  4, 13} },
    { { 18,  2, 68,  2, 69}, { 26,  4, 43,  1, 44}, { 24,  6, 19,  2, 20}, { 28,  6, 15,  2, 16} },
    { { 20,  4, 81,  0,  0}, { 30,  1, 50,  4, 51}, { 28,  4, 22,  4, 23}, { 24,  3, 12,  8, 13} },
    { { 24,  2, 92,  2, 93}, { 22,  6, 36,  2, 37}, { 26,  4, 20,  6, 21}, { 28,  7, 14,  4, 15} },
    { { 26,  4,107,  0,  0}, { 22,  8, 37,  1, 38}, { 24,  8, 20,  4, 21}, { 22, 12, 11,  4, 12} },
    { { 30,  3,115,  1,116}, { 24,  4, 40,  5, 41}, { 20, 11, 16,  5, 17}, { 24, 11, 12,  5, 13} },
    { { 22,  5, 87,  1, 88}, { 24,  5, 41,  5, 42}, { 30,  5, 24,  7, 25}, { 24, 11, 12,  7, 13} },
    { { 24,  5, 98,  1, 99}, { 28,  7, 45,  3, 46}, { 24, 15, 19,  2, 20}, { 30,  3, 15, 13, 16} },
    { { 28,  1,107,  5,108}, { 28, 10, 46,  1, 47}, { 28,  1, 22, 15, 23}, { 28,  2, 14, 17, 15} },
    { { 30,  5,120,  1,121}, { 26,  9, 43,  4, 44}, { 28, 17, 22,  1, 23}, { 28,  2, 14, 19, 15} },
    { { 28,  3,113,  4,114}, { 26,  3, 44, 11, 45}, { 26, 17, 21,  4, 22}, { 26,  9, 13, 16, 14} },
    { { 28,  3,107,  5,108}, { 26,  3, 41, 13, 42}, { 30, 15, 24,  5, 25}, { 28, 15, 15, 10, 16} },
    { { 28,  4,116,  4,117}, { 26, 17, 42,  0,  0}, { 28, 17, 22,  6, 23}, { 30, 19, 16,  6, 17} },
    { { 28,  2,111,  7,112}, { 28, 17, 46,  0,  0}, { 30,  7, 24, 16, 25}, { 24, 34, 13,  0,  0} },
    { { 30,  4,121,  5,122}, { 28,  4, 47, 14, 48}, { 30, 11, 24, 14, 25}, { 30, 16, 15, 14, 16} },
    { { 30,  6,117,  4,118}, { 28,  6, 45, 14, 46}, { 30, 11, 24, 16, 25}, { 30, 30, 16,  2, 17} },
    { { 26,  8,106,  4,107}, { 28,  8, 47, 13, 48}, { 30,  7, 24, 22, 25}, { 30, 22, 15, 13, 16} },
    { { 28, 10,114,  2,115}, { 28, 19, 46,  4, 47}, { 28, 28, 22,  6, 23}, { 30, 33, 16,  4, 17} },
    { { 30,  8,122,  4,123}, { 28, 22, 45,  3, 46}, { 30,  8, 23, 26, 24}, { 30, 12, 15, 28, 16} },
    { { 30,  3,117, 10,118}, { 28,  3, 45, 23, 46}, { 30,  4, 24, 31, 25}, { 30, 11, 15, 31, 16} },
    { { 30,  7,116,  7,117}, { 28, 21, 45,  7, 46}, { 30,  1, 23, 37, 24}, { 30, 19, 15, 26, 16} },
    { { 30,  5,115, 10,116}, { 28, 19, 47, 10, 48}, { 30, 15, 24, 25, 25}, { 30, 23, 15, 25, 16} },
    { { 30, 13,115,  3,116}, { 28,  2, 46, 29, 47}, { 30, 42, 24,  1, 25}, { 30, 23, 15, 28, 16} },
    { { 30, 17,115,  0,  0}, { 28, 10, 46, 23, 47}, { 30, 10, 24, 35, 25}, { 30, 19, 15, 35, 16} },
    { { 30, 17,115,  1,116}, { 28, 14, 46, 21, 47}, { 30, 29, 24, 19, 25}, { 30, 11, 15, 46, 16} },
    { { 30, 13,115,  6,116}, { 28, 14, 46, 23, 47}, { 30, 44, 24,  7, 25}, { 30, 59, 16,  1, 17} },
    { { 30, 12,121,  7,122}, { 28, 12, 47, 26, 48}, { 30, 39, 24, 14, 25}, { 30, 22, 15, 41, 16} },
    { { 30,  6,121, 14,122}, { 28,  6, 47, 34, 48}, { 30, 46, 24, 10, 25}, { 30,  2, 15, 64, 16} },
    { { 30, 17,122,  4,123}, { 28, 29, 46, 14, 47}, { 30, 49, 24, 10, 25}, { 30, 24, 15, 46, 16} },
    { { 30,  4,122, 18,123}, { 28, 13, 46, 32, 47}, { 30, 48, 24, 14, 25}, { 30, 42, 15, 32, 16} },
    { { 30, 20,117,  4,118}, { 28, 40, 47,  7, 48}, { 30, 43, 24, 22, 25}, { 30, 10, 15, 67, 16} },
    { { 30, 19,118,  6,119}, { 28, 18, 47, 31, 48}, { 30, 34, 24, 34, 25}, { 30, 20, 15, 61, 16} }
};

/*
 * The following table contains the log values corresponding to the index
 * in GF(2^8) arithmetic.
 */
vector<unsigned char> QRCode::logTable = {
      0,  0,  1, 25,  2, 50, 26,198,  3,223, 51,238, 27,104,199, 75,
      4,100,224, 14, 52,141,239,129, 28,193,105,248,200,  8, 76,113,
      5,138,101, 47,225, 36, 15, 33, 53,147,142,218,240, 18,130, 69,
     29,181,194,125,106, 39,249,185,201,154,  9,120, 77,228,114,166,
      6,191,139, 98,102,221, 48,253,226,152, 37,179, 16,145, 34,136,
     54,208,148,206,143,150,219,189,241,210, 19, 92,131, 56, 70, 64,
     30, 66,182,163,195, 72,126,110,107, 58, 40, 84,250,133,186, 61,
    202, 94,155,159, 10, 21,121, 43, 78,212,229,172,115,243,167, 87,
      7,112,192,247,140,128, 99, 13,103, 74,222,237, 49,197,254, 24,
    227,165,153,119, 38,184,180,124, 17, 68,146,217, 35, 32,137, 46,
     55, 63,209, 91,149,188,207,205,144,135,151,178,220,252,190, 97,
    242, 86,211,171, 20, 42, 93,158,132, 60, 57, 83, 71,109, 65,162,
     31, 45, 67,216,183,123,164,118,196, 23, 73,236,127, 12,111,246,
    108,161, 59, 82, 41,157, 85,170,251, 96,134,177,187,204, 62, 90,
    203, 89, 95,176,156,169,160, 81, 11,245, 22,235,122,117, 44,215,
     79,174,213,233,230,231,173,232,116,214,244,234,168, 80, 88,175
};

/*
 * The following table contains the antilog values corresponding to the index
 * in GF(2^8) arithmetic.
 */
vector<unsigned char> QRCode::antilogTable = {
      1,  2,  4,  8, 16, 32, 64,128, 29, 58,116,232,205,135, 19, 38,
     76,152, 45, 90,180,117,234,201,143,  3,  6, 12, 24, 48, 96,192,
    157, 39, 78,156, 37, 74,148, 53,106,212,181,119,238,193,159, 35,
     70,140,  5, 10, 20, 40, 80,160, 93,186,105,210,185,111,222,161,
     95,190, 97,194,153, 47, 94,188,101,202,137, 15, 30, 60,120,240,
    253,231,211,187,107,214,177,127,254,225,223,163, 91,182,113,226,
    217,175, 67,134, 17, 34, 68,136, 13, 26, 52,104,208,189,103,206,
    129, 31, 62,124,248,237,199,147, 59,118,236,197,151, 51,102,204,
    133, 23, 46, 92,184,109,218,169, 79,158, 33, 66,132, 21, 42, 84,
    168, 77,154, 41, 82,164, 85,170, 73,146, 57,114,228,213,183,115,
    230,209,191, 99,198,145, 63,126,252,229,215,179,123,246,241,255,
    227,219,171, 75,150, 49, 98,196,149, 55,110,220,165, 87,174, 65,
    130, 25, 50,100,200,141,  7, 14, 28, 56,112,224,221,167, 83,166,
     81,162, 89,178,121,242,249,239,195,155, 43, 86,172, 69,138,  9,
     18, 36, 72,144, 61,122,244,245,247,243,251,235,203,139, 11, 22,
     44, 88,176,125,250,233,207,131, 27, 54,108,216,173, 71,142,  1
};

/*
 * The following table lists the number of remainder bits of 40 QR-code versions
 * that must be added to the end of the final message string. Note that the remainder
 * bits specified for each version are required no matter which error correction level
 * is in use. Also note that some versions, such as versions 7 through 13, do not
 * require any remainder bits to be added.
 */
vector<unsigned char> QRCode::remainderBit = { 0,
     0, 7, 7, 7, 7, 7, 0, 0, 0, 0,
     0, 0, 0, 3, 3, 3, 3, 3, 3, 3,
     4, 4, 4, 4, 4, 4, 4, 3, 3, 3,
     3, 3, 3, 3, 0, 0, 0, 0, 0, 0
};

/*
 * The table contains the coordinates of central modules of alignment patterns for all
 * kinds of versions. For example, the coordinates of central modules of alignment patterns
 * for version 14 have (6,26), (6,46), (26,6), (26,26), (26,46), (26,66), (46,6), (46,26),
 * (46,46), (46,66), (66,26), (66,46), (66,66).
 */
vector<vector<unsigned char>> QRCode::centerLocation = {
    {}, {},
    { 6, 18 },
    { 6, 22 },
    { 6, 26 },
    { 6, 30 },
    { 6, 34 },
    { 6, 22, 38 },
    { 6, 24, 42 },
    { 6, 26, 46 },
    { 6, 28, 50 },
    { 6, 30, 54 },
    { 6, 32, 58 },
    { 6, 34, 62 },
    { 6, 26, 46, 66 },
    { 6, 26, 48, 70 },
    { 6, 26, 50, 74 },
    { 6, 30, 54, 78 },
    { 6, 30, 56, 82 },
    { 6, 30, 58, 86 },
    { 6, 34, 62, 90 },
    { 6, 28, 50, 72, 94 },
    { 6, 26, 50, 74, 98 },
    { 6, 30, 54, 78,102 },
    { 6, 28, 54, 80,106 },
    { 6, 32, 58, 84,110 },
    { 6, 30, 58, 86,114	},
    { 6, 34, 62, 90,118 },
    { 6, 26, 50, 74, 98,122 },
    { 6, 30, 54, 78,102,126 },
    { 6, 26, 52, 78,104,130 },
    { 6, 30, 56, 82,108,134	},
    { 6, 34, 60, 86,112,138 },
    { 6, 30, 58, 86,114,142	},
    { 6, 34, 62, 90,118,146 },
    { 6, 30, 54, 78,102,126,150 },
    { 6, 24, 50, 76,102,128,154 },
    { 6, 28, 54, 80,106,132,158 },
    { 6, 32, 58, 84,110,136,162 },
    { 6, 26, 54, 82,110,138,166 },
    { 6, 30, 58, 86,114,142,170 }
};

/*
 * Format strings of different error correction levels and mask modes.
 */
vector<vector<unsigned short>> QRCode::formatString = {
        /* ZERO---ONE-----TWO----THREE----FOUR----FIVE----SIX----SEVEN */
/*L*/   {0x77c4, 0x72f3, 0x7daa, 0x789d, 0x662f, 0x6318, 0x6c41, 0x6976},
/*M*/   {0x5412, 0x5125, 0x5e7c, 0x5b4b, 0x45f9, 0x40ce, 0x4f97, 0x4aa0},
/*Q*/   {0x355f, 0x3068, 0x3f31, 0x3a06, 0x24b4, 0x2183, 0x2eda, 0x2bed},
/*H*/   {0x1689, 0x13be, 0x1ce7, 0x19d0, 0x0762, 0x0255, 0x0d0c, 0x083b}
};

/*
 * Version strings.
 */
vector<unsigned int> QRCode::versionString = { 0,
       /* 1--------2--------3--------4--------5--------6--------7--------8--------9-------10 */
/*0*/  0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x07c94, 0x085bc, 0x09a99, 0x0a4d3,
/*1*/  0x0bbf6, 0x0c762, 0x0d847, 0x0e60d, 0x0f928, 0x10b78, 0x1145d, 0x12a17, 0x13532, 0x149a6,
/*2*/  0x15683, 0x168c9, 0x177ec, 0x18ec4, 0x191e1, 0x1afab, 0x1b08e, 0x1cc1a, 0x1d33f, 0x1ed75,
/*3*/  0x1f250, 0x209d5, 0x216f0, 0x228ba, 0x2379f, 0x24b0b, 0x2542e, 0x26a64, 0x27541, 0x28c69
};

/*
 * Encode the last `len` length bits of `val` into sequence `seq`.
 */
void QRCode::encode(Sequence &seq, unsigned long val, unsigned char len) {
    unsigned long end = 0;
    if (!seq.sequence.empty()) {
        end = seq.sequence.back();
        seq.sequence.pop_back();
    }
    while (len>0) {
        if ((seq.sequence.size()+1)*LONG == seq.length) {
            seq.sequence.push_back(end);
            end = 0;
        }
        unsigned char rst = LONG - seq.length % LONG;
        if (len <= rst) {
            end |= val << (rst-len);
            seq.length += len;
            seq.sequence.push_back(end);
            return;
        }
        else {
            val = (val << (LONG-len)) >> (LONG-len);
            end |= val >> (len-rst);
            seq.sequence.push_back(end);
            end = 0; len -= rst;
            seq.length += rst;
        }
    }
}

/*
 * Decode and return the position `pos` and the length `len` bits of sequence.
 */
unsigned long QRCode::decode(const Sequence &seq, unsigned short pos, unsigned char len) {
    if (pos>seq.length || (pos+len)>seq.length) {
        printf("Error: the position(%d) and length(%d) to be decoded "
               "exceeds the length of sequence(%d).\n", pos, len, seq.length);
        exit(1);
    }
    if (len<0 || len>LONG) {
        printf("Error: the length(%d) to be decoded is too large, and "
               "exceeds the number(0-%d) of bits of unsigned long.\n", len, LONG);
        exit(1);
    }
    unsigned long val = 0;
    unsigned long end = 0;
    while (len > 0) {
        unsigned char rst = LONG - pos % LONG;
        if (len <= rst) {
            end = (seq.sequence[pos/LONG]<<(pos%LONG)) >> pos%LONG;
            val |= end >> (rst-len);
            return val;
        }
        else {
            end = (seq.sequence[pos/LONG]<<(pos%LONG)) >> pos%LONG;
            val |= end << (len-rst);
            len -= rst; pos += rst;
        }
    }
}

/*
 * Polynomial multiplication in GF(2^8). When the coefficient becomes 0, the item will be deleted.
 */
vector<QRCode::Polynomial> QRCode::multiply(const vector<Polynomial> &A, const vector<Polynomial> &B) {
    vector<Polynomial> P;
    for (int j=0; j<B.size(); ++j) {
        for (int i=0; i<A.size(); ++i) {
            Polynomial item;
            int log = logTable[A[i].coefficient]+logTable[B[j].coefficient];
            item.coefficient = antilogTable[log % 255];
            item.exponent = A[i].exponent + B[j].exponent;
            vector<Polynomial>::iterator iter = P.begin();
            while (iter!=P.end() && iter->exponent>item.exponent) ++iter;
            if (iter!=P.end() && iter->exponent==item.exponent) {
                iter->coefficient ^= item.coefficient;
                if (iter->coefficient == 0) P.erase(iter);
            }
            else P.insert(iter, item);
        }
    }
    return P;
}

/*
 * Polynomial division in GF(2^8). Return the polynomial remainder.
 */
vector<QRCode::Polynomial> QRCode::divide(const vector<QRCode::Polynomial> &A, const vector<QRCode::Polynomial> &B) {
    if (B.empty()) {
        printf("Error: the divisor polynomial can't be 0.\n");
        exit(1);
    }
    vector<Polynomial> Q;
    vector<Polynomial> R(A);
    while (!R.empty() && R[0].exponent>=B[0].exponent) {
        Q.push_back({R[0].coefficient, R[0].exponent-B[0].exponent});
        Polynomial temp = R[0];
        for (int j=0; j<B.size(); ++j) {
            Polynomial item;
            int log = logTable[B[j].coefficient]+logTable[temp.coefficient];
            item.coefficient = antilogTable[log % 255];
            item.exponent = B[j].exponent+temp.exponent-B[0].exponent;
            vector<Polynomial>::iterator iter = R.begin();
            while (iter!=R.end() && iter->exponent>item.exponent) ++iter;
            if (iter!=R.end() && iter->exponent==item.exponent) {
                iter->coefficient ^= item.coefficient;
                if (iter->coefficient==0) R.erase(iter);
            }
            else R.insert(iter, item);
        }
    }
    return R;
}

/*
 * Caculate the penalty score of the pattern `pat` according following four rules:
 */
unsigned int QRCode::penalize(vector<vector<Color>> pat) {
    unsigned int score = 0;
    /* 
     * The first rule gives the QR-code a penalty for each group
     * of five or more same-colored modules in a row (or column).
     * Check each row one-by-one. If there are five consecutive
     * modules of the same color, add 3 to the penalty. If there
     * are more modules of the same color after the first five,
     * add 1 for each additional module of the same color. Afterward, 
     * check each column one-by-one, checking for the same condition.
     * Add the horizontal and vertical total to obtain penalty score.
     */
    /* The `dark` is the number of dark modules used by the fourth rule. */
    unsigned short dark = 0;
    for (int i=0; i<pat.size(); ++i) {
        Color color[2] = { DARK, DARK };
        unsigned int count[2] = { 0, 0 };
        for (int j=0; j<pat[i].size(); ++j) {
            if (pat[i][j]==DARK) ++dark;
            /* When k is 0, caculate the score of the rows. */
            /* When k is 1, caculate the score of the columns. */
            for (int k=0; k<2; ++k) {
                int x=k?i:j, y=k?j:i;
                if (pat[x][y]==color[k]) ++count[k];
                else {
                    if (count[k] >= 5) score += (count[k]-2);
                    color[k] = pat[x][y]; count[k] = 1;
                }
            }
        }
    }
    /* 
     * The second rule gives the QR code a penalty for each 2x2 
     * area of same-colored modules in the matrix. Look for areas
     * of the same color that are at least 2x2 modules or larger.
     * The QR code specification says that for a solid-color block
     * of size m × n, the penalty score is 3 × (m - 1) × (n - 1).
     * However, the QR code specification does not specify how to
     * calculate the penalty when there are multiple ways of dividing
     * up the solid-color blocks. Therefore, rather than looking for
     * solid-color blocks larger than 2x2, simply add 3 to the penalty
     * score for every 2x2 block of the same color in the QR code,
     * making sure to count overlapping 2x2 blocks. For example, a
     * 3x2 block of the same color should be counted as two 2x2 blocks,
     * one overlapping the other.
     */
    for (int i=0; i<pat.size()-1; ++i) {
        for (int j=0; j<pat[i].size()-1; ++j) {
            if (pat[i][j]==pat[i+1][j] && pat[i][j]==pat[i][j+1] \
                && pat[i][j]==pat[i+1][j+1])
                score += 3;
        }
    }
    /* 
     * The third rule gives the QR code a large penalty if there
     * are patterns that look similar to the finder patterns. Looks
     * for patterns of dark-light-dark-dark-dark-light-dark that have
     * four light modules on either side. Each time this pattern is found,
     * add 40 to the penalty score.
     */
    for (int i=0; i<pat.size(); ++i) {
        for (int j=0; j<pat[i].size()-10; ++j) {
            vector<Color> type = { DARK, LIGHT, DARK, DARK, DARK, LIGHT, DARK, LIGHT, LIGHT, LIGHT, LIGHT };
            bool same[4] = { true, true, true, true };
            for (int k=0; k<11; ++k) {
                /* Forward match in the row. */
                if (same[0] && pat[i][j+k]!=type[k]) same[0] = false;
                /* Reverse match in the row. */
                if (same[1] && pat[i][j+k]!=type[10-k]) same[1] = false;
                /* Forward match in the column. */
                if (same[2] && pat[j+k][i]!=type[k]) same[2] = false;
                /* Reverse match in the column. */
                if (same[3] && pat[j+k][i]!=type[10-k]) same[3] = false;
            }
            for (int k=0; k<4; ++k) {
                if (same[k]) score += 40;
            }
        }
    }
    /* 
     * The fourth rule gives the QR code a penalty if more than half
     * of the modules are dark or light, with a larger penalty for a
     * larger difference.
     * 1.Count the total number of modules in the matrix.
     * 2.Count how many dark modules there are in the matrix.
     * 3.Calculate the percent of modules in the matrix that are dark:
     *          (darkmodules / totalmodules) * 100
     * 4.Determine the previous and next multiple of five of this percent.
     *   For example, for 43 percent, the previous multiple of five is 40,
     *   and the next multiple of five is 45.
     * 5.Subtract 50 from each of these multiples of five and take the absolute
     *   value of the result. For example:
     *      |40 - 50| = |-10| = 10 and |45 - 50| = |-5| = 5.
     * 6.Divide each of these by five. For example, 10/5 = 2 and 5/5 = 1.
     * 7.Finally, take the smallest of the two numbers and multiply it by 10.
     *   In this example, the lower number is 1, so the result is 10. This is
     *   penalty score
     */
    unsigned int rate = dark*100/(size*size);
    if ((rate/5)*5 < 50) score += 2*(50-(rate/5+1)*5);
    else score += 2*((rate/5)*5-50);

    return score;
}

/*
 * Now treat each group of digits as a three digit number (or fewer than
 * three, if the final group is two or one digits long). Convert that
 * three-digit number into 10 binary bits. If the final group consists of
 * only two digits, you should convert it to 7 binary bits, and if the final
 * group consists of only one digit, you should convert it to 4 binary bits.
 */
void QRCode::numericEncoding(Sequence &seq, const string &data) {
    if (data.size() == 0) {
        printf("Error: the data to numeric encode is null.\n");
        exit(1);
    }
    unsigned short val = data[0]-'0';
    for (int i=1; i<data.size(); ++i)
    {
        if (i%3 == 0) {
            encode(seq, val, 10);
            val = data[i]-'0';
        }
        else val = val*10+(data[i]-'0');
    }
    switch (data.size()%3)
    {
        case 1: encode(seq, val,  4); break;
        case 2: encode(seq, val,  7); break;
        case 0: encode(seq, val, 10); break;
    }
}

/*
 * First, break up the string into pairs of characters. For each pair
 * of characters, get the number representation (from the alphanumeric
 * table) of the first character and multiply it by 45. Then add that
 * number to the number representation of the second character. Then
 * convert that number into an 11-bit binary string, padding on the
 * left with 0s if necessary.
 */
void QRCode::alphanumericEncoding(Sequence &seq, const string &data) {
    if (data.size() == 0) {
        printf("Error: the data to alphanumeric encode is null.\n");
        exit(1);
    }
    for (int i=0; i<data.size()/2*2; i+=2) {
        unsigned short val = alphaValue[data[i]] * 45 + alphaValue[data[i+1]];
        encode(seq, val, 11);
    }
    if (data.size()%2 == 1) {
        encode(seq, alphaValue[data.size()-1],  6);
    }
}

/*
 * After converting your input string into ISO 8859-1 value, convert the
 * byte into an 8-bit binary string. Pad on the left with 0s if necessary
 * to make each one 8-bits long.
 */
void QRCode::byteEncoding(Sequence &seq, const string &data) {
    if (data.size() == 0) {
        printf("Error: the data to byte encode is null.\n");
        exit(1);
    }
    for (int i=0; i<data.size(); ++i) {
        /* Must tranfer `data[i]` to unsigned char firstly, especially when `data[i]` is negative. */
        /* If pass `data[i]` into `encode` directly, data[i] will be transfered to unsigned long, */
        /* which is wrong because the high invalid bits will be 1. For example, */
        /* data[i] = -1 (->unsigned char->unsigned long) 00000000,00000000,00000000,11111111 true */
        /* data[i] = -1 (->unsigned long)                11111111,11111111,11111111,11111111 false */
        unsigned char val = data[i];
        encode(seq, val, 8);
    }
}

/*
 * In the Shift JIS system, Kanji characters are represented by a two byte combination.
 * 1. For characters with Shift JIS values from 8140HEX to 9FFCHEX:
 *   a) Subtract 8140HEX from Shift JIS value;
 *   b) Multiply most significant byte of result by C0HEX;
 *   c) Add least significant byte to product from b);
 *   d) Convert result to a 13 bit binary string.
 * 2. For characters with Shift JIS values from E040HEX to EBBFHEX:
 *   a) Subtract C140HEX from Shift JIS value;
 *   b) Multiply most significant byte of result by C0HEX;
 *   c) Add least significant byte to product from b);
 *   d) Convert result to a 13 bit binary string;
 */
void QRCode::kanjiEncoding(Sequence &seq, const string &data) {
    if (data.size() == 0) {
        printf("Error: the data to Kanji encode is null.\n");
        exit(1);
    }
    if (data.size()%2 != 0) {
        printf("Error: the byte number(%d) of the Kanji string is odd.\n", data.size());
        exit(1);
    }
    for (int i=0; i<data.size(); i+=2) {
        unsigned short val;
        if (data[i]>=0x81 && data[i]<=0x9f && data[i+1]>=0x40 && data[i+1]<=0xfc) {
            val = (data[i]-0x81)*0xc0+(data[i+1]-0x40);
        }
        else if (data[i]>=0xe0 && data[i]<=0xeb && data[i+1]>=0x40 && data[i+1]<=0xbf) {
            val = (data[i]-0xc1)*0xc0+(data[i+1]-0x40);
        }
        encode(seq, val, 13);
    }
}

/*
 * When the string includes Chinese characters, use ECI mode.
 * The assignment value of eci designator:
 *  0,      Code page 437 
 *  2,      Code page 437 
 *  1,      ISO/IEC 8859-1 (Latin-1)
 *  3,      ISO/IEC 8859-1 (Latin-1)
 *  4,      ISO/IEC 8859-2 (Latin-2)
 *  5,      ISO/IEC 8859-3 (Latin-3)
 *  6,      ISO/IEC 8859-4 (Latin-4)
 *  7,      ISO/IEC 8859-5 (Latin/Cyrillic)
 *  8,      ISO/IEC 8859-6 (Latin/Arabic)
 *  9,      ISO/IEC 8859-7 (Latin/Greek)
 * 10,      ISO/IEC 8859-8 (Latin/Hebrew)
 * 11,      ISO/IEC 8859-9 (Latin-5)
 * 12,      ISO/IEC 8859-10 (Latin-6)
 * 13,      ISO/IEC 8859-11 (Latin/Thai)
 * 15,      ISO/IEC 8859-13 (Latin-7)
 * 16,      ISO/IEC 8859-14 (Latin-8 (Celtic))
 * 17,      ISO/IEC 8859-15 (Latin-9)
 * 18,      ISO/IEC 8859-16 (Latin-10)
 * 20,      Shift JIS
 * 21,      Windows-1250 (Superset of Latin-2)
 * 22,      Windows-1251 (Latin/Cyrillic)
 * 23,      Windows-1252 (Superset of Latin-1)
 * 24,      Windows-1256 (Arabic)
 * 25,      UTF-16 (Big endian)
 * 26,      UTF-8
 * 27,      US-ASCII
 * 28,      Big5
 * 29,      GB18030
 * 30       EUC-KR
 */
unsigned char QRCode::selectEci(const string &data) {
    for (int i=0; i<data.size(); ++i) {
        if (data[i]<0 || data[i]>127)
            /* UTF-8 characters set. */
            return 26;
    }
    /* Shift JIS characters set. */
    return 20;
}

/*
 * Analyse the data and select appropriate mode. Numeric mode
 * is for decimal digits 0 through 9. Alphanumeric mode is for
 * the decimal digits 0 through 9, as well as uppercase letters
 * (not lowercase!), and the symbols $, %, *, +, -, ., /, and :
 * as well as a space. Byte mode, by default, is for characters
 * from the ISO-8859-1 character set. Utf8 mode is for Chinese.
 */
QRCode::Mode QRCode::selectMode(const string &data, unsigned char eci) {
    int *count = new int[modeIndicator.size()]{0};
    for (int i=0; i<data.size(); ++i) {
        unsigned char val = data[i];
        if (val>=0x30 && val<=0x39) ++count[NUMERIC];
        else if ((val>=0x41 && val<=0x5a) || val==0x20 || val==0x24 \
                || val==0x25 || val==0x2a || val==0x2b || val==0x2d \
                || val==0x2e || val==0x2f || val==0x3a) ++count[ALPHANUMERIC];
        else if (eci==20 && i%2==0 && i+1<data.size() && \
                 ((data[i]>=0x81&&data[i]<=0x9f&&data[i+1]>=0x40&&data[i+1]<=0xfc) || \
                  (data[i]>=0xe0&&data[i]<=0xeb&&data[i+1]>=0x40&&data[i+1]<=0xbf)) \
                ) count[KANJI]+=2;
        else ++count[BYTE];
    }
    if (count[NUMERIC]==data.size()) return NUMERIC;
    else if (count[NUMERIC]+count[ALPHANUMERIC]==data.size()) return ALPHANUMERIC;
    else if (count[KANJI]==data.size()) return KANJI;
    else return BYTE;
}

/*
 * Select the smallest version for data by looking up the char capacity
 * table according the encoding mode and error correction level used.
 * There are forty versions for QR-code in total. Every version
 * coresponds to a size. The side length L of version V can be
 * caculated by following formula:
 *      L = ( V - 1 ) * 4 + 21
 * That is, the size of version 1 is 21*21. Every time the version
 * increases by 1, the side lenght of size increases by 4.
 */
unsigned char QRCode::selectVersion(const string &data, Mode mode, Level level) {
    int p = mode * modeIndicator.size() + level;
    for (int v=1; v<41; ++v) {
        if (charCapacity[v][p] > data.size()) {
            return v;
        }
    }
    printf("Error: the account of data is too large, which has exceeded "
           "the capacity that the QR-code with %s encoding mode and %s "
           "error correction level can store.\n", getMode(), getLevel());
    exit(1);
}

/*
 * Encode data and generate error correction codes.
 */
void QRCode::encodeData(Sequence &seq, const string &data) {
    seq = { {}, 0 };
    /* Add 4 bits of ECI mode indicator and 8 bits of ECI assignment number. */
    if (eci != 20) {
        encode(seq, 0x07, 4);
        encode(seq, eci, 8);
    }
    /* Add mode indicator. */
    encode(seq, modeIndicator[mode], 4);
    /* Add character count indicator. */
    unsigned char len = 0;
    if (version>=1 && version<=9) {
        len = countLength[0][mode];
    }
    else if (version>=10 && version<=26) {
        len = countLength[1][mode];
    }
    else if (version>=27 && version<=40) {
        len = countLength[2][mode];
    }
    else {
        printf("Error: the version %d is wrong.\n", version);
        exit(1);
    }
    encode(seq, data.size(), len);
    /* Encode data. */
    switch (mode)
    {
        case NUMERIC: numericEncoding(seq, data); break;
        case ALPHANUMERIC: alphanumericEncoding(seq, data); break;
        case BYTE: byteEncoding(seq, data); break;
        case KANJI: kanjiEncoding(seq, data); break;
        default: printf("Error: %s mode encoding fails.\n", getMode()); exit(1);
    }
    /* Add terminator and pad bytes. */
    /* The `total` is the total bits number of all codewords. */
    vector<unsigned short> codewords = codewordNumber[version][level];
    unsigned short totalBits = (codewords[1]*codewords[2] + codewords[3]*codewords[4]) * 8;
    if (totalBits-seq.length > 4) {
        encode(seq, 0, 4);
        if (seq.length%8) encode(seq, 0, 8-seq.length%8);
        while (seq.length < totalBits) {
            if (seq.length < totalBits) encode(seq, 0xec, 8);
            if (seq.length < totalBits) encode(seq, 0x11, 8);
        }
    }
    else encode(seq, 0, totalBits-seq.length);
    /* Generate error correction code. */
    vector<Polynomial> generator = { {1, 1}, {1, 0} };
    for (int i=1; i<codewords[0]; ++i) {
        generator = multiply(generator, { {1, 1}, {antilogTable[i], 0} });
    }
    unsigned short pos = 0;
    for (int i=0; i<codewords[1]+codewords[3]; ++i) {
        vector<Polynomial> message;
        unsigned short codewordNum;
        if (i<codewords[1]) codewordNum = codewords[2];
        else codewordNum = codewords[4];
        vector<unsigned char> dataBlock;
        vector<unsigned char> ecBlock;
        for (int j=0; j<codewordNum; ++j) {
            unsigned char val = decode(seq, pos, 8);
            dataBlock.push_back(val);
            if (val != 0) message.push_back({val, codewordNum-1+codewords[0]-j});
            pos += 8;
        }
        dataBlocks.push_back(dataBlock);
        vector<Polynomial> remainder = divide(message, generator);
        vector<Polynomial>::iterator iter = remainder.begin();
        for (int j=0; j<codewords[0]; ++j) {
            if (iter!=remainder.end() && iter->exponent==codewords[0]-j-1) {
                ecBlock.push_back(iter->coefficient);
                ++iter;
            }
            else ecBlock.push_back(0);
        }
        ecBlocks.push_back(ecBlock);
    }
    /* Interleave the blocks. */
    /* When the `k` is 0, encode the codewords of data blocks. */
    /* When the `k` is 1, encode the codewords of error correction blocks. */
    seq.sequence.clear();
    seq.length = 0;
    for (int k=0; k<2; ++k) {
        vector<vector<unsigned char>> *blocks;
        if (k) blocks = &ecBlocks;
        else blocks = &dataBlocks;
        for (int j=0; ; ++j) {
            unsigned char flag = 0;
            for (int i=0; i<(*blocks).size(); ++i) {
                if (j < (*blocks)[i].size()) {
                    encode(seq, (*blocks)[i][j], 8);
                    ++flag;
                }
            }
            if (flag == 0) break;
        }
    }
    /* Add remainder bits if necessary. */
    encode(seq, 0, remainderBit[version]);
}

/*
 * Draw patterns according to the encoded sequence.
 */
void QRCode::drawPattern(vector<vector<Color>> &pat, const Sequence &seq) {
    pat = vector<vector<Color>>(size, vector<Color>(size, LIGHT));
    /* Draw finder patterns. */
    static int xShift[4] = { 1, 0, -1, 0 };
    static int yShift[4] = { 0, 1, 0, -1 };
	vector<vector<int>> start = {{0,0,6}, {size-7,0,6}, {0,size-7,6},
                            {2,2,2}, {size-5,2,2}, {2,size-5,2}};
	for (int k=0; k<6; ++k) {
		int x=start[k][0], y=start[k][1];
        for (int i=0; i<4; ++i) {
            for (int j=0; j<start[k][2]; ++j) {
                pat[y][x] = DARK;
                x += xShift[i]; y += yShift[i];
            }
        }
	}
    pat[3][3] = DARK;
    pat[3][size-4] = DARK;
    pat[size-4][3] = DARK;
    /* Draw timing patterns. */
    for (int i=8; i<size-8; i+=2) {
        pat[6][i] = DARK;
        pat[i][6] = DARK;
    }
    /* Draw dark module. */
    pat[size-8][8] = DARK;
    /* Draw alignment patterns. */
    vector<unsigned char> centers = centerLocation[version];
    vector<Coordinate> centerList;
    for (int i=1; i<centers.size(); ++i) {
        for (int j=1; j<centers.size(); ++j) {
            centerList.push_back({centers[i], centers[j]});
        }
        if (i!=centers.size()-1) {
            centerList.push_back({6, centers[i]});
            centerList.push_back({centers[i], 6});
        }
    }
    for (int k=0; k<centerList.size(); ++k) {
		int x=centerList[k].xValue-2, y=centerList[k].yValue-2;
        for (int i=0; i<4; ++i) {
            for (int j=0; j<4; ++j) {
                pat[y][x] = DARK;
                x += xShift[i]; y += yShift[i];
            }
        }
        pat[centerList[k].xValue][centerList[k].yValue] = DARK;
	}
    /* Draw data. */
    int pos = 0;
    for (int i=size-1; i>=0; i-=2) {
        if (i == 6) --i;
        int direction=-1, begin=size-1;
        if (i>6 && i/2%2 == 1) { direction = 1; begin = 0; }
        if (i<6 && i/2%2 == 0) { direction = 1; begin = 0; }
        for (int j=begin; j>-1 && j<size; j+=direction) {
            /* Overleap the position of timing patterns. */
            if (j == 6) j += direction;
            for (int k=i; k>i-2; --k) {
                /* Overleap the position of version patterns. */
                if (version>6 && (j<6&&k>size-12 ||
                    j>size-12&&k<6)) continue; 
                /* Overleap the position of finder patterns and format patterns. */
                if ((j<9&&k<9) || (j<9&&k>size-9)
                    || (j>size-9&&k<9)) continue;
                bool flag = false;
                /* Overleap the position of alignment patterns. */
                for (int n=0; n<centerList.size(); ++n) {
                    if (k>centerList[n].xValue-3 && k<centerList[n].xValue+3 \
                        && j>centerList[n].yValue-3 && j<centerList[n].yValue+3) \
                        { flag=true; break;};
                }
                if (flag) continue;
                if (decode(seq, pos++, 1)) pat[j][k] = DARK;
            }
        }
    }
    /* Mask patterns */
    vector<vector<vector<Color>>> maskList(8, pat);
    for (int i=size-1; i>=0; i-=2) {
        if (i == 6) --i;
        int direction=-1, begin=size-1;
        if (i>6 && i/2%2 == 1) { direction = 1; begin = 0; }
        if (i<6 && i/2%2 == 0) { direction = 1; begin = 0; }
        for (int j=begin; j>-1 && j<size; j+=direction) {
            if (j == 6) j += direction;
            for (int k=i; k>i-2; --k) {
                if (version>6 && (j<6&&k>size-12 ||
                    j>size-12&&k<6)) continue; 
                if ((j<9&&k<9) || (j<9&&k>size-9)
                    || (j>size-9&&k<9)) continue;
                bool flag = false;
                for (int n=0; n<centerList.size(); ++n) {
                    if (k>centerList[n].xValue-3 && k<centerList[n].xValue+3 \
                        && j>centerList[n].yValue-3 && j<centerList[n].yValue+3)
                        { flag=true; break;};
                }
                if (flag) continue;
                if ((j+k)%2 == 0) maskList[0][j][k] = (maskList[0][j][k]==DARK)?LIGHT:DARK;
                if (j%2 == 0) maskList[1][j][k] = (maskList[1][j][k]==DARK)?LIGHT:DARK;
                if (k%3 == 0) maskList[2][j][k] = (maskList[2][j][k]==DARK)?LIGHT:DARK;
                if ((j+k)%3 == 0) maskList[3][j][k] = (maskList[3][j][k]==DARK)?LIGHT:DARK;
                if ((j/2+k/3)%2 == 0) maskList[4][j][k] = (maskList[4][j][k]==DARK)?LIGHT:DARK;
                if ((j*k)%2+(j*k)%3 == 0) maskList[5][j][k] = (maskList[5][j][k]==DARK)?LIGHT:DARK;
                if (((j*k)%2+(j*k)%3)%2 == 0) maskList[6][j][k] = (maskList[6][j][k]==DARK)?LIGHT:DARK;
                if (((j+k)%2+(j*k)%3)%2 == 0) maskList[7][j][k] = (maskList[7][j][k]==DARK)?LIGHT:DARK;
            }
        }
    }
    mask = Mask(0);
    unsigned int score = penalize(maskList[0]);
    for (int i=1; i<maskList.size(); ++i) {
        unsigned int temp = penalize(maskList[i]);
        if (score > temp) { score = temp; mask = Mask(i); }
    }
    pat = maskList[mask];
    /* Draw format patterns. */
    unsigned short formatBits = formatString[level][mask];
    for (int i=0; i<15; ++i) {
        if (formatBits&1 == 1) {
            if (i < 6) { 
                pat[i][8] = DARK;
                pat[8][size-1-i] = DARK;
            }
            else if (i < 8) {
                pat[i+1][8] = DARK;
                pat[8][size-1-i] = DARK;
            }
            else if (i < 9) {
                pat[8][7] = DARK;
                pat[size-7][8] = DARK;
            }
            else {
                pat[8][14-i] = DARK;
                pat[size-1-(14-i)][8] = DARK;
            }
        }
        formatBits >>= 1;
    }
    /* Draw version patterns. */
    if (version > 6) {
        unsigned int versionBits = versionString[version];
        for (int i=0; i<18; ++i) {
            if (versionBits&1 == 1) {
                pat[size-11+i%3][0+i/3] = DARK;
                pat[0+i/3][size-11+i%3] = DARK;
            }
            versionBits >>= 1;
        }
    }
}

QRCode::QRCode() {
    createQrcode("", Level::UNKNOWNLEVEL);
}

QRCode::QRCode(const string &data, const Level &level) {
    if (data.size() == 0) {
        printf("Error: the data to store is empty and can't create QR-code.\n");
        exit(1);
    }
    createQrcode(data, level);
}

QRCode::~QRCode() {

}

void QRCode::createQrcode(const string &data, const Level &level) {
    if (data.size()==0) {
        this->data = "";
        this->version = 0;
        this->size = 0;
        this->mode = Mode::UNKNOWNMODE;
        this->level = Level::UNKNOWNLEVEL;
        this->mask = Mask::UNKNOWNMASK;
    }
    this->data = data;
    this->level = level;
    this->eci = selectEci(data);
    this->mode = selectMode(data, eci);
    this->version = selectVersion(data, mode, level);
    this->size = (version-1)*4+21;
    encodeData(sequence, data);
    drawPattern(pattern, sequence);
}

unsigned char QRCode::getEci() {
    return eci;
}

unsigned char QRCode::getVersion() {
    return version;
}

unsigned char QRCode::getSize() {
    return size;
}

char* QRCode::getMode() {
    switch (mode)
    {
        case NUMERIC: return "NUMERIC";
        case ALPHANUMERIC: return "ALPHANUMERIC";
        case BYTE: return "BYTE";
        case KANJI: return "KANJI";
        default: return "UNKNOWN";
    }
}

char* QRCode::getLevel() {
    switch (level)
    {
        case LOW: return "LOW";
        case MIDDLE: return "MIDDLE";
        case QUITE: return "QUITE";
        case HIGH: return "HIGH";
        default: return "UNKNOWN";
    }
}

vector<vector<QRCode::Color>> QRCode::getPattern() {
    return pattern;
}