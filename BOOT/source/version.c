/**
 * @file TODO
 * @brief TODO
 * @author Takashi Kashiwagi
 * @date        2018/10/23
 * @version     0.1
 * @copyright   TODO
 * 
 * @par Update:
 * - 2018/10/23: Takashi Kashiwagi: for IMXRT1060-EVK
 */
#include "version.h"

#define DEF_PROGRAM "MIMXRT1010-EVK_BOOT"
#define DEF_VERSION "Ver 0.1.0 "

const char g_szProgram[] = {DEF_PROGRAM};
const char g_szVersion[] = {DEF_VERSION};	/* Major.Minor.Bugfix */
const char g_szProgramVersion[] = {DEF_PROGRAM " " DEF_VERSION __DATE__ " " __TIME__};

