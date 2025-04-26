#pragma once
#include "PFPipelineObject.h"
#include <memory>

template <typename DataType>
class PFPipelineObjectContainer : public PFPipelineObject {
public:
    PFPipelineObjectContainer() = default;
    PFPipelineObjectContainer(const DataType& data) : data_(data) {}

    const DataType& get() const { return data_; }
    void set(const DataType& data) { data_ = data; }

private:
    DataType data_;
};
