#pragma once

#include "common/rc.h"
#include "sql/stmt/stmt.h"
#include "sql/parser/parse_defs.h"
#include "sql/stmt/filter_stmt.h"

class Table;

/**
 * @brief 更新语句
 * @ingroup Statement
 */
class UpdateStmt : public Stmt
{
public:
  UpdateStmt() = default;
  UpdateStmt(Table *table, const char *field_name, Value value, FilterStmt *filter_stmt);
  virtual StmtType type() const override { return StmtType::UPDATE; }
  virtual ~UpdateStmt();

public:
  static RC create(Db *db, const UpdateSqlNode &update_sql, Stmt *&stmt);

public:
  Table *table() const { return table_; }
  Value values() const { return value_; }
  FilterStmt *filter() const { return filter_stmt_; }
  const char *field() const { return field_name_; }

private:
  Table *table_        = nullptr;
  const char *field_name_ = nullptr;
  Value value_       ;
  FilterStmt *filter_stmt_  = nullptr;

};