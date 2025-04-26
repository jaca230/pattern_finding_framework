#pragma once
#include <memory>

class PFPipelineObject {
public:
    virtual ~PFPipelineObject() = default;

    // The data and methods that the pipeline object should provide
    // Can define common operations that are shared between pipeline objects
};
