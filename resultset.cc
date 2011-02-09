/* vim:set ts=2 sw=2 et cindent: */
/*
 * Copyright (c) 2011 William Lima <wlima@primate.com.br>
 * All rights reserved.
 */

#include "dolphinconn/resultset.h"

#include <cstdlib>

namespace dolphinconn {

ResultSet::ResultSet(Connection* connection, MYSQL_RES* res)
    : connection_(connection),
      result_(res) {
}

ResultSet::~ResultSet() {
  close();
}

bool ResultSet::step() {
  if (!is_valid())
    return false;
  row_ = mysql_fetch_row(result_);
  return row_ != NULL;
}

void ResultSet::close() {
  if (is_valid()) {
    mysql_free_result(result_);
    result_ = NULL;
  }
  connection_ = NULL;
}

uint64_t ResultSet::row_count() const {
  if (!is_valid())
    return 0;
  return mysql_num_rows(result_);
}

int ResultSet::column_count() const {
  if (!is_valid())
    return 0;
  return mysql_num_fields(result_);
}

bool ResultSet::column_bool(int col) const {
  return !!column_int(col);
}

int ResultSet::column_int(int col) const {
  if (!is_valid())
    return 0;
  return static_cast<int>(strtol(row_[col], NULL, 10));
}

uint32_t ResultSet::column_uint(int col) const {
  if (!is_valid())
    return 0;
  return static_cast<uint32_t>(strtoul(row_[col], NULL, 10));
}

int64_t ResultSet::column_int64(int col) const {
  if (!is_valid())
    return 0;
  return strtoll(row_[col], NULL, 10);
}

uint64_t ResultSet::column_uint64(int col) const {
  if (!is_valid())
    return 0;
  return strtoull(row_[col], NULL, 10);
}

double ResultSet::column_double(int col) const {
  if (!is_valid())
    return 0;
  return strtod(row_[col], NULL);
}

std::string ResultSet::column_string(int col) const {
  if (!is_valid())
    return "";
  const char* str = reinterpret_cast<const char*>(row_[col]);
  size_t len = mysql_fetch_lengths(result_)[col];

  std::string result;
  if (str && len > 0)
    result.assign(str, len);
  return result;
}

} // namespace dolphinconn
