/* vim:set ts=2 sw=2 et cindent: */
/*
 * Copyright (c) 2011 William Lima <wlima@primate.com.br>
 * All rights reserved.
 */

#ifndef DOLPHINCONN_RESULTSET_H_
#define DOLPHINCONN_RESULTSET_H_
#pragma once

#include <stdint.h>
#include <string>

#include <boost/noncopyable.hpp>
#include <mysql.h>

#include "dolphinconn/connection.h"

namespace dolphinconn {

class ResultSet : private boost::noncopyable {
 public:
  ResultSet(Connection* connection, MYSQL_RES* res);
  ~ResultSet();

  bool is_valid() const { return !!result_; }

  Connection* connection() const { return connection_; }

  bool step();

  void close();

  uint64_t row_count() const;

  // NOTE: index is 0 based

  int column_count() const;
  bool column_bool(int col) const;
  int column_int(int col) const;
  uint32_t column_uint(int col) const;
  int64_t column_int64(int col) const;
  uint64_t column_uint64(int col) const;
  double column_double(int col) const;
  std::string column_string(int col) const;

 private:
  Connection* connection_;
  MYSQL_ROW row_;
  MYSQL_RES* result_;
};

} // namespace dolphinconn

#endif // DOLPHINCONN_RESULTSET_H_
