#include <stdio.h>
#include <string>

#include <dolphinconn/connection.h>
#include <dolphinconn/resultset.h>

#include <boost/scoped_ptr.hpp>

using std::string;

int main(int argc, const char** argv) {
  boost::scoped_ptr<dolphinconn::Connection> db(new dolphinconn::Connection);

  if (!db->open("test", "joe", "doe", std::string(),
                0, std::string())) {
    fprintf(stderr, "Could not connect!\n");
    return 1;
  }

  if (!db->execute("CREATE TABLE foo(bar INT, baz VARCHAR(5))")) {
    fprintf(stderr, "Cannot create table 'foo'\n");
    return 1;
  }

  string sql("INSERT INTO foo(bar, baz) VALUES (1, '");
  sql.append(db->escape("one\\n"));  // escape this
  sql.append("')");
  db->execute(sql);

  boost::scoped_ptr<dolphinconn::ResultSet> res(db->execute_query("SELECT "
        "bar, baz FROM foo"));
  if (res)
    while (res->step()) {
      fprintf(stderr, "%d: %s\n",
              res->column_int(0), res->column_string(1).c_str());
    }

  db->execute("DROP TABLE IF EXISTS foo");

  printf("done\n");
  return 0;
}
