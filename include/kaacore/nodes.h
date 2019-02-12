#pragma once

#include <vector>
#include <memory>
#include <glm/glm.hpp>

#include "kaacore/renderer.h"
#include "kaacore/shape.h"
#include "kaacore/physics.h"


enum struct NodeType {
    basic = 1,
    space = 2,
    body = 3,
    hitbox = 4,
};


struct NodeRenderData {
    std::vector<StandardVertexData> computed_vertices;
};


struct ForeignNodeWrapper {
    ForeignNodeWrapper() = default;
    virtual ~ForeignNodeWrapper() = default;
};


struct MyForeignWrapper : ForeignNodeWrapper {
    MyForeignWrapper();
    ~MyForeignWrapper();
};


struct Scene;

struct Node {
    const NodeType type = NodeType::basic;
    glm::dvec2 position = {0., 0.};
    double rotation = 0.;
    glm::dvec2 scale = {1., 1.};
    int16_t z_index = 0;
    Shape shape;
    glm::dvec4 color = {1., 1., 1., 1.};
    bool visible = true;

    Scene* scene = nullptr;
    Node* parent = nullptr;
    std::vector<Node*> children;

    std::unique_ptr<ForeignNodeWrapper> node_wrapper;

    NodeRenderData render_data;
    glm::fmat4 matrix;

    union {
        SpaceNode space;
        BodyNode body;
        HitboxNode hitbox;
    };

    Node(NodeType type = NodeType::basic);
    ~Node();

    void add_child(Node* child_node);
    void recalculate_matrix();
    void recalculate_render_data();

    void set_position(const glm::dvec2& position);
    void set_shape(const Shape& shape);
    glm::dvec2 get_absolute_position();
};