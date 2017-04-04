

#include "mytype.h"
#include "NxpMcu.h"


// Used for LPC11xx devices
static const unsigned int SectorTable_11xx[] =
{
    4096,  4096,  4096,  4096,  4096,  4096,  4096,  4096,
    4096,  4096,  4096,  4096,  4096,  4096,  4096,  4096,
    4096,  4096,  4096,  4096,  4096,  4096,  4096,  4096,
    4096,  4096,  4096,  4096,  4096,  4096,  4096,  4096
};

// Used for LPC17xx devices
static const unsigned int SectorTable_17xx[] =
{
    4096,  4096,  4096,  4096,  4096,  4096,  4096,  4096,
    4096,  4096,  4096,  4096,  4096,  4096,  4096,  4096,
    32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768,
    32768, 32768, 32768, 32768, 32768, 32768
};

// Used for LPC18xx devices
static const unsigned int SectorTable_18xx[] =
{
    8192,  8192,  8192,  8192,  8192,  8192,  8192,  8192,
    65536, 65536, 65536, 65536, 65536, 65536, 65536
};

// Used for LPC43xx devices
static const unsigned int SectorTable_43xx[] =
{
    8192,  8192,  8192,  8192,  8192,  8192,  8192,  8192,
    65536, 65536, 65536, 65536, 65536, 65536, 65536
};

// Used for LPC8xx devices
static const unsigned int SectorTable_8xx[] =
{
    1024,  1024,  1024,  1024,  1024,  1024,  1024,  1024,
    1024,  1024,  1024,  1024,  1024,  1024,  1024,  1024
};

