/* vim:set ts=2 sw=2 et cindent: */
/*
 * Copyright (c) 2011 William Lima <wlima@primate.com.br>
 * All rights reserved.
 */

#include "dolphinconn/connection.h"

#include <cstring>

#include <boost/scoped_array.hpp>
#include <errmsg.h>
#include <mysql.h>

#include "dolphinconn/resultset.h"

static int instance_count = 0;

namespace dolphinconn {

Connection::Connection()
    : db_(NULL) {
  instance_count++;
  initialize();
}

void Connection::initialize() {
  static bool initialize_called = false;

  if (initialize_called)
    return;

  mysql_library_init(0, NULL, NULL);
  initialize_called = true;
}

Connection::~Connection() {
  close();
  instance_count--;
  if (instance_count == 0)
    mysql_library_end();
}

bool Connection::open(const std::string& db,
                      const std::string& user,
                      const std::string& password,
                      const std::string& host,
                      int port,
                      const std::string& unix_socket) {
  if (db_)
    close();

  unsigned int flags = CLIENT_MULTI_RESULTS;
  my_bool reconnect = true;

  if (!(db_ = mysql_init(NULL)))
    return false;

  mysql_options(db_, MYSQL_OPT_RECONNECT, &reconnect);

  if (!mysql_real_connect(db_,
                          host.empty() ? static_cast<const char *>(0)
                                       : host.c_str(),
                          user.empty() ? static_cast<const char *>(0)
                                       : user.c_str(),
                          password.empty() ? static_cast<const char *>(0)
                                           : password.c_str(),
                          db.empty() ? static_cast<const char *>(0)
                                     : db.c_str(),
                          (port > -1) ? port : 0,
                          unix_socket.empty() ? static_cast<const char *>(0)
                                              : unix_socket.c_str(),
                          flags)) {
    return false;
  }

  // force the communication to be utf8
  mysql_set_character_set(db_, "utf8");

  return true;
}

void Connection::close() {
  if (db_) {
    mysql_thread_end();
    mysql_close(db_);
    db_ = NULL;
  }
}

bool Connection::execute(const std::string& sql) {
  if (!db_)
    return false;
  return mysql_real_query(db_, sql.c_str(), sql.length()) == 0;
}

ResultSet* Connection::execute_query(const std::string& sql) {
  if (!db_)
    return new ResultSet(this, NULL);

  if (mysql_real_query(db_, sql.c_str(), sql.length())) {
    return new ResultSet(this, NULL);
  }
  MYSQL_RES* result = mysql_store_result(db_);
  if (!result) {
    return new ResultSet(this, NULL);
  }

  return new ResultSet(this, result);
}

uint64_t Connection::get_last_insert_id() const {
  if (!db_)
    return 0;
  return mysql_insert_id(db_);
}

std::string Connection::escape(const char* text) const {
  const size_t len = strlen(text);
  boost::scoped_array<char> buffer(new char[len * 2 + 1]);
  mysql_real_escape_string(db_, buffer.get(), text, len);
  return std::string(buffer.get());
}

std::string Connection::escape(const std::string& text) const {
  boost::scoped_array<char> buffer(new char[text.length() * 2 + 1]);
  mysql_real_escape_string(db_, buffer.get(), text.c_str(), text.size());
  return std::string(buffer.get());
}

} // namespace dolphinconn
