
#include "sql/operator/physical_operator.h"

class UpdatePhysicalOperator: public PhysicalOperator {
    public:
        UpdatePhysicalOperator(Table *table,const char *field_name,Value value):
            table_(table),field_name_(field_name),value_(value){};

        virtual ~UpdatePhysicalOperator() = default;

        PhysicalOperatorType type() const override{return PhysicalOperatorType::UPDATE;};

        RC open(Trx *trx) override;
        RC next() override;
        RC close() override;

        Tuple *current_tuple() const {return nullptr;}

    private:
        Table *table_ = nullptr;
        const char *field_name_ = nullptr;
        Value value_ ;
        Trx *trx_ = nullptr;
        std::vector<Record> records_;
};