const 
LPC_DEVICE_TYPE LPCtypes[] =
{
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, CHIP_VARIANT_NONE },  /* unknown */

    // id,        id2,  use id2, name of product,          flash size, ram size, total number of sector, max copy size, sector table, chip variant

    { 0x00008100, 0x00000000, 0, "810M021FN8",                      4,   1,  4,  256, SectorTable_8xx,  CHIP_VARIANT_LPC8XX  },
    { 0x00008110, 0x00000000, 0, "811M001FDH16",                    8,   2,  8, 1024, SectorTable_8xx,  CHIP_VARIANT_LPC8XX  },
    { 0x00008120, 0x00000000, 0, "812M101FDH16",                   16,   4, 16, 1024, SectorTable_8xx,  CHIP_VARIANT_LPC8XX  },
    { 0x00008121, 0x00000000, 0, "812M101FD20",                    16,   4, 16, 1024, SectorTable_8xx,  CHIP_VARIANT_LPC8XX  },
    { 0x00008122, 0x00000000, 0, "812M101FDH20",                   16,   4, 16, 1024, SectorTable_8xx,  CHIP_VARIANT_LPC8XX  },

    { 0x2500102B, 0x00000000, 0, "1102",                           32,   8,  8, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX },

    { 0x0A07102B, 0x00000000, 0, "1110.../002",                     4,   1,  1, 1024, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
    { 0x1A07102B, 0x00000000, 0, "1110.../002",                     4,   1,  1, 1024, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
    { 0x0A16D02B, 0x00000000, 0, "1111.../002",                     8,   2,  2, 1024, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
    { 0x1A16D02B, 0x00000000, 0, "1111.../002",                     8,   2,  2, 1024, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
    { 0x041E502B, 0x00000000, 0, "1111.../101",                     8,   2,  2, 1024, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
    { 0x2516D02B, 0x00000000, 0, "1111.../102",                     8,   2,  2, 1024, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
    { 0x00010013, 0x00000000, 0, "1111.../103",                     8,   2,  2, 1024, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
    { 0x0416502B, 0x00000000, 0, "1111.../201",                     8,   4,  2, 1024, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
    { 0x2516902B, 0x00000000, 0, "1111.../202",                     8,   4,  2, 1024, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
    { 0x00010012, 0x00000000, 0, "1111.../203",                     8,   4,  2, 1024, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
    { 0x042D502B, 0x00000000, 0, "1112.../101",                    16,   2,  4, 1024, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
    { 0x2524D02B, 0x00000000, 0, "1112.../102",                    16,   2,  4, 1024, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
    { 0x0A24902B, 0x00000000, 0, "1112.../102",                    16,   4,  4, 1024, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
    { 0x1A24902B, 0x00000000, 0, "1112.../102",                    16,   4,  4, 1024, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
    { 0x00020023, 0x00000000, 0, "1112.../103",                    16,   2,  4, 1024, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
    { 0x0425502B, 0x00000000, 0, "1112.../201",                    16,   4,  4, 1024, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
    { 0x2524902B, 0x00000000, 0, "1112.../202",                    16,   4,  4, 1024, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
    { 0x00020022, 0x00000000, 0, "1112.../203",                    16,   4,  4, 1024, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
    { 0x0434502B, 0x00000000, 0, "1113.../201",                    24,   4,  6, 1024, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
    { 0x2532902B, 0x00000000, 0, "1113.../202",                    24,   4,  6, 1024, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
    { 0x00030032, 0x00000000, 0, "1113.../203",                    24,   4,  6, 1024, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
    { 0x0434102B, 0x00000000, 0, "1113.../301",                    24,   8,  6, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
    { 0x2532102B, 0x00000000, 0, "1113.../302",                    24,   8,  6, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
    { 0x00030030, 0x00000000, 0, "1113.../303",                    24,   8,  6, 1024, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
    { 0x0A40902B, 0x00000000, 0, "1114.../102",                    32,   4,  8, 1024, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
    { 0x1A40902B, 0x00000000, 0, "1114.../102",                    32,   4,  8, 1024, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
    { 0x0444502B, 0x00000000, 0, "1114.../201",                    32,   4,  8, 1024, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
    { 0x2540902B, 0x00000000, 0, "1114.../202",                    32,   4,  8, 1024, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
    { 0x00040042, 0x00000000, 0, "1114.../203",                    32,   8,  8, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
    { 0x0444102B, 0x00000000, 0, "1114.../301",                    32,   8,  8, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
    { 0x2540102B, 0x00000000, 0, "1114.../302",                    32,   8,  8, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
    { 0x00040040, 0x00000000, 0, "1114.../303",                    32,   8,  8, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
    { 0x00040060, 0x00000000, 0, "1114.../323",                    32,   8, 12, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
    { 0x00040070, 0x00000000, 0, "1114.../333",                    32,   8, 14, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
    { 0x00050080, 0x00000000, 0, "1115.../303",                    64,   8, 16, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX },

    { 0x1421102B, 0x00000000, 0, "11C12.../301",                   16,   8,  4, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
    { 0x1440102B, 0x00000000, 0, "11C14.../301",                   32,   8,  8, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
    { 0x1431102B, 0x00000000, 0, "11C22.../301",                   16,   8,  4, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX },
    { 0x1430102B, 0x00000000, 0, "11C24.../301",                   32,   8,  8, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX },

    { 0x293E902B, 0x00000000, 0, "11E11FHN33/101",                  8,   4,  2, 1024, SectorTable_11xx, CHIP_VARIANT_LPC11XX }, /* From UM10518 Rev. 3 -- 25 Nov 2013 */
    { 0x2954502B, 0x00000000, 0, "11E12FBD48/201",                 16,   6,  4, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX }, /* From UM10518 Rev. 3 -- 25 Nov 2013 */
    { 0x296A102B, 0x00000000, 0, "11E13FBD48/301",                 24,   8,  6, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX }, /* From UM10518 Rev. 3 -- 25 Nov 2013 */
    { 0x2980102B, 0x00000000, 0, "11E14(FHN33,FBD48,FBD64)/401",   32,  10,  8, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX }, /* From UM10518 Rev. 3 -- 25 Nov 2013 */
    { 0x00009C41, 0x00000000, 0, "11E36(FBD64,FHN33)/501",         96,  12, 24, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX }, /* From UM10518 Rev. 3 -- 25 Nov 2013 */
    { 0x00007C45, 0x00000000, 0, "11E37HFBD64/401",               128,  10, 32, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX }, /* From UM10518 Rev. 3 -- 25 Nov 2013 */
    { 0x00007C41, 0x00000000, 0, "11E37(FBD48,FBD64)/501",        128,  12, 32, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX }, /* From UM10518 Rev. 3 -- 25 Nov 2013 */

    { 0x095C802B, 0x00000000, 0, "11U12(FHN33,FBD48)/201",         16,   6,  4, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX }, /* From UM10462 Rev. 5 -- 20 Nov 2013 */
    { 0x295C802B, 0x00000000, 0, "11U12(FHN33,FBD48)/201",         16,   6,  4, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX }, /* From UM10462 Rev. 5 -- 20 Nov 2013 */
    { 0x097A802B, 0x00000000, 0, "11U13FBD48/201",                 24,   6,  6, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX }, /* From UM10462 Rev. 5 -- 20 Nov 2013 */
    { 0x297A802B, 0x00000000, 0, "11U13FBD48/201",                 24,   6,  6, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX }, /* From UM10462 Rev. 5 -- 20 Nov 2013 */
    { 0x0998802B, 0x00000000, 0, "11U14FHN33/201",                 32,   6,  8, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX }, /* From UM10462 Rev. 5 -- 20 Nov 2013 */
    { 0x2998802B, 0x00000000, 0, "11U14(FHN,FHI)33/201",           32,   6,  8, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX }, /* From UM10462 Rev. 5 -- 20 Nov 2013 */
    { 0x0998802B, 0x00000000, 0, "11U14(FBD,FET)48/201",           32,   6,  8, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX }, /* From UM10462 Rev. 5 -- 20 Nov 2013 */
    { 0x2998802B, 0x00000000, 0, "11U14(FBD,FET)48/201",           32,   6,  8, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX }, /* From UM10462 Rev. 5 -- 20 Nov 2013 */
    { 0x2972402B, 0x00000000, 0, "11U23FBD48/301",                 24,   8,  6, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX }, /* From UM10462 Rev. 5 -- 20 Nov 2013 */
    { 0x2988402B, 0x00000000, 0, "11U24(FHI33,FBD48,FET48)/301",   32,   8,  8, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX }, /* From UM10462 Rev. 5 -- 20 Nov 2013 */
    { 0x2980002B, 0x00000000, 0, "11U24(FHN33,FBD48,FBD64)/401",   32,  10,  8, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX }, /* From UM10462 Rev. 5 -- 20 Nov 2013 */
    { 0x0003D440, 0x00000000, 0, "11U34(FHN33,FBD48)/311",         40,   8, 10, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX }, /* From UM10462 Rev. 5 -- 20 Nov 2013 */
    { 0x0001CC40, 0x00000000, 0, "11U34(FHN33,FBD48)/421",         48,  10, 12, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX }, /* From UM10462 Rev. 5 -- 20 Nov 2013 */
    { 0x0001BC40, 0x00000000, 0, "11U35(FHN33,FBD48,FBD64)/401",   64,  10, 16, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX }, /* From UM10462 Rev. 5 -- 20 Nov 2013 */
    { 0x0000BC40, 0x00000000, 0, "11U35(FHI33,FET48)/501",         64,  12, 16, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX }, /* From UM10462 Rev. 5 -- 20 Nov 2013 */
    { 0x00019C40, 0x00000000, 0, "11U36(FBD48,FBD64)/401",         96,  10, 24, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX }, /* From UM10462 Rev. 5 -- 20 Nov 2013 */
    { 0x00017C40, 0x00000000, 0, "11U37FBD48/401",                128,  10, 32, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX }, /* From UM10462 Rev. 5 -- 20 Nov 2013 */
    { 0x00007C44, 0x00000000, 0, "11U37HFBD64/401",               128,  10, 32, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX }, /* From UM10462 Rev. 5 -- 20 Nov 2013 */
    { 0x00007C40, 0x00000000, 0, "11U37FBD64/501",                128,  12, 32, 4096, SectorTable_11xx, CHIP_VARIANT_LPC11XX }, /* From UM10462 Rev. 5 -- 20 Nov 2013 */

    { 0x3640C02B, 0x00000000, 0, "1224.../101",                    32,   8,  4, 2048, SectorTable_17xx, CHIP_VARIANT_LPC11XX },
    { 0x3642C02B, 0x00000000, 0, "1224.../121",                    48,  12, 32, 4096, SectorTable_17xx, CHIP_VARIANT_LPC11XX },
    { 0x3650002B, 0x00000000, 0, "1225.../301",                    64,  16, 32, 4096, SectorTable_17xx, CHIP_VARIANT_LPC11XX },
    { 0x3652002B, 0x00000000, 0, "1225.../321",                    80,  20, 32, 4096, SectorTable_17xx, CHIP_VARIANT_LPC11XX },
    { 0x3660002B, 0x00000000, 0, "1226",                           96,  24, 32, 4096, SectorTable_17xx, CHIP_VARIANT_LPC11XX },
    { 0x3670002B, 0x00000000, 0, "1227",                          128,  32, 32, 4096, SectorTable_17xx, CHIP_VARIANT_LPC11XX },

    { 0x2C42502B, 0x00000000, 0, "1311",                            8,   4,  2, 1024, SectorTable_17xx, CHIP_VARIANT_LPC13XX },
    { 0x1816902B, 0x00000000, 0, "1311/01",                         8,   4,  2, 1024, SectorTable_17xx, CHIP_VARIANT_LPC13XX },
    { 0x2C40102B, 0x00000000, 0, "1313",                           32,   8,  8, 4096, SectorTable_17xx, CHIP_VARIANT_LPC13XX },
    { 0x1830102B, 0x00000000, 0, "1313/01",                        32,   8,  8, 4096, SectorTable_17xx, CHIP_VARIANT_LPC13XX },
    { 0x3A010523, 0x00000000, 0, "1315",                           32,   8,  8, 4096, SectorTable_17xx, CHIP_VARIANT_LPC13XX },
    { 0x1A018524, 0x00000000, 0, "1316",                           48,   8, 12, 4096, SectorTable_17xx, CHIP_VARIANT_LPC13XX },
    { 0x1A020525, 0x00000000, 0, "1317",                           64,   8, 16, 4096, SectorTable_17xx, CHIP_VARIANT_LPC13XX },
    { 0x3D01402B, 0x00000000, 0, "1342",                           16,   4,  4, 1024, SectorTable_17xx, CHIP_VARIANT_LPC13XX },
    { 0x3D00002B, 0x00000000, 0, "1343",                           32,   8,  8, 4096, SectorTable_17xx, CHIP_VARIANT_LPC13XX },
    { 0x28010541, 0x00000000, 0, "1345",                           32,   8,  8, 4096, SectorTable_17xx, CHIP_VARIANT_LPC13XX },
    { 0x08018542, 0x00000000, 0, "1346",                           48,   8, 12, 4096, SectorTable_17xx, CHIP_VARIANT_LPC13XX },
    { 0x08020543, 0x00000000, 0, "1347",                           64,   8, 16, 4096, SectorTable_17xx, CHIP_VARIANT_LPC13XX },

    { 0x25001118, 0x00000000, 0, "1751",                           32,   8,  8, 4096, SectorTable_17xx, CHIP_VARIANT_LPC17XX },
    { 0x25001121, 0x00000000, 0, "1752",                           64,  16, 16, 4096, SectorTable_17xx, CHIP_VARIANT_LPC17XX },
    { 0x25011722, 0x00000000, 0, "1754",                          128,  32, 18, 4096, SectorTable_17xx, CHIP_VARIANT_LPC17XX },
    { 0x25011723, 0x00000000, 0, "1756",                          256,  32, 22, 4096, SectorTable_17xx, CHIP_VARIANT_LPC17XX },
    { 0x25013F37, 0x00000000, 0, "1758",                          512,  64, 30, 4096, SectorTable_17xx, CHIP_VARIANT_LPC17XX },
    { 0x25113737, 0x00000000, 0, "1759",                          512,  64, 30, 4096, SectorTable_17xx, CHIP_VARIANT_LPC17XX },
    { 0x26012033, 0x00000000, 0, "1763",                          256,  64, 22, 4096, SectorTable_17xx, CHIP_VARIANT_LPC17XX },
    { 0x26011922, 0x00000000, 0, "1764",                          128,  32, 18, 4096, SectorTable_17xx, CHIP_VARIANT_LPC17XX },
    { 0x26013733, 0x00000000, 0, "1765",                          256,  64, 22, 4096, SectorTable_17xx, CHIP_VARIANT_LPC17XX },
    { 0x26013F33, 0x00000000, 0, "1766",                          256,  64, 22, 4096, SectorTable_17xx, CHIP_VARIANT_LPC17XX },
    { 0x26012837, 0x00000000, 0, "1767",                          512,  64, 30, 4096, SectorTable_17xx, CHIP_VARIANT_LPC17XX },
    { 0x26013F37, 0x00000000, 0, "1768",                          512,  64, 30, 4096, SectorTable_17xx, CHIP_VARIANT_LPC17XX },
    { 0x26113F37, 0x00000000, 0, "1769",                          512,  64, 30, 4096, SectorTable_17xx, CHIP_VARIANT_LPC17XX },

    { 0x27011132, 0x00000000, 0, "1774",                          128,  40, 18, 4096, SectorTable_17xx, CHIP_VARIANT_LPC17XX },
    { 0x27191F43, 0x00000000, 0, "1776",                          256,  80, 22, 4096, SectorTable_17xx, CHIP_VARIANT_LPC17XX },
    { 0x27193747, 0x00000000, 0, "1777",                          512,  96, 30, 4096, SectorTable_17xx, CHIP_VARIANT_LPC17XX },
    { 0x27193F47, 0x00000000, 0, "1778",                          512,  96, 30, 4096, SectorTable_17xx, CHIP_VARIANT_LPC17XX },
    { 0x281D1743, 0x00000000, 0, "1785",                          256,  80, 22, 4096, SectorTable_17xx, CHIP_VARIANT_LPC17XX },
    { 0x281D1F43, 0x00000000, 0, "1786",                          256,  80, 22, 4096, SectorTable_17xx, CHIP_VARIANT_LPC17XX },
    { 0x281D3747, 0x00000000, 0, "1787",                          512,  96, 30, 4096, SectorTable_17xx, CHIP_VARIANT_LPC17XX },
    { 0x281D3F47, 0x00000000, 0, "1788",                          512,  96, 30, 4096, SectorTable_17xx, CHIP_VARIANT_LPC17XX },

    // LPC18xx
    { 0xF00B1B3F, 0x00000000, 1, "1810",                            0,  32,  0, 8192, SectorTable_18xx, CHIP_VARIANT_LPC18XX }, // Flashless
    { 0xF001D830, 0x00000000, 1, "1812",                          512,  32, 15, 8192, SectorTable_18xx, CHIP_VARIANT_LPC18XX },
    { 0xF001D830, 0x00000000, 1, "1813",                          512,  32, 11, 8192, SectorTable_18xx, CHIP_VARIANT_LPC18XX },
    { 0xF001D830, 0x00000000, 1, "1815",                          768,  32, 13, 8192, SectorTable_18xx, CHIP_VARIANT_LPC18XX },
    { 0xF001D830, 0x00000000, 1, "1817",                         1024,  32, 15, 8192, SectorTable_18xx, CHIP_VARIANT_LPC18XX },
    { 0xF00A9B3C, 0x00000000, 1, "1820",                            0,  32,  0, 8192, SectorTable_18xx, CHIP_VARIANT_LPC18XX }, // Flashless
    { 0xF001D830, 0x00000000, 1, "1822",                          512,  32, 15, 8192, SectorTable_18xx, CHIP_VARIANT_LPC18XX },
    { 0xF001D830, 0x00000000, 1, "1823",                          512,  32, 11, 8192, SectorTable_18xx, CHIP_VARIANT_LPC18XX },
    { 0xF001D830, 0x00000000, 1, "1825",                          768,  32, 13, 8192, SectorTable_18xx, CHIP_VARIANT_LPC18XX },
    { 0xF001D830, 0x00000000, 1, "1827",                         1024,  32, 15, 8192, SectorTable_18xx, CHIP_VARIANT_LPC18XX },
    { 0xF0009A30, 0x00000000, 1, "1830",                            0,  32,  0, 8192, SectorTable_18xx, CHIP_VARIANT_LPC18XX }, // Flashless
    { 0xF001DA30, 0x00000044, 1, "1833",                          512,  32, 11, 8192, SectorTable_18xx, CHIP_VARIANT_LPC18XX },
    { 0xF001DA30, 0x00000000, 1, "1837",                         1024,  32, 15, 8192, SectorTable_18xx, CHIP_VARIANT_LPC18XX },
    { 0xF0009830, 0x00000000, 1, "1850",                            0,  32,  0, 8192, SectorTable_18xx, CHIP_VARIANT_LPC18XX }, // Flashless
    { 0xF001D830, 0x00000044, 1, "1853",                          512,  32, 11, 8192, SectorTable_18xx, CHIP_VARIANT_LPC18XX },
    { 0xF001D830, 0x00000000, 1, "1857",                         1024,  32, 15, 8192, SectorTable_18xx, CHIP_VARIANT_LPC18XX },
};

unsigned long ReturnValueLpcRamStartExt(const LPC_DEVICE_TYPE *pLPC)
{
    if(pLPC->ChipVariant == CHIP_VARIANT_LPC43XX)
    {
        return LPC_RAMSTART_LPC43XX;
    }
    else if(pLPC->ChipVariant == CHIP_VARIANT_LPC2XXX)
    {
        return LPC_RAMSTART_LPC2XXX;
    }
    else if(pLPC->ChipVariant == CHIP_VARIANT_LPC18XX)
    {
        return LPC_RAMSTART_LPC18XX;
    }
    else if(pLPC->ChipVariant == CHIP_VARIANT_LPC17XX)
    {
        return LPC_RAMSTART_LPC17XX;
    }
    else if(pLPC->ChipVariant == CHIP_VARIANT_LPC13XX)
    {
        return LPC_RAMSTART_LPC13XX;
    }
    else if(pLPC->ChipVariant == CHIP_VARIANT_LPC11XX)
    {
        return LPC_RAMSTART_LPC11XX;
    }
    else if(pLPC->ChipVariant == CHIP_VARIANT_LPC8XX)
    {
        return LPC_RAMSTART_LPC8XX;
    }
    //DebugPrintf(1, "Error in ReturnValueLpcRamStart (%d)\n", pLPC->ChipVariant);
    return -1;
}


unsigned long ReturnValueLpcRamBaseExt( const LPC_DEVICE_TYPE *pLPC )
{
    if(pLPC->ChipVariant == CHIP_VARIANT_LPC43XX)
    {
        return LPC_RAMBASE_LPC43XX;
    }
    else if(pLPC->ChipVariant == CHIP_VARIANT_LPC2XXX)
    {
        return LPC_RAMBASE_LPC2XXX;
    }
    else if(pLPC->ChipVariant == CHIP_VARIANT_LPC18XX)
    {
        return LPC_RAMBASE_LPC18XX;
    }
    else if(pLPC->ChipVariant == CHIP_VARIANT_LPC17XX)
    {
        return LPC_RAMBASE_LPC17XX;
    }
    else if(pLPC->ChipVariant == CHIP_VARIANT_LPC13XX)
    {
        return LPC_RAMBASE_LPC13XX;
    }
    else if(pLPC->ChipVariant == CHIP_VARIANT_LPC11XX)
    {
        return LPC_RAMBASE_LPC11XX;
    }
    else if(pLPC->ChipVariant == CHIP_VARIANT_LPC8XX)
    {
        return LPC_RAMBASE_LPC8XX;
    }
    //DebugPrintf(1, "Error in ReturnValueLpcRamBase (%d)\n", pLPC->ChipVariant);
    return -1;
}

const LPC_DEVICE_TYPE *FindID( unsigned long id )
{
    int size = ARRAY_SIZE( LPCtypes);
    for(int i=0; i< size; i++)
    {
        if( LPCtypes[i].id == id )
        {
            return &LPCtypes[i];
        }
    }
    return NULL;
}


const LPC_DEVICE_TYPE *FindIDExt( unsigned long id, unsigned long id2 )
{
    int size = ARRAY_SIZE( LPCtypes);
    for(int i=0; i< size; i++)
    {
        if( LPCtypes[i].id == id && LPCtypes[i].EvalId2 == id2 )
        {
            return &LPCtypes[i];
        }
    }
    return NULL;
}



unsigned long GetSectorStart(  const LPC_DEVICE_TYPE *pLPC, int sector )
{  //获取页文件flash地址
    int start = 0;
    int i;
    for( i=0; i<sector; i++)
    {
        start += pLPC->SectorTable[i];
    }
    return start;
}

unsigned long NxpGetSector( const LPC_DEVICE_TYPE *pLPC, unsigned long addr )
{
    int sector = pLPC->FlashSectors;
    unsigned long start = 0;
    int i;
    for( i=0; i<sector-1; i++)
    {
        start += pLPC->SectorTable[i];
        if( addr < start )
            return i;
    }
    return sector-1;
}


int NxpSetBinCRC( const LPC_DEVICE_TYPE *pLPC, unsigned char *buff  )
{
    DWORD ivt_CRC = 0;
    int i;
    int pos;

    if( pLPC->ChipVariant == CHIP_VARIANT_LPC2XXX)
    {
        pos = 0x14;
    }else if(pLPC->ChipVariant == CHIP_VARIANT_LPC43XX ||
            pLPC->ChipVariant == CHIP_VARIANT_LPC18XX ||
            pLPC->ChipVariant == CHIP_VARIANT_LPC17XX ||
            pLPC->ChipVariant == CHIP_VARIANT_LPC13XX ||
            pLPC->ChipVariant == CHIP_VARIANT_LPC11XX ||
            pLPC->ChipVariant == CHIP_VARIANT_LPC8XX)
    {
        pos = 0x1C;
    }else
    {
        //DebugPrintf(1, "Internal error: wrong chip variant %d\n", pLPC->ChipVariant);
        return 1;
    }

    for (i = 0; i < 4; i++)
    {
        buff[i + pos] = 0;
    }

    // Calculate a native checksum of the little endian vector table:
    for (i = 0; i < (4 * 8);) {
        ivt_CRC += buff[i++];
        ivt_CRC += buff[i++] << 8;
        ivt_CRC += buff[i++] << 16;
        ivt_CRC += buff[i++] << 24;
    }

    /* Negate the result and place in the vector at 0x14 as little endian
     * again. The resulting vector table should checksum to 0. */
    ivt_CRC = (unsigned long) (0 - ivt_CRC);
    //DebugPrintf(3, "Position 0x%x patched: ivt_CRC = 0x%08lX\n", pos, ivt_CRC);
    for (i = 0; i < 4; i++)
    {
        buff[i + pos] = (unsigned char)ivt_CRC;
        ivt_CRC >>= 8;
    }
    return 0;
}

