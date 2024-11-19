/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by Wangyunlai on 2022/5/22.
//

#include "sql/stmt/update_stmt.h"
#include "storage/db/db.h"
#include "sql/stmt/filter_stmt.h"
// UpdateStmt::UpdateStmt(Table *table, Value *values, int value_amount)
//     : table_(table), values_(values), value_amount_(value_amount)
// {}

UpdateStmt::UpdateStmt(Table *table, const char *field_name, Value value, FilterStmt *filter_stmt)
    : table_(table),field_name_(field_name),value_(value),filter_stmt_(filter_stmt)
{}
    
UpdateStmt::~UpdateStmt()
{
  if(nullptr != filter_stmt_){
    delete filter_stmt_;
    filter_stmt_ = nullptr;
  }
}


RC UpdateStmt::create(Db *db, const UpdateSqlNode &update, Stmt *&stmt)
{
  // 检查基本属性
  const char *table_name = update.relation_name.c_str();
  const char *field_name = update.attribute_name.c_str();
  Value value = update.value;
  if (nullptr == db || nullptr == table_name ) {
    LOG_WARN("invalid argument. db=%p, table_name=%p, value_num=%d",db, table_name, field_name);
    return RC::INVALID_ARGUMENT;
  }

  Table *table = db->find_table(table_name);
  if (nullptr == table) {
    LOG_WARN("no such table. db=%s, table_name=%s", db->name(), table_name);
    return RC::SCHEMA_TABLE_NOT_EXIST;
  }

  //check whether the field exists
  const FieldMeta *field = table->table_meta().field(update.attribute_name.c_str());
  if (nullptr == field) {
    LOG_WARN("no such field. table_name=%s, field_name=%s", table_name, update.attribute_name.c_str());
    return RC::SCHEMA_FIELD_NOT_EXIST;
  }

  std::unordered_map<std::string, Table *> table_map;
  table_map.insert(std::pair<std::string, Table *>(std::string(table_name), table));

  //获取被修改对象需要满足的条件
  FilterStmt *filter_stmt = nullptr;
  RC rc = FilterStmt::create(
    db,table,&table_map,update.conditions.data(),static_cast<int>(update.conditions.size()),filter_stmt);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to create filter stmt. rc=%s", strrc(rc));
    return rc;
  }

  stmt = new UpdateStmt(table,field_name, value,filter_stmt);
  return RC::SUCCESS;
}
