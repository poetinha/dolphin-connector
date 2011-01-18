/* vim:set ts=2 sw=2 et cindent: */
/*
 * Copyright (c) 2011 William Lima <wlima@primate.com.br>
 * All rights reserved.
 */

#ifndef DOLPHINCONN_CONNECTION_H_
#define DOLPHINCONN_CONNECTION_H_
#pragma once

#include <stdint.h>
#include <string>

#include <boost/noncopyable.hpp>

struct st_mysql;

namespace dolphinconn {

class ResultSet;

class Connection : private boost::noncopyable {
 public:
  Connection();
  ~Connection();

  bool open(const std::string& db,
            const std::string& user,
            const std::string& password,
            const std::string& host,
            int port,
            const std::string& unix_socket);

  bool is_open() const { return !!db_; }

  void close();

  bool execute(const std::string& sql);

  ResultSet* execute_query(const std::string& sql);

  uint64_t get_last_insert_id() const;

  int get_last_errno() const;
  const char* get_sqlstate() const;
  const char* get_error_msg() const;

  std::string escape(const char* text) const;
  std::string escape(const std::string& text) const;

 private:
  void initialize();

  st_mysql* db_;
};

} // namespace dolphinconn

#endif // DOLPHINCONN_CONNECTION_H_
