/* Copyright (c) 2010, 2017, Oracle and/or its affiliates. All rights reserved.

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; version 2 of the License.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Suite 500, Boston, MA 02110-1335 USA */

#ifndef TABLE_ETS_BY_HOST_BY_EVENT_NAME_H
#define TABLE_ETS_BY_HOST_BY_EVENT_NAME_H

/**
  @file storage/perfschema/table_ets_by_host_by_event_name.h
  Table EVENTS_TRANSACTIONS_SUMMARY_BY_HOST_BY_EVENT_NAME (declarations).
*/

#include "pfs_column_types.h"
#include "pfs_engine_table.h"
#include "pfs_instr_class.h"
#include "pfs_instr.h"
#include "pfs_host.h"
#include "table_helper.h"

/**
  @addtogroup performance_schema_tables
  @{
*/

class PFS_index_ets_by_host_by_event_name : public PFS_engine_index
{
public:
  PFS_index_ets_by_host_by_event_name()
    : PFS_engine_index(&m_key_1, &m_key_2),
      m_key_1("HOST"),
      m_key_2("EVENT_NAME")
  {
  }

  ~PFS_index_ets_by_host_by_event_name()
  {
  }

  virtual bool match(PFS_host *pfs);
  virtual bool match(PFS_instr_class *instr_class);

private:
  PFS_key_host m_key_1;
  PFS_key_event_name m_key_2;
};

/**
  A row of table
  PERFORMANCE_SCHEMA.EVENTS_TRANSACTIONS_SUMMARY_BY_HOST_BY_EVENT_NAME.
*/
struct row_ets_by_host_by_event_name
{
  /** Column HOST */
  PFS_host_row m_host;
  /** Column EVENT_NAME */
  PFS_event_name_row m_event_name;
  /**
    Columns COUNT_STAR, SUM/MIN/AVG/MAX_TIMER_WAIT,
    COUNT_READ_WRITE, SUM/MIN/AVG/MAX_TIMER_READ_WRITE,
    COUNT_READ_ONLY, SUM/MIN/AVG/MAX_TIMER_READ_ONLY
  */
  PFS_transaction_stat_row m_stat;
};

/**
  Position of a cursor on
  PERFORMANCE_SCHEMA.EVENTS_TRANSACTIONS_SUMMARY_BY_HOST_BY_EVENT_NAME.
  Index 1 on host (0 based)
  Index 2 on transaction class (1 based)
*/
struct pos_ets_by_host_by_event_name : public PFS_double_index
{
  pos_ets_by_host_by_event_name() : PFS_double_index(0, 1)
  {
  }

  inline void
  reset(void)
  {
    m_index_1 = 0;
    m_index_2 = 1;
  }

  inline void
  next_host(void)
  {
    m_index_1++;
    m_index_2 = 1;
  }
};

/** Table PERFORMANCE_SCHEMA.EVENTS_TRANSACTIONS_SUMMARY_BY_HOST_BY_EVENT_NAME.
 */
class table_ets_by_host_by_event_name : public PFS_engine_table
{
public:
  /** Table share */
  static PFS_engine_table_share m_share;
  static PFS_engine_table *create();
  static int delete_all_rows();
  static ha_rows get_row_count();

  virtual void reset_position(void);

  virtual int rnd_init(bool scan);
  virtual int rnd_next();
  virtual int rnd_pos(const void *pos);

  virtual int index_init(uint idx, bool sorted);
  virtual int index_next();

protected:
  virtual int read_row_values(TABLE *table,
                              unsigned char *buf,
                              Field **fields,
                              bool read_all);

  table_ets_by_host_by_event_name();

public:
  ~table_ets_by_host_by_event_name()
  {
  }

protected:
  int make_row(PFS_host *host, PFS_transaction_class *klass);

private:
  /** Table share lock. */
  static THR_LOCK m_table_lock;
  /** Fields definition. */
  static TABLE_FIELD_DEF m_field_def;

  /** Current row. */
  row_ets_by_host_by_event_name m_row;
  /** Current position. */
  pos_ets_by_host_by_event_name m_pos;
  /** Next position. */
  pos_ets_by_host_by_event_name m_next_pos;

  PFS_index_ets_by_host_by_event_name *m_opened_index;
};

/** @} */
#endif
