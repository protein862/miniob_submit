
#include "sql/operator/logical_operator.h"

class UpdateLogicalOperator: public LogicalOperator 
 {
    public:
        UpdateLogicalOperator(Table *table,Value value,const char *field);
        virtual ~UpdateLogicalOperator() = default;

        LogicalOperatorType type() const override{return LogicalOperatorType::UPDATE;};
        Table *table() const {return table_;}
        const char *  field_name() const {return field_name_;}
        Value value() const {return value_;}
    private:
        Table *table_ = nullptr;
        Value value_;
        const char *field_name_ = nullptr;
        
};