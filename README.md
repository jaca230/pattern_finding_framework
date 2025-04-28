# Pattern Finder

**Pattern Finder** is a modular and extensible C++ project for processing and grouping low-level objects ("tracklets") into higher-level structures ("vertices" and "patterns") following a customizable algorithm pipeline.  
It is designed with flexibility in mind, allowing easy swapping or tuning of different stages of the pattern finding workflow.

---

---

## Key Concepts

- **Tracklet**: A basic object representing a small collection of hits or measurements.
- **Vertex**: A group of tracklets clustered together, typically spatially or temporally.
- **Pattern**: A larger structure composed of multiple vertices that represent an event or object.
- **Pipeline**: An ordered sequence of algorithm stages that progressively build up from tracklets to full patterns.

---

## Build Instructions

You need `CMake` and a C++17 compatible compiler.  
If you're using **conda**, a C++ toolchain like `gcc` should already be available.

```bash
# From the project root
mkdir -p build
cd build
cmake ..
make -j12
```

Alternatively, you can use the provided helper script:

```bash
scripts/build.sh
```

The executable will be located at:

```bash
build/bin/pattern_finder
```

---

## Run Instructions

After building:

```bash
scripts/run.sh
```
*(or manually run `build/bin/pattern_finder`)*

---

## Important Classes

- `PFAlgorithmPipeline`: Manages the sequential execution of algorithm stages.
- `PFDefaultTrackletFormer`: Default tracklet forming logic.
- `PFKMeansVertexFormer`: Vertex forming using K-Means clustering.
- `PFDefaultPatternFormer`: Default pattern forming from vertices.
- `PFEvent`, `PFTracklet`, `PFVertex`, `PFPattern`: Core container classes.

---

## Development Notes

- **C++17** standard is used.
- Modular design with emphasis on **extensibility** — adding new algorithm stages is easy.

---

## 📚 Examples

---

### Example 1: Building and Running a Simple Pipeline

Here’s a minimal example showing how to create an event, build a pipeline, and run it:

```cpp
#include <iostream>
#include <memory>

#include "PFAlgorithmPipeline.h"
#include "PFDefaultTrackletFormer.h"
#include "PFKMeansVertexFormer.h"
#include "PFDefaultPatternFormer.h"
#include "PFPipelineObjectContainer.h"
#include "PFEvent.h"
#include "PFPattern.h"
#include "PFTracklet.h"
#include "PFVertex.h"

#include <nlohmann/json.hpp>

int main() {
    // Create a new event
    PFEvent event(42);

    // Create and configure the algorithm pipeline
    std::shared_ptr<PFAlgorithmPipeline> pipeline = std::make_shared<PFAlgorithmPipeline>();

    pipeline->registerStage(std::make_shared<PFDefaultTrackletFormer>());

    // Configure KMeans vertex former with sigma and number of iterations
    double sigma = 1.5;
    int n_iters = 4;
    pipeline->registerStage(std::make_shared<PFKMeansVertexFormer>(sigma, n_iters));

    pipeline->registerStage(std::make_shared<PFDefaultPatternFormer>());

    // Attach pipeline to the event
    event.setPipeline(pipeline);

    // Prepare input JSON (tracklets)
    nlohmann::json input = {
        { "tracklet1", {
            { "particle_id", 100 },
            { "endpoint0", {0.0, 0.0, 0.0} },
            { "endpoint1", {1.0, 1.0, 1.0} }
        }},
        { "tracklet2", {
            { "particle_id", 101 },
            { "endpoint0", {1.05, 1.05, 1.05} },
            { "endpoint1", {2.0, 2.0, 2.0} }
        }},
        { "tracklet3", {
            { "particle_id", 102 },
            { "endpoint0", {2.05, 2.05, 2.05} },
            { "endpoint1", {3.0, 3.0, 3.0} }
        }}
    };

    auto pipelineInput = std::make_shared<PFPipelineObjectContainer<nlohmann::json>>(input);

    // Run the pipeline
    auto output = event.getPipeline()->runPipeline(pipelineInput);

    // Cast output to a container of patterns
    auto patternContainer = std::dynamic_pointer_cast<PFPipelineObjectContainer<
        std::unordered_set<std::shared_ptr<PFPattern>>>>(output);

    if (!patternContainer) {
        std::cerr << "Error: Final output is not a pattern set." << std::endl;
        return 1;
    }

    // Store resulting patterns into the event
    event.setPatterns(patternContainer->get());

    // Print results
    std::cout << "Event ID: " << event.getEventId() << std::endl;
    std::cout << "Number of patterns: " << event.getPatterns().size() << std::endl;

    for (const auto& pattern : event.getPatterns()) {
        std::cout << "Pattern ID: " << pattern->getId() << std::endl;
        for (const auto& tracklet : pattern->getUniqueTracklets()) {
            std::cout << "  Tracklet ID: " << tracklet->getTrackletId() << std::endl;
        }
    }

    return 0;
}
```

---

### Example 2: Creating a New Vertex Former

If you want to create a custom **VertexFormer**, inherit from `PFVertexFormer` and implement the `form()` method.

```cpp
#include "PFVertexFormer.h"

class DummyVertexFormer : public PFVertexFormer {
protected:
    std::unordered_set<std::shared_ptr<PFVertex>> form(const std::unordered_set<std::shared_ptr<PFTracklet>>& tracklets) override {
        std::unordered_set<std::shared_ptr<PFVertex>> vertices;
        int vertexIdCounter = 0;

        for (const auto& tracklet : tracklets) {
            auto vertex = std::make_shared<PFVertex>(vertexIdCounter++);
            vertex->addTracklet(tracklet);
            vertices.insert(vertex);
        }

        return vertices;
    }
};
```

✅ **Register** it inside your pipeline:

```cpp
pipeline->registerStage(std::make_shared<DummyVertexFormer>());
```

---

### Example 3: Creating a New Algorithm Stage

If you want to create a **completely new AlgorithmStage**, directly inherit from `PFAlgorithmStage` and implement the `runImpl()` function.

Here’s an example of an **event logger stage** that simply prints the number of incoming tracklets:

```cpp
#include "PFAlgorithmStage.h"
#include "PFTracklet.h"
#include "PFPipelineObjectContainer.h"
#include <iostream>

class TrackletLoggerStage : public PFAlgorithmStage {
public:
    TrackletLoggerStage() {
        setOrderIndex(50);  // Run early in the pipeline
        setId("tracklet-logger");
    }

protected:
    std::shared_ptr<PFPipelineObject> runImpl(std::shared_ptr<PFPipelineObject> input) override {
        auto trackletContainer = std::dynamic_pointer_cast<
            PFPipelineObjectContainer<std::unordered_set<std::shared_ptr<PFTracklet>>>>(input);

        if (!trackletContainer) {
            throw std::invalid_argument("Input must be PFPipelineObjectContainer<std::unordered_set<std::shared_ptr<PFTracklet>>>");
        }

        const auto& tracklets = trackletContainer->get();
        std::cout << "[TrackletLoggerStage] Number of tracklets: " << tracklets.size() << std::endl;

        // Pass input through unchanged
        return input;
    }
};
```

✅ **Register** it inside your pipeline:

```cpp
pipeline->registerStage(std::make_shared<TrackletLoggerStage>());
```

---

# ✅ Quick Table

| Task | Inherit From | What You Override | Example |
|:-----|:-------------|:-------------------|:--------|
| New Vertex Former | `PFVertexFormer` | `form(tracklets)` | `PFKMeansVertexFormer` |
| New Algorithm Stage | `PFAlgorithmStage` | `runImpl(input)` | `PFVertexFormer` |

---
