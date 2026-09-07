/*
Project "Electronic schematic and pcb CAD"
Copyright (c) 2026 Alexander Sibilev

SPDX-License-Identifier: GPL-3.0-or-later

Author
  Alexander Sibilev S.

Web
  www.SalixEDA.org

Description
  Constants for AI agent server communication
*/
#ifndef SDAIAGENTDEF_H
#define SDAIAGENTDEF_H

//Fields of query
#define SDAI_TYPE                    0 //!< Type of query
#define SDAI_QUERY                   1 //!< Text of query
#define SDAI_ANSWER                  2 //!< Text of answer
#define SDAI_TASK_ID                 3 //!< Task id of query

//Types of query
#define SDAI_TYPE_FAIL               0
#define SDAI_TYPE_QUERY              1 //!< Query from user
#define SDAI_TYPE_POLL               2 //!< Check if task ready
#define SDAI_TYPE_BUSY               3 //!< Answer is not ready
#define SDAI_TYPE_ANSWER             4 //!< Answer ready


#define SD_AI_AGENT_PORT 1973

#endif // SDAIAGENTDEF_H
