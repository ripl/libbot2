///////////////////////////////////////////////////////////////////////////////
//
//  FILE: getopt.h
//
//      GetOption function declaration
//
//  FUNCTIONS:
//
//      GetOption() - Get next command line option and parameter
//
//  COMMENTS:
//
//      Copyright (C) 1992 - 1996 Microsoft Corporation
//
///////////////////////////////////////////////////////////////////////////////

#ifndef BOT2_LCM_UTILS_TUNNEL_LDPC_GETOPT_H_
#define BOT2_LCM_UTILS_TUNNEL_LDPC_GETOPT_H_

int GetOption(int argc, char** argv, char* pszValidOpts, char** ppszParam);

#endif  // BOT2_LCM_UTILS_TUNNEL_LDPC_GETOPT_H_
