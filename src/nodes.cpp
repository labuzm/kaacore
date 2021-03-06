#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <unordered_set>

#include <glm/gtc/matrix_transform.hpp>

#include "kaacore/engine.h"
#include "kaacore/exceptions.h"
#include "kaacore/geometry.h"
#include "kaacore/log.h"
#include "kaacore/nodes.h"
#include "kaacore/scenes.h"
#include "kaacore/shapes.h"
#include "kaacore/views.h"

namespace kaacore {

const int16_t default_root_z_index = 0;
const ViewIndexSet default_root_views =
    std::unordered_set<int16_t>{views_default_z_index};

inline double
_normalize_angle(const double angle)
{
    return std::remainder(angle, 2 * M_PI);
}

Node::Node(NodeType type) : _type(type)
{
    if (type == NodeType::space) {
        new (&this->space) SpaceNode();
    } else if (type == NodeType::body) {
        new (&this->body) BodyNode();
    } else if (type == NodeType::hitbox) {
        new (&this->hitbox) HitboxNode();
        this->_color = {1., 0., 0., 0.};
    } else if (type == NodeType::text) {
        new (&this->text) TextNode();
        this->_origin_alignment = Alignment::center;
    }
}

Node::~Node()
{
    if (this->_parent != nullptr) {
        auto pos_in_parent = std::find(
            this->_parent->_children.begin(), this->_parent->_children.end(),
            this);
        if (pos_in_parent != this->_parent->_children.end()) {
            this->_parent->_children.erase(pos_in_parent);
        }
    }

    while (not this->_children.empty()) {
        delete this->_children[0];
    }

    if (this->_type == NodeType::space) {
        if (this->_scene) {
            this->_scene->unregister_simulation(this);
        }
        this->space.~SpaceNode();
    } else if (this->_type == NodeType::body) {
        this->body.~BodyNode();
    } else if (this->_type == NodeType::hitbox) {
        this->hitbox.~HitboxNode();
    } else if (this->_type == NodeType::text) {
        this->text.~TextNode();
    }
}

void
Node::_mark_dirty()
{
    this->_render_data.is_dirty = true;
    this->_model_matrix.is_dirty = true;
    this->_spatial_data.is_dirty = true;
    for (auto child : this->_children) {
        if (not child->_model_matrix.is_dirty) {
            child->_mark_dirty();
        }
    }
}

void
Node::_mark_ordering_dirty()
{
    this->_ordering_data.is_dirty = true;
    for (auto child : this->_children) {
        if (not child->_ordering_data.is_dirty) {
            child->_mark_ordering_dirty();
        }
    }
}

void
Node::_mark_to_delete()
{
    if (this->_marked_to_delete) {
        return;
    }
    KAACORE_ASSERT(this->_scene != nullptr, "Node not attached to the tree.");
    this->_marked_to_delete = true;
    if (this->_node_wrapper) {
        this->_node_wrapper->on_detach();
    }
    this->_scene->spatial_index.stop_tracking(this);
    for (auto child : this->_children) {
        child->_mark_to_delete();
    }

    // Physics have side effect if we don't perform
    // deletion immediately, so we give it special
    // treatment here by detaching them from simulation.
    if (this->_type == NodeType::body) {
        this->body.detach_from_simulation();
    } else if (this->_type == NodeType::hitbox) {
        this->hitbox.detach_from_simulation();
    }
}

glm::fmat4
Node::_compute_model_matrix(const glm::fmat4& parent_matrix) const
{
    return glm::scale(
        glm::rotate(
            glm::translate(
                parent_matrix,
                glm::fvec3(this->_position.x, this->_position.y, 0.)),
            static_cast<float>(this->_rotation), glm::fvec3(0., 0., 1.)),
        glm::fvec3(this->_scale.x, this->_scale.y, 1.));
}

glm::fmat4
Node::_compute_model_matrix_cumulative(const Node* const ancestor) const
{
    const Node* pointer = this;
    std::vector<const Node*> inheritance_chain{pointer};
    while ((pointer = pointer->_parent) != ancestor) {
        if (pointer == nullptr) {
            throw kaacore::exception("Can't compute position relative to node "
                                     "that isn't its parent.");
        }
        inheritance_chain.push_back(pointer);
    }

    glm::fmat4 matrix(1.0);
    for (auto it = inheritance_chain.rbegin(); it != inheritance_chain.rend();
         it++) {
        matrix = (*it)->_compute_model_matrix(matrix);
    }
    return matrix;
}

void
Node::_recalculate_model_matrix()
{
    const static glm::fmat4 identity(1.0);
    this->_model_matrix.value = this->_compute_model_matrix(
        this->_parent ? this->_parent->_model_matrix.value : identity);
    this->_model_matrix.is_dirty = false;
}

void
Node::_recalculate_model_matrix_cumulative()
{
    Node* pointer = this;
    std::vector<Node*> inheritance_chain{pointer};
    while ((pointer = pointer->_parent) != nullptr and
           pointer->_model_matrix.is_dirty) {
        inheritance_chain.push_back(pointer);
    }

    for (auto it = inheritance_chain.rbegin(); it != inheritance_chain.rend();
         it++) {
        (*it)->_recalculate_model_matrix();
    }
}

void
Node::_set_position(const glm::dvec2& position)
{
    if (position != this->_position) {
        this->_mark_dirty();
    }
    this->_position = position;
}

void
Node::_set_rotation(const double rotation)
{
    auto normalized_rotation = _normalize_angle(rotation);
    if (normalized_rotation != this->_rotation) {
        this->_mark_dirty();
    }
    this->_rotation = normalized_rotation;
}

NodePtr
Node::add_child(NodeOwnerPtr& owned_ptr)
{
    KAACORE_CHECK(owned_ptr, "Cannot attach uninitialized/released node.");
    KAACORE_CHECK(owned_ptr->_parent == nullptr, "Node has a parent already.");

    auto child_node = owned_ptr.release();
    child_node->_parent = this;
    this->_children.push_back(child_node.get());

    if (child_node->_node_wrapper) {
        child_node->_node_wrapper->on_add_to_parent();
    }

    // TODO set root
    // TODO optimize (replace with iterator?)
    std::function<void(Node*)> initialize_node;
    initialize_node = [&initialize_node, this](Node* n) {
        bool added_to_scene =
            (n->_scene == nullptr and this->_scene != nullptr);
        n->_scene = this->_scene;
        if (added_to_scene) {
            n->_scene->spatial_index.start_tracking(n);
            if (n->_node_wrapper) {
                n->_node_wrapper->on_attach();
            }
        }
        if (added_to_scene and n->_type == NodeType::space) {
            n->_scene->register_simulation(n);
        } else if (n->_type == NodeType::body) {
            n->body.attach_to_simulation();
        } else if (n->_type == NodeType::hitbox) {
            n->hitbox.update_physics_shape();
        }

        std::for_each(
            n->_children.begin(), n->_children.end(), initialize_node);
    };

    initialize_node(child_node.get());
    return child_node;
}

void
Node::recalculate_model_matrix()
{
    if (not this->_model_matrix.is_dirty) {
        return;
    }
    this->_recalculate_model_matrix();
}

void
Node::recalculate_render_data()
{
    if (not this->_render_data.is_dirty) {
        return;
    }

    // TODO optimize
    glm::dvec2 pos_realignment = calculate_realignment_vector(
        this->_origin_alignment, this->_shape.vertices_bbox);
    this->_render_data.computed_vertices = this->_shape.vertices;
    for (auto& vertex : this->_render_data.computed_vertices) {
        glm::dvec4 pos = {vertex.xyz.x + pos_realignment.x,
                          vertex.xyz.y + pos_realignment.y, vertex.xyz.z, 1.};
        pos = this->_model_matrix.value * pos;
        vertex.xyz = {pos.x, pos.y, pos.z};

        if (this->_sprite.has_texture()) {
            auto uv_rect = this->_sprite.get_display_rect();
            vertex.uv = glm::mix(uv_rect.first, uv_rect.second, vertex.uv);
        }

        vertex.rgba *= this->_color;
    }

    if (this->_sprite.has_texture()) {
        this->_render_data.texture_handle =
            this->_sprite.texture->texture_handle;
    } else {
        this->_render_data.texture_handle =
            get_engine()->renderer->default_texture;
    }
    this->_render_data.is_dirty = false;
}

void
Node::recalculate_ordering_data()
{
    if (not this->_ordering_data.is_dirty) {
        return;
    }

    if (this->_views.has_value()) {
        this->_ordering_data.calculated_views = *this->_views;
    } else if (this->is_root()) {
        this->_ordering_data.calculated_views = default_root_views;
    } else {
        KAACORE_ASSERT(
            this->_parent != nullptr,
            "Can't inherit view data if node has no parent");
        this->_parent->recalculate_ordering_data();
        this->_ordering_data.calculated_views =
            this->_parent->_ordering_data.calculated_views;
    }

    if (this->_z_index.has_value()) {
        this->_ordering_data.calculated_z_index = *this->_z_index;
    } else if (this->is_root()) {
        this->_ordering_data.calculated_z_index = default_root_z_index;
    } else {
        KAACORE_ASSERT(
            this->_parent != nullptr,
            "Can't inherit z_index data if node has no parent");
        this->_ordering_data.calculated_z_index =
            this->_parent->_ordering_data.calculated_z_index;
    }
    this->_ordering_data.is_dirty = false;
}

const NodeType
Node::type() const
{
    return this->_type;
}

const std::vector<Node*>&
Node::children()
{
    return this->_children;
}

bool
Node::is_root() const
{
    return (this->_scene != nullptr and &this->_scene->root_node == this);
}

glm::dvec2
Node::position()
{
    return this->_position;
}

void
Node::position(const glm::dvec2& position)
{
    this->_set_position(position);
    if (this->_type == NodeType::body) {
        this->body.override_simulation_position();
    } else if (this->_type == NodeType::hitbox) {
        this->hitbox.update_physics_shape();
    }
}

glm::dvec2
Node::absolute_position()
{
    if (this->_model_matrix.is_dirty) {
        this->_recalculate_model_matrix_cumulative();
    }

    glm::fvec4 pos = {0., 0., 0., 1.};
    pos = this->_model_matrix.value * pos;
    return {pos.x, pos.y};
}

glm::dvec2
Node::get_relative_position(const Node* const ancestor)
{
    if (ancestor == this->_parent) {
        return this->position();
    } else if (ancestor == nullptr) {
        return this->absolute_position();
    } else if (ancestor == this) {
        return {0., 0.};
    }

    glm::fvec4 pos = {0., 0., 0., 1.};
    pos = this->_compute_model_matrix_cumulative(ancestor) * pos;
    return {pos.x, pos.y};
}

double
Node::rotation()
{
    return this->_rotation;
}

double
Node::absolute_rotation()
{
    if (this->_model_matrix.is_dirty) {
        this->_recalculate_model_matrix_cumulative();
    }

    return DecomposedTransformation<float>(this->_model_matrix.value).rotation;
}

void
Node::rotation(const double& rotation)
{
    this->_set_rotation(rotation);
    if (this->_type == NodeType::body) {
        this->body.override_simulation_rotation();
    } else if (this->_type == NodeType::hitbox) {
        this->hitbox.update_physics_shape();
    }
}

glm::dvec2
Node::scale()
{
    return this->_scale;
}

glm::dvec2
Node::absolute_scale()
{
    if (this->_model_matrix.is_dirty) {
        this->_recalculate_model_matrix_cumulative();
    }

    return DecomposedTransformation<float>(this->_model_matrix.value).scale;
}

void
Node::scale(const glm::dvec2& scale)
{
    if (scale != this->_scale) {
        this->_mark_dirty();
    }
    this->_scale = scale;
    if (this->_type == NodeType::body) {
        for (const auto& n : this->_children) {
            if (n->_type == NodeType::hitbox) {
                n->hitbox.update_physics_shape();
            }
        }
    } else if (this->_type == NodeType::hitbox) {
        this->hitbox.update_physics_shape();
    }
}

Transformation
Node::absolute_transformation()
{
    if (this->_model_matrix.is_dirty) {
        this->_recalculate_model_matrix_cumulative();
    }
    return Transformation{this->_model_matrix.value};
}

Transformation
Node::get_relative_transformation(const Node* const ancestor)
{
    if (ancestor == nullptr) {
        return this->absolute_transformation();
    } else if (ancestor == this) {
        return Transformation{glm::dmat4(1.)};
    }

    return Transformation{this->_compute_model_matrix_cumulative(ancestor)};
}

Transformation
Node::transformation()
{
    return this->get_relative_transformation(this->_parent);
}

void
Node::transformation(const Transformation& transformation)
{
    auto decomposed = transformation.decompose();
    this->position(decomposed.translation);
    this->rotation(decomposed.rotation);
    this->scale(decomposed.scale);
}

std::optional<int16_t>
Node::z_index()
{
    return this->_z_index;
}

void
Node::z_index(const std::optional<int16_t>& z_index)
{
    this->_z_index = z_index;
    this->_mark_ordering_dirty();
}

Shape
Node::shape()
{
    return this->_shape;
}

void
Node::shape(const Shape& shape)
{
    this->shape(shape, false);
}

void
Node::shape(const Shape& shape, bool is_auto_shape)
{
    this->_shape = shape;
    if (not shape) {
        this->_auto_shape = true;
    } else {
        this->_auto_shape = is_auto_shape;
    }

    if (this->_type == NodeType::hitbox) {
        this->hitbox.update_physics_shape();
    }
    // TODO: check if we aren't setting the same shape before marking it dirty
    this->_render_data.is_dirty = true;
    this->_spatial_data.is_dirty = true;
}

Sprite
Node::sprite()
{
    return this->_sprite;
}

void
Node::sprite(const Sprite& sprite)
{
    this->_sprite = sprite;
    if (this->_auto_shape) {
        if (sprite) {
            this->shape(Shape::Box(sprite.get_size()), true);
        } else {
            this->shape(Shape{});
        }
    }
    // TODO: check if we aren't setting the same sprite before marking it dirty
    this->_render_data.is_dirty = true;
}

glm::dvec4
Node::color()
{
    return this->_color;
}

void
Node::color(const glm::dvec4& color)
{
    if (color != this->_color) {
        this->_render_data.is_dirty = true;
    }
    this->_color = color;
}

bool
Node::visible()
{
    return this->_visible;
}

void
Node::visible(const bool& visible)
{
    if (visible and visible != this->_visible) {
        this->_mark_dirty();
    }
    this->_visible = visible;
}

Alignment
Node::origin_alignment()
{
    return this->_origin_alignment;
}

void
Node::origin_alignment(const Alignment& alignment)
{
    if (alignment != this->_origin_alignment) {
        this->_render_data.is_dirty = true;
    }
    this->_origin_alignment = alignment;
}

NodeTransitionHandle
Node::transition()
{
    return this->_transitions_manager.get(default_transition_name);
}

void
Node::transition(const NodeTransitionHandle& transition)
{
    this->_transitions_manager.set(default_transition_name, transition);
}

Duration
Node::lifetime()
{
    return this->_lifetime;
}

void
Node::lifetime(const Duration& lifetime)
{
    this->_lifetime =
        std::chrono::duration_cast<HighPrecisionDuration>(lifetime);
}

NodeTransitionsManager&
Node::transitions_manager()
{
    return this->_transitions_manager;
}

Scene* const
Node::scene() const
{
    return this->_scene;
}

NodePtr
Node::parent() const
{
    return this->_parent;
}

void
Node::views(const std::optional<std::unordered_set<int16_t>>& z_indices)
{
    if (z_indices.has_value()) {
        KAACORE_CHECK(
            z_indices->size() <= KAACORE_MAX_VIEWS, "Invalid indices size.");
    }

    this->_views = z_indices;
    this->_mark_ordering_dirty();
}

const std::optional<std::vector<int16_t>>
Node::views() const
{
    return this->_views;
}

void
Node::setup_wrapper(std::unique_ptr<ForeignNodeWrapper>&& wrapper)
{
    KAACORE_ASSERT(!this->_node_wrapper, "Node wrapper already initialized.");
    this->_node_wrapper = std::move(wrapper);
}

ForeignNodeWrapper*
Node::wrapper_ptr() const
{
    return this->_node_wrapper.get();
}

void
Node::indexable(const bool indexable_flag)
{
    if (this->_indexable != indexable_flag) {
        this->_indexable = indexable_flag;
        this->_spatial_data.is_dirty = true;
    }
}

bool
Node::indexable() const
{
    return this->_indexable;
}

BoundingBox<double>
Node::bounding_box()
{
    const auto transformation = this->absolute_transformation();

    if (this->_shape) {
        KAACORE_ASSERT(
            not this->_shape.bounding_points.empty(),
            "Shape must have bounding points");
        std::vector<glm::dvec2> bounding_points;
        bounding_points.resize(this->_shape.bounding_points.size());
        std::transform(
            this->_shape.bounding_points.begin(),
            this->_shape.bounding_points.end(), bounding_points.begin(),
            [&transformation](glm::dvec2 pt) -> glm::dvec2 {
                return pt | transformation;
            });
        return BoundingBox<double>::from_points(bounding_points);
    } else {
        return BoundingBox<double>::single_point(
            this->_position | transformation);
    }
}

} // namespace kaacore
