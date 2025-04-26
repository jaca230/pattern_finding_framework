#pragma once
#include "PFAlgorithmStage.h"
#include "PFTracklet.h"
#include "PFVertexSet.h"
#include "PFTrackletSet.h"
#include <set>
#include <memory>

class PFVertexFormer : public PFAlgorithmStage {
public:
    PFVertexFormer() {
        setOrderIndex(200);  // Set the order index specific to PFVertexFormer
    }

protected:
    // The runImpl method will handle the conversion and call the abstract method.
    std::shared_ptr<PFPipelineObject> runImpl(std::shared_ptr<PFPipelineObject> input) override {
        // Cast the input to a std::shared_ptr<PFTrackletSet>
        std::shared_ptr<PFTrackletSet> trackletSet = std::dynamic_pointer_cast<PFTrackletSet>(input);
        if (!trackletSet) {
            throw std::invalid_argument("Input must be of type PFTrackletSet");
        }

        // Call the abstract method to form the vertices
        std::set<PFVertex> vertices = form(trackletSet->getTracklets());

        // Return the result as a PFVertexSet, which is a derived class of PFPipelineObject
        return std::make_shared<PFVertexSet>(vertices);
    }

    // Abstract method that will be implemented by derived classes to form vertices
    virtual std::set<PFVertex> form(const std::set<PFTracklet>& tracklets) = 0;
};